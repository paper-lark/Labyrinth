/* Headers */
#include "aux.h"
#include <stdlib.h>
#include <assert.h>
#if defined _WIN64 || defined _WIN32
    #include <curses.h>
#elif defined __linuwidth__ || defined __APPLE__
    #include <ncurses.h>
#endif

/* MACRO */
#define MAX_SIZE 30
#define PLAYER 'o'
#define WALL '#'
#define EMPTY ' '
#define aumin(x,y) (x < y) ? x : y
#define min(x, y, z) aumin(x, aumin(y, z))

/* Prototypes */
int menu(const int width, const int height);
int play(const int width, const int height);
void show(WINDOW *game_scene, State **map, const unsigned size);

/* Implementation */
int main(void) {
    /* Init */
    initscr();
    noecho();
    cbreak(); 
    //curs_set(0);
    refresh();
    const int width = getmaxx(stdscr), height = getmaxy(stdscr);

    assert(width > 4 || height > 4);

    /* Main */
    menu(width, height);

    /* Epilogue */
    getch();
    endwin();
    return 0;
}

int menu(const int width, const int height) {
    return play(width, height);
}

int play(const int width, const int height) {
    const unsigned size = makeodd(min(height - 2, width, MAX_SIZE));

    WINDOW *game_scene = newwin(size, size, 0, (width - size) / 2);
    WINDOW *log_scene = newwin(2, width, height - 2, 0);
    keypad(game_scene, TRUE);

    State **map = create_labyrinth(size);

    show(game_scene, map, size);
    mvwprintw(log_scene, 0, 0, "Log:");
    wrefresh(log_scene);
    free_m(map, size);
    return 0;
}

void show(WINDOW *game_scene, State **map, const unsigned size) {
    for (int y = 0; y < size; y++) {
        wmove(game_scene, y, 0);
        for (int x = 0; x < size; x++) {
            switch(map[x][y]) {
                case Empty:
                    waddch(game_scene, EMPTY);
                    break;
                case Player:
                    waddch(game_scene, PLAYER);
                    break;
                case Wall:
                    waddch(game_scene, WALL);
                    break;
                default:
                    assert(0);
            }
        }
    }
    wrefresh(game_scene);
}