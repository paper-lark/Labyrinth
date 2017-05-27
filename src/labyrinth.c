/**********************************************************************************
 * Copyright (c) 2017 Max Zhuravsky                                               *
 *                                                                                *
 * Permission is hereby granted, free of charge, to any person obtaining a copy   *
 * of this software and associated documentation files (the "Software"), to deal  *
 * in the Software without restriction, including without limitation the rights   *
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      *
 * copies of the Software, and to permit persons to whom the Software is          *
 * furnished to do so, subject to the following conditions:                       *
 *                                                                                *
 * The above copyright notice and this permission notice shall be included in all *
 * copies or substantial portions of the Software.                                *
 *                                                                                *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  *
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  *
 * SOFTWARE.                                                                      *
 **********************************************************************************/

/**********************************************************************************
 *                                 Labyrinth Game                                 *
 *                  Author:  Max Zhuravsky <paperlark@yandex.ru>                  *
 **********************************************************************************/

/* Headers */
#include "auxiliary.h"
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
#define VERSION "Version 1.1.0"
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
void menu(const int width, const int height);
void init_game(const int width, const int height);
void init_help(const int width, const int height);
void init_about(const int width, const int height);
void show_game(WINDOW *game_scene, State **map, Hidden **fog, const unsigned size, const Point *player, const Point *pexit);
void write_info(WINDOW *info_scene, char *msg);
void show_menu(WINDOW* background_scene, WINDOW *menu_scene, const char *entries[], const unsigned length, const int hlt) ;

/* Implementation */
int main(void) {
    /* Init */
    initscr();
    noecho();
    cbreak(); 
    curs_set(0);
    refresh();

    /* Init Colors */
    start_color();
    if (can_change_color()) {
        init_color(COLOR_RED, 900, 0, 0);
        init_color(COLOR_BLACK, 30, 30, 30);
    }
    init_pair(1, COLOR_WHITE, COLOR_BLACK); // Menu
    init_pair(2, COLOR_CYAN, COLOR_BLACK); // Player
    init_pair(3, COLOR_MAGENTA, COLOR_BLACK); // Walls
    init_pair(4, COLOR_YELLOW, COLOR_BLACK); // Exit
    init_pair(5, COLOR_RED, COLOR_BLACK); // Heart

    /* Start */
    const int width = getmaxx(stdscr), height = getmaxy(stdscr);
    menu(width, height);

    /* Epilogue */
    endwin();
    return 0;
}

void menu(const int width, const int height) {
    /* Init scene */
    const unsigned mwidth = 30, mheight = 10;
    WINDOW *background_scene = newwin(height, width, 0, 0);
    WINDOW *menu_scene = newwin(mheight, mwidth, (height - mheight) / 2, (width - mwidth) / 2);
    keypad(menu_scene, TRUE);
    unsigned highlight = 0, choice = -1;
    const char *entries[] = { "Play",
                            "Help",
                            "About",
                            "Exit"};
    const unsigned length = 4;
    wattron(menu_scene, COLOR_PAIR(1));
    show_menu(background_scene, menu_scene, entries, length, highlight);

    /* Get the choice */
    while (TRUE) {
        int in = wgetch(menu_scene);
        switch (in) {
            case KEY_DOWN:
                highlight = (highlight + 1) % length;
                break;
            case KEY_UP:
                highlight = (highlight + length - 1) % length;
                break;
            case 10:
                choice = highlight;
                break;
        }

        show_menu(background_scene, menu_scene, entries, length, highlight);

        if (choice == 0) { // Play
            init_game(width, height);
            //wattron(menu_scene, COLOR_PAIR(1));
            show_menu(background_scene, menu_scene, entries, length, highlight);
            choice = -1;
        } else if (choice == 1) { // Help
            init_help(width, height);
            //wattron(menu_scene, COLOR_PAIR(1));
            show_menu(background_scene, menu_scene, entries, length, highlight);
            choice = -1;
        } else if (choice == 2) { // About
            init_about(width, height);
            //wattron(menu_scene, COLOR_PAIR(1));
            show_menu(background_scene, menu_scene, entries, length, highlight);
            choice = -1;
        } else if (choice == 3) { // Exit
            //::TODO Add goodbye screen
            break;
        }
    }
    return;
}

void init_game(const int width, const int height) {

    /* Init scenes */
    const unsigned size = makeodd(min(height - 1, width, MAX_SIZE));
    WINDOW *background_scene = newwin(height, width, 0, 0);
    WINDOW *game_scene = newwin(size, size, 0, (width - size) / 2);
    WINDOW *info_scene = newwin(1, width, height - 1, 0);
    keypad(game_scene, TRUE);

    /* Create labyrinth */
    State **map = create_labyrinth(size);
    Point *player = rand_position(map, size);
    Point *pexit = rand_position(map, size);
    Hidden **fog = create_hid(size);
    reveal(fog, player, size);

    /* Show scenes */
    wrefresh(background_scene);
    show_game(game_scene, map, fog, size, player, pexit);

    /* Logic */
    Point *target = malloc(sizeof(Point));
    while (player->x != pexit->x || player->y != pexit->y) {
        int in = wgetch(game_scene);
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
        }
        if (isInside(target->x, target->y, size) && map[target->x][target->y] == Empty) {
            player->x = target->x;
            player->y = target->y;
            reveal(fog, player, size);
            show_game(game_scene, map, fog, size, player, pexit);
        }
    }
    write_info(info_scene, "<Exit Reached. Press ENTER to continue>");
    while (TRUE) {
        int in = wgetch(info_scene);
        if (in == 10)
            break;
    }

    /* Free memory */
    free_st(map, size);
    free_hid(fog, size);
    free(player);
    free(pexit);
    free(target);
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
                        wattron(game_scene, COLOR_PAIR(3));
                        waddch(game_scene, WALL);
                        wattroff(game_scene, COLOR_PAIR(3));
                        break;
                    default:
                        assert(0);
                }
            } else {
                wattron(game_scene, COLOR_PAIR(1));
                waddch(game_scene, UNKNOWN);
                wattroff(game_scene, COLOR_PAIR(1));
            }
        }
    }

    {
        wattron(game_scene, A_BOLD);
        wattron(game_scene, COLOR_PAIR(2));
        mvwaddch(game_scene, player->y, player->x, PLAYER);
        wattroff(game_scene, COLOR_PAIR(2));
        wattroff(game_scene, A_BOLD);
    }

    if (fog[pexit->x][pexit->y] == 1) {
        wattron(game_scene, A_BOLD);
        wattron(game_scene, COLOR_PAIR(4));
        mvwaddch(game_scene, pexit->y, pexit->x, EXIT);
        wattroff(game_scene, COLOR_PAIR(4)); 
        wattroff(game_scene, A_BOLD);
    }

    wrefresh(game_scene);
}

void write_info(WINDOW *info_scene, char *msg) {
    wclear(info_scene);
    unsigned x = getmaxx(info_scene);
    mvwprintw(info_scene, 0, (x - strlen(msg)) / 2, msg);
    wrefresh(info_scene);
}

void show_menu(WINDOW *background_scene, WINDOW *menu_scene, const char *entries[], const unsigned length, const int hlt) {
    wclear(background_scene);
    wrefresh(background_scene);
    const unsigned x = getmaxx(menu_scene);
    box(menu_scene, 0, 0);
    wattron(menu_scene, A_BOLD);
    mvwprintw(menu_scene, 1, (x - 9) / 2, "Labyrinth");
    wattroff(menu_scene, A_BOLD);
    for (unsigned i = 0; i < length; i++) {
        if (hlt == i) {
            wattron(menu_scene, A_REVERSE);
            mvwprintw(menu_scene, i + 3, 3, "%s", entries[i]);
            wattroff(menu_scene, A_REVERSE);
        } else {
            mvwprintw(menu_scene, i + 3, 3, "%s", entries[i]);
        }
    }
    wrefresh(menu_scene);
}

void init_help(const int width, const int height) {
    const unsigned mwidth = 64, mheight = 15;
    WINDOW *background_scene = newwin(height, width, 0, 0);
    wrefresh(background_scene);
    assert(width >= mwidth && height >= mheight);

    WINDOW *help_scene = newwin(mheight, mwidth, (height - mheight) / 2, (width - mwidth) / 2);
    keypad(help_scene, TRUE);
    wattron(help_scene, COLOR_PAIR(1));
    box(help_scene, 0, 0);

    wattron(help_scene, A_BOLD);
    mvwprintw(help_scene, 1, (mwidth - 4) / 2, "Help");
    wattroff(help_scene, A_BOLD);

    mvwprintw(help_scene, 3, 3, "Hey there, young (well, maybe not) labyrinth explorer!");
    mvwprintw(help_scene, 4, 3, "Here's a brief legend for you, so you wouldn't get lost :)");
    mvwprintw(help_scene, 5, 7, "%c - player", PLAYER);
    mvwprintw(help_scene, 6, 7, "%c - fog", UNKNOWN);
    mvwprintw(help_scene, 7, 7, "%c - wall", WALL);
    mvwprintw(help_scene, 8, 7, "%c - exit", EXIT);
    mvwprintw(help_scene, 9, 3, "Best of luck on your journey!");
    mvwprintw(help_scene, 10, 3, "Here's hoping you'll like it :)");
    mvwprintw(help_scene, mheight - 2, (mwidth - 25) / 2, "<Press any key to return>");

    wrefresh(help_scene);
    wgetch(help_scene);
}

void init_about(const int width, const int height) {
    const unsigned mwidth = 64, mheight = 12;
    WINDOW *background_scene = newwin(height, width, 0, 0);
    wrefresh(background_scene);
    assert(width >= mwidth && height >= mheight);

    WINDOW *about_scene = newwin(mheight, mwidth, (height - mheight) / 2, (width - mwidth) / 2);
    keypad(about_scene, TRUE);
    wattron(about_scene, COLOR_PAIR(1));
    box(about_scene, 0, 0);

    wattron(about_scene, A_BOLD);
    mvwprintw(about_scene, 1, (mwidth - 4) / 2, "About");
    wattroff(about_scene, A_BOLD);

    mvwprintw(about_scene, 3, (mwidth - 16) / 2, "A labyrinth game"); //::TODO Add license
    mvwprintw(about_scene, 4, (mwidth - 50) / 2, "Huge thanks to my best friend, Migle Kucinskaite ");
    {
        wattron(about_scene, COLOR_PAIR(5));
        wattron(about_scene, A_BOLD);
        wprintw(about_scene, "<3");
        wattroff(about_scene, COLOR_PAIR(5));
        wattroff(about_scene, A_BOLD);
    }
    mvwprintw(about_scene, 5, (mwidth - 24) / 2, "Created by Max Zhuravsky");
    mvwprintw(about_scene, 6, (mwidth - 12) / 2, "Moscow, 2017");
    mvwprintw(about_scene, 8, (mwidth - 13) / 2, VERSION);
    mvwprintw(about_scene, mheight - 2, (mwidth - 25) / 2, "<Press any key to return>");

    wrefresh(about_scene);
    wgetch(about_scene);
}