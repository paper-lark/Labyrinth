#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"
#include <time.h>

#if defined _WIN64 || defined _WIN32
    #include <curses.h>
#elif defined __linuwidth__ || defined __APPLE__
    #include <ncurses.h>
#endif

#define MAX_SIZE 30
#define PLAYER 'o'
#define WALL '#'
#define EMPTY ' '
#define aumin(x,y) (x < y) ? x : y
#define min(x, y, z) aumin(x, aumin(y, z))
#define isInside(x, y, size) (x >= 0) && (y >= 0) && (y < size) && (x < size)

typedef enum {
    Wall, Empty, Player, Exit
} State;

int menu(const int width, const int height);
int play(const int width, const int height);
State **create_labyrinth(const unsigned size);
void show(WINDOW *game_scene, State **map, const unsigned size);
void free_m(State **map, const unsigned size);
unsigned makeodd(unsigned x);

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

State **create_labyrinth(const unsigned size) {

    /* Init map */
    State **map = malloc(size * sizeof(void *));
    for (int i = 0; i < size; i++) {
        map[i] = malloc(size * sizeof(State));
        for(int j = 0; j < size; j++) {
            map[i][j] = Wall;
        }
    }

    /* Init auxiliary values */
    List *walls = nlist(); // Wall list
    srand(time(0));
    unsigned cell_x = makeodd(rand() % size), cell_y = makeodd(rand() % size); // Initial cell coordinates
    unsigned wall_x, wall_y;
    Direction dir;
    

    /* Build */
    map[cell_x][cell_y] = Empty;
    if (isInside(cell_x - 1, cell_y, size) && isInside(cell_x - 2, cell_y, size)) { // Left
        add(walls, cell_x - 1, cell_y, left);
    }
    if (isInside(cell_x + 1, cell_y, size) && isInside(cell_x + 2, cell_y, size)) { // Right
        add(walls, cell_x + 1, cell_y, right);
    }
    if (isInside(cell_x, cell_y - 1, size) && isInside(cell_x, cell_y - 2, size)) { // Up
        add(walls, cell_x, cell_y - 1, up);
    }
    if (isInside(cell_x, cell_y + 1, size) && isInside(cell_x, cell_y + 2, size)) { // Down
        add(walls, cell_x, cell_y + 1, down);
    }

    while(!isEmpty(walls)) {

        // Pick random and free memory
        Entry *current = getrandom(walls);
        wall_x = current->value.x;
        wall_y = current->value.y;
        dir = current->value.dir;
        free(current);

        switch(dir) {
            case right:
                cell_x = wall_x + 1;
                cell_y = wall_y;
                break;
            case left:
                cell_x = wall_x - 1;
                cell_y = wall_y;
                break;
            case up:
                cell_x = wall_x;
                cell_y = wall_y - 1;
                break;
            case down:
                cell_x = wall_x;
                cell_y = wall_y + 1;
                break;
            default:
                assert(0);
        }

        if (map[cell_x][cell_y] == Wall) {// If cell in the direction is a wall
            map[wall_x][wall_y] = map[cell_x][cell_y] = Empty; // Mark both wall and destination empty

            if (isInside(cell_x - 1, cell_y, size) && isInside(cell_x - 2, cell_y, size) && map[cell_x - 2][cell_y] == Wall) { // Left
                add(walls, cell_x - 1, cell_y, left);
            }
            if (isInside(cell_x + 1, cell_y, size) && isInside(cell_x + 2, cell_y, size) && map[cell_x + 2][cell_y] == Wall) { // Right
                add(walls, cell_x + 1, cell_y, right);
            }
            if (isInside(cell_x, cell_y - 1, size) && isInside(cell_x, cell_y - 2, size) && map[cell_x][cell_y - 2] == Wall) { // Up
                add(walls, cell_x, cell_y - 1, up);
            }
            if (isInside(cell_x, cell_y + 1, size) && isInside(cell_x, cell_y + 2, size) && map[cell_x][cell_y + 2] == Wall) { // Down
                add(walls, cell_x, cell_y + 1, down);
            }
        }
    }
    free(walls);

    return map;
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

void free_m(State **map, const unsigned size) {
    for (int i = 0; i < size; i++) {
        free(map[i]);
    }
    free(map);
}

unsigned makeodd(unsigned x) {
    if (x == 0) {
        return 1;
    } else  {
        return (x % 2 == 1) ? x : x - 1;
    }
}