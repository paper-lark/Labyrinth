/* Headers */
#include "aux.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#if defined _WIN64 || defined _WIN32
    #include <curses.h>
#elif defined __linux__ || defined __APPLE__
    #include <ncurses.h>
#endif

/* MACRO */
#define MAX_SIZE 30
#define PLAYER 'o'
#define WALL '#'
#define EMPTY ' '
#define EXIT '^'
#define UNKNOWN '.'
#define aumin(x,y) (x < y) ? x : y
#define min(x, y, z) aumin(x, aumin(y, z))
#define isInside(x, y, size) (x >= 0) && (y >= 0) && (y < size) && (x < size)

/* Prototypes */
int menu(const int width, const int height);
int init_game(const int width, const int height);
void show_game(WINDOW *game_scene, State **map, Hidden **fog, const unsigned size, const Point *player, const Point *pexit);
void write_log(WINDOW *log_scene, char *msg);
void write_info(WINDOW *info_scene, char *msg);

/* Implementation */
int main(void) {
    /* Init */
    initscr();
    noecho();
    cbreak(); 
    //curs_set(0);
    refresh();
    const int width = getmaxx(stdscr), height = getmaxy(stdscr);
    assert(width > 7 && height > 7);
    menu(width, height);

    /* Epilogue */
    endwin();
    return 0;
}

int menu(const int width, const int height) {
    return init_game(width, height);
}

int init_game(const int width, const int height) {

    /* Init scenes */
    const unsigned size = makeodd(min(height - 2, width, MAX_SIZE));
    WINDOW *game_scene = newwin(size, size, 0, (width - size) / 2);
    WINDOW *log_scene = newwin(1, width, height - 2, 0);
    WINDOW *info_scene = newwin(1, width, height - 1, 0);
    keypad(log_scene, TRUE);

    /* Create labyrinth */
    State **map = create_labyrinth(size);
    Point *player = rand_position(map, size);
    Point *pexit = rand_position(map, size);
    Hidden **fog = create_hid(size);
    reveal(fog, player, size);

    /* Show scenes */
    show_game(game_scene, map, fog, size, player, pexit);
    char buffer[100]; //::DEBUG
    sprintf(buffer, "Player: %d %d, Exit: %d %d", player->x, player->y, pexit->x, pexit->y); //::DEBUG
    write_log(log_scene, buffer); //::DEBUG

    /* Logic */
    Point *target = malloc(sizeof(Point));
    while (player->x != pexit->x || player->y != pexit->y) {
        int in = wgetch(log_scene);
        switch (in) {
            case KEY_DOWN:
                target->x = player->x;
                target->y = player->y + 1;
                break;
            case KEY_UP:
                target->x = player->x;
                target->y = player->y - 1;
                break;
            case KEY_LEFT:
                target->x = player->x - 1;
                target->y = player->y;
                break;
            case KEY_RIGHT:
                target->x = player->x + 1;
                target->y = player->y;
                break;
            default:
                write_log(log_scene, "Invalid output");
        }
        if (isInside(target->x, target->y, size) && map[target->x][target->y] == Empty) {
            player->x = target->x;
            player->y = target->y;
            reveal(fog, player, size);
            show_game(game_scene, map, fog, size, player, pexit);
            //write_log(log_scene, "Successfully moved");
            char buffer[100]; //::DEBUG
            sprintf(buffer, "Player: %d %d, Exit: %d %d", player->x, player->y, pexit->x, pexit->y); //::DEBUG
            write_log(log_scene, buffer); //::DEBUG
        } else {
            write_log(log_scene, "Destination is blocked");
        }
    } 
    write_log(log_scene, "Exit reached");
    write_info(info_scene, "<Press any key to exit>");
    wgetch(info_scene);

    /* Free memory */
    free_st(map, size);
    free_hid(fog, size);
    free(player);
    free(pexit);
    free(target);
    return 0;
}

void show_game(WINDOW *game_scene, State **map, Hidden **fog, const unsigned size, const Point *player, const Point *pexit) {
    for (int y = 0; y < size; y++) {
        wmove(game_scene, y, 0);
        for (int x = 0; x < size; x++) {
            if (fog[x][y] == 1) {
                switch(map[x][y]) {
                    case Empty:
                        waddch(game_scene, EMPTY);
                        break;
                    case Wall:
                        waddch(game_scene, WALL);
                        break;
                    default:
                        assert(0);
                }
            } else {
                waddch(game_scene, UNKNOWN);
            }
        }
    }
    mvwaddch(game_scene, player->y, player->x, PLAYER);
    if (fog[pexit->x][pexit->y] == 1)
        mvwaddch(game_scene, pexit->y, pexit->x, EXIT);
    wrefresh(game_scene);
}

void write_log(WINDOW *log_scene, char *msg) {
    wclear(log_scene);
    mvwprintw(log_scene, 0, 0, "Log: ");
    wprintw(log_scene, msg);
    wrefresh(log_scene);
}

void write_info(WINDOW *info_scene, char *msg) {
    wclear(info_scene);
    unsigned x = getmaxx(info_scene);
    mvwprintw(info_scene, 0, (x - strlen(msg)) / 2, msg);
    wrefresh(info_scene);
}