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
#include <assert.h>
#include <string.h>
#include "main.h"
#include "gamescene.h"

/* Prototypes */
void menu(const int width, const int height);
void init_setup(const int width, const int height);
void init_settings(const int width, const int height);
void init_help(const int width, const int height);
void init_about(const int width, const int height);
void init_exit(const int width, const int height);
void write_info(WINDOW *info_scene, char *msg);
void show_menu(WINDOW* background_scene, WINDOW *menu_scene, const char *title, const char *entries[], const unsigned length, const int hlt);

/* Implementation */
void menu(const int width, const int height) {
    /* Init scene */
    const unsigned mwidth = 64, mheight = 15;
    WINDOW *background_scene = newwin(height, width, 0, 0);
    WINDOW *menu_scene = newwin(mheight, mwidth, (height - mheight) / 2, (width - mwidth) / 2);
    keypad(menu_scene, TRUE);
    unsigned highlight = 0, choice = -1;
    const char *title = "Labyrinth";
    const char *entries[] = { "Play",
                              "Settings",
                              "Help",
                              "About",
                              "Exit"};
    const unsigned length = 5;
    wattron(menu_scene, COLOR_PAIR(1));
    show_menu(background_scene, menu_scene, title, entries, length, highlight);

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

        show_menu(background_scene, menu_scene, title, entries, length, highlight);

        if (choice == 0) { // Play
            init_setup(width, height);
            show_menu(background_scene, menu_scene, title, entries, length, highlight);
            cbreak();
            choice = -1;
        } else if (choice == 1) { // Settings
            init_settings(width, height);
            show_menu(background_scene, menu_scene, title, entries, length, highlight);
            choice = -1;
        } else if (choice == 2) { // Help
            init_help(width, height);
            show_menu(background_scene, menu_scene, title, entries, length, highlight);
            choice = -1;
        } else if (choice == 3) { // About
            init_about(width, height);
            show_menu(background_scene, menu_scene, title, entries, length, highlight);
            choice = -1;
        } else if (choice == 4) { // Exit
            init_exit(width, height);
            break;
        }
    }
    return;
}

void write_info(WINDOW *info_scene, char *msg) { 
    wclear(info_scene);
    unsigned x = getmaxx(info_scene);
    mvwprintw(info_scene, 0, (x - strlen(msg)) / 2, msg);
    wrefresh(info_scene);
}

void show_menu(WINDOW *background_scene, WINDOW *menu_scene, const char *title, const char *entries[], const unsigned length, const int hlt) {
    wclear(background_scene);
    wrefresh(background_scene);
    const unsigned x = getmaxx(menu_scene);
    box(menu_scene, 0, 0);
    wattron(menu_scene, A_BOLD);
    mvwprintw(menu_scene, 1, (x - strlen(title)) / 2, title);
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
    mvwprintw(help_scene, 6, 7, "%c - minotaur", MINOTAUR);
    mvwprintw(help_scene, 7, 7, "%c - fog", UNKNOWN);
    mvwprintw(help_scene, 8, 7, "%c - wall", WALL);
    mvwprintw(help_scene, 9, 7, "%c - exit", EXIT);
    mvwprintw(help_scene, 10, 3, "Good luck on your journey and don't get caught!");
    mvwprintw(help_scene, 11, 3, "Here's hoping you'll like it :D");
    mvwprintw(help_scene, mheight - 2, (mwidth - 25) / 2, "<Press any key to return>");

    wrefresh(help_scene);
    wgetch(help_scene);
}

void init_about(const int width, const int height) {
    const unsigned mwidth = 64, mheight = 15;
    WINDOW *background_scene = newwin(height, width, 0, 0);
    wrefresh(background_scene);
    //assert(width >= mwidth && height >= mheight);

    WINDOW *about_scene = newwin(mheight, mwidth, (height - mheight) / 2, (width - mwidth) / 2);
    keypad(about_scene, TRUE);
    wattron(about_scene, COLOR_PAIR(1));
    box(about_scene, 0, 0);

    wattron(about_scene, A_BOLD);
    mvwprintw(about_scene, 1, (mwidth - 4) / 2, "About");
    wattroff(about_scene, A_BOLD);

    mvwprintw(about_scene, 3, (mwidth - 16) / 2, "A labyrinth game");
    mvwprintw(about_scene, 4, (mwidth - 50) / 2, "Huge thanks to my best friend, Migle Kucinskaite ");
    {
        wattron(about_scene, COLOR_PAIR(6));
        wattron(about_scene, A_BOLD);
        wprintw(about_scene, "<3");
        wattroff(about_scene, COLOR_PAIR(6));
        wattroff(about_scene, A_BOLD);
    }
    mvwprintw(about_scene, 5, (mwidth - 41) / 2, "Software is distributed under MIT License");
    mvwprintw(about_scene, 6, (mwidth - 24) / 2, "Created by Max Zhuravsky");
    mvwprintw(about_scene, 7, (mwidth - 12) / 2, "Moscow, 2017");
    mvwprintw(about_scene, 9, (mwidth - 13) / 2, VERSION);
    mvwprintw(about_scene, mheight - 2, (mwidth - 25) / 2, "<Press any key to return>");

    wrefresh(about_scene);
    wgetch(about_scene);
}

void init_exit(const int width, const int height) {
    const unsigned mwidth = 64, mheight = 15;
    WINDOW *background_scene = newwin(height, width, 0, 0);
    wrefresh(background_scene);
    //assert(width >= mwidth && height >= mheight);

    WINDOW *exit_scene = newwin(mheight, mwidth, (height - mheight) / 2, (width - mwidth) / 2);
    keypad(exit_scene, TRUE);
    wattron(exit_scene, COLOR_PAIR(1));
    box(exit_scene, 0, 0);

    mvwprintw(exit_scene, 3, (mwidth - 24) / 2, "Hope you had some fun :)");
    mvwprintw(exit_scene, 4, (mwidth - 30) / 2, "Have a lovely day and see you!");
    mvwprintw(exit_scene, mheight - 2, (mwidth - 25) / 2, "<Press any key to quit>");

    wrefresh(exit_scene);
    wgetch(exit_scene);
}

void init_settings(const int width, const int height) {
    const unsigned mwidth = 64, mheight = 15;
    WINDOW *background_scene = newwin(height, width, 0, 0);
    wrefresh(background_scene);
    assert(width >= mwidth && height >= mheight);

    WINDOW *settings_scene = newwin(mheight, mwidth, (height - mheight) / 2, (width - mwidth) / 2);
    keypad(settings_scene, TRUE);
    wattron(settings_scene, COLOR_PAIR(1));
    box(settings_scene, 0, 0);

    mvwprintw(settings_scene, 3, (mwidth - 52) / 2, "Oops! Looks like this menu has not been created yet.");
    mvwprintw(settings_scene, 4, (mwidth - 15) / 2, "Don't panic! :)");
    mvwprintw(settings_scene, 4, (mwidth - 56) / 2, "We'll do our best to get it working as soon as possible.");
    mvwprintw(settings_scene, mheight - 2, (mwidth - 25) / 2, "<Press any key to return>");

    wrefresh(settings_scene);
    wgetch(settings_scene);
}

void init_setup(const int width, const int height) {
    /* Init scene */
    const unsigned mwidth = 30, mheight = 15;
    WINDOW *background_scene = newwin(height, width, 0, 0);
    WINDOW *setup_scene = newwin(mheight, mwidth, (height - mheight) / 2, (width - mwidth) / 2);
    keypad(setup_scene, TRUE);
    unsigned highlight = 0, choice = -1;
    const char *title = "Select Game Mode";
    const char *entries[] = { "Single Player",
                              "Hotseat",
                              "Multiplayer"};
    const unsigned length = 3;
    wattron(setup_scene, COLOR_PAIR(1));
    show_menu(background_scene, setup_scene, title, entries, length, highlight);

    /* Get the choice */
    while (choice == -1) {
        int in = wgetch(setup_scene);
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
        show_menu(background_scene, setup_scene, title, entries, length, highlight);
    }
    if (choice == Hotseat || choice == SinglePlayer) {
        init_game(width, height, choice);
    } else {
        //::ToDo Establish connection and start the game
    }
}