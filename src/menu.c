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
#include "connect.h"
#include "menu.h"

/* Prototypes */
void menu(const int width, const int height);
void init_select(const int width, const int height);
void init_mpmenu(const int width, const int height);
void init_settings(const int width, const int height);
void init_help(const int width, const int height);
void init_about(const int width, const int height);
void init_exit(const int width, const int height);
void show_info(WINDOW *info_scene, const MAlign alignment, const TInfo type, const char *msg);
void show_menu(WINDOW *menu_scene, const char *title, const char *entries[], const unsigned length, const MAlign alignment, const unsigned hlt);
void show_txtfld(WINDOW* menu_scene, const char *title, const char *text, const unsigned txtindex, const unsigned hlt);

/* Implementation */
void menu(const int width, const int height) {
    /* Init scene */
    const unsigned mwidth = 64, mheight = 10;
    const MAlign alignment = Center;
    WINDOW *background_scene = newwin(height, width, 0, 0);
    WINDOW *menu_scene = newwin(mheight, mwidth, (height - mheight) / 2, (width - mwidth) / 2);
    keypad(menu_scene, TRUE);
    unsigned highlight = 0, choice = (unsigned)-1;
    const char *title = "Labyrinth";
    const char *entries[] = { "Play",
                              "Help",
                              "About me",
                              "Exit"};
    const unsigned length = 4;
    wattron(menu_scene, COLOR_PAIR(1));

    /* Initial Refresh */
    wrefresh(background_scene);
    show_menu(menu_scene, title, entries, length, alignment, highlight);

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

        show_menu(menu_scene, title, entries, length, alignment, highlight);

        if (choice == 0) { // Play
            init_select(width, height);
            wclear(background_scene);
            wrefresh(background_scene);
            show_menu(menu_scene, title, entries, length, alignment, highlight);
            choice = (unsigned)-1;
        } else if (choice == 1) { // Help
            init_help(width, height);
            wclear(background_scene);
            wrefresh(background_scene);
            show_menu(menu_scene, title, entries, length, alignment, highlight);
            choice = (unsigned)-1;
        } else if (choice == 2) { // About
            init_about(width, height);
            wclear(background_scene);
            wrefresh(background_scene);
            show_menu(menu_scene, title, entries, length, alignment, highlight);
            choice = (unsigned)-1;
        } else if (choice == 3) { // Exit
            init_exit(width, height);
            break;
        }
    }
    return;
}

void init_help(const int width, const int height) {
    const unsigned mwidth = 64, mheight = 15;
    WINDOW *background_scene = newwin(height, width, 0, 0);
    wrefresh(background_scene);

    WINDOW *help_scene = newwin(mheight, mwidth, (height - mheight) / 2, (width - mwidth) / 2);
    keypad(help_scene, TRUE);
    wattron(help_scene, COLOR_PAIR(1));
    box(help_scene, 0, 0);

    /* Title */
    wattron(help_scene, A_BOLD);
    mvwprintw(help_scene, 1, (mwidth - 4) / 2, "Help");
    wattroff(help_scene, A_BOLD);

    /* Body */
    mvwprintw(help_scene, 3, 3, "Hey there, young (well, maybe not) labyrinth explorer!");
    mvwprintw(help_scene, 4, 3, "Here's a brief legend for you, so you wouldn't get lost :)");
    mvwprintw(help_scene, 5, 7, "%c - human", HUMAN);
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

    WINDOW *about_scene = newwin(mheight, mwidth, (height - mheight) / 2, (width - mwidth) / 2);
    keypad(about_scene, TRUE);
    wattron(about_scene, COLOR_PAIR(1));
    box(about_scene, 0, 0);

    /* Title */
    wattron(about_scene, A_BOLD);
    mvwprintw(about_scene, 1, (mwidth - 4) / 2, "About");
    wattroff(about_scene, A_BOLD);

    /* Body */
    mvwprintw(about_scene, 3, (mwidth - 16) / 2, "A labyrinth game");
    mvwprintw(about_scene, 4, (mwidth - 50) / 2, "Huge thanks to my best friend, Migle Kucinskaite ");
    {
        wattron(about_scene, COLOR_PAIR(6));
        wattron(about_scene, A_BOLD);
        wprintw(about_scene, "<3");
        wattroff(about_scene, COLOR_PAIR(6));
        wattroff(about_scene, A_BOLD);
    }
    mvwprintw(about_scene, 5, (mwidth - 41) / 2, "Software is distributed under terms of MIT License");
    mvwprintw(about_scene, 6, (mwidth - 24) / 2, "Created by Max Zhuravsky");
    mvwprintw(about_scene, 7, (mwidth - 12) / 2, "Moscow, 2017");
    mvwprintw(about_scene, 9, (mwidth - 11 - strlen(BUILD_TYPE)) / 2, "Version %d.%d (%s)", VERSION_MAJOR, VERSION_MINOR, BUILD_TYPE);
    mvwprintw(about_scene, mheight - 2, (mwidth - 25) / 2, "<Press any key to return>");

    wrefresh(about_scene);
    wgetch(about_scene);
}

void init_exit(const int width, const int height) {
    const unsigned mwidth = 64, mheight = 15;
    WINDOW *background_scene = newwin(height, width, 0, 0);
    wrefresh(background_scene);

    WINDOW *exit_scene = newwin(mheight, mwidth, (height - mheight) / 2, (width - mwidth) / 2);
    keypad(exit_scene, TRUE);
    wattron(exit_scene, COLOR_PAIR(1));
    box(exit_scene, 0, 0);

    /* Body */
    mvwprintw(exit_scene, 3, (mwidth - 24) / 2, "Hope you had some fun :)");
    mvwprintw(exit_scene, 4, (mwidth - 30) / 2, "Have a lovely day and see you!");
    mvwprintw(exit_scene, mheight - 2, (mwidth - 25) / 2, "<Press any key to quit>");

    wrefresh(exit_scene);
    wgetch(exit_scene);
}

void init_select(const int width, const int height) {
    /* Init scene */
    const unsigned mwidth = 64, mheight = 15;
    const MAlign alignment = Center;
    WINDOW *background_scene = newwin(height, width, 0, 0);
    WINDOW *setup_scene = newwin(mheight, mwidth, (height - mheight) / 2, (width - mwidth) / 2);
    keypad(setup_scene, TRUE);
    unsigned highlight = 0, choice = (unsigned)-1;
    const char *title = "Select Game Mode";
    const char *entries[] = { "Single Player",
                              "Hotseat",
                              "Multiplayer",
                              "Back"};
    const unsigned length = 4;
    wattron(setup_scene, COLOR_PAIR(1));

    /* Initial Refresh */
    wrefresh(background_scene);
    show_menu(setup_scene, title, entries, length, alignment, highlight);

    /* Get the choice */
    while (choice == (unsigned)-1) {
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
        show_menu(setup_scene, title, entries, length, alignment, highlight);
    }

    /* Init game */
    if (choice == Hotseat || choice == SinglePlayer) {
        init_server(width, height, choice, Human, (USock) NULL);
        return;
    } else if (choice == Multiplayer) {
        init_mpmenu(width, height);
        return;
    }
}

void init_mpmenu(const int width, const int height) {
    /* Init scene */
    const unsigned mwidth = 64;
    const MAlign alignment = Center;
    WINDOW *background_scene = newwin(height, width, 0, 0);
    WINDOW *side_scene = newwin(10, mwidth, (height - 14) / 2, (width - mwidth) / 2);
    keypad(background_scene, TRUE);
    unsigned highlight = 0, choice = (unsigned)-1;
    const char *side_title = "Choose your side";
    const char *entries[] = { "Server (Human)",
                              "Server (Minotaur)",
                              "Client",
                              "Back"};
    const unsigned length = 4;
    wattron(side_scene, COLOR_PAIR(1));

    WINDOW *ip_scene = newwin(4, mwidth, (height - 14) / 2 + 12, (width - mwidth) / 2);
    const unsigned txtindex = length;
    const char *ip_title = "IPv4 to connect (for client only)";
    char buffer[100];
    memset(buffer, 0, 100);
    unsigned current = 0;
    wattron(ip_scene, COLOR_PAIR(1));

    WINDOW *info_scene = newwin(2, width, height - 3, 0);

    /* Initial Refresh */
    wrefresh(background_scene);
    show_menu(side_scene, side_title, entries, length, alignment, highlight);
    show_txtfld(ip_scene, ip_title, buffer, txtindex, highlight);
    show_info(info_scene, Center, Message, "");

    /* Get the choice */
    while (TRUE) {
        int in = wgetch(background_scene);
        switch (in) {
            case KEY_DOWN:
                highlight = (highlight + 1) % (length + 1);
                break;
            case KEY_UP:
                highlight = (highlight + length) % (length + 1);
                break;
            case 10:
                if (highlight < length)
                    choice = highlight;
                break;
            case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': case '0': case '.':
                if (highlight == txtindex && current < 100) {
                    buffer[current++] = (char)in;
                }
                break;
            case KEY_BACKSPACE: case 127: case 8:
                if (highlight == txtindex) {
                    buffer[--current] = '\0';
                }
                break;
        }

        /* Update */
        show_menu(side_scene, side_title, entries, length, alignment, highlight);
        show_txtfld(ip_scene, ip_title, buffer, txtindex, highlight);

        /* Init game */
        if (choice == 0) { // Server (Human)
            USock sockfd;
            if ((sockfd = create_server(info_scene)) == USOCK_ERROR) {
                choice = (unsigned)-1;
                /* Refresh */
                // wclear(background_scene);
                // wrefresh(background_scene);
                // show_menu(side_scene, side_title, entries, length, alignment, highlight);
                // show_txtfld(ip_scene, ip_title, buffer, txtindex, highlight);
                // wrefresh(info_scene);
                continue;
            }
            Affiliation client_side = Minotaur;
            send(sockfd, &client_side, sizeof(Affiliation), 0);
            init_server(width, height, Multiplayer, Human, sockfd);
            close_usocket(sockfd);
            return;
        } else if (choice == 1) { // Server (Minotaur)
            USock sockfd;
            if ((sockfd = create_server(info_scene)) == USOCK_ERROR) {
                choice = (unsigned)-1;
                /* Refresh */
                // wclear(background_scene);
                // wrefresh(background_scene);
                // show_menu(side_scene, side_title, entries, length, alignment, highlight);
                // show_txtfld(ip_scene, ip_title, buffer, txtindex, highlight);
                // wrefresh(info_scene);
                continue;
            }
            Affiliation client_side = Human;
            send(sockfd, &client_side, sizeof(Affiliation), 0);
            init_server(width, height, Multiplayer, Minotaur, sockfd);
            close_usocket(sockfd);
            return;
        } else if (choice == 2) { // Client
            char *ip = buffer;
            USock sockfd;
            if ((sockfd = create_client(ip, info_scene)) == USOCK_ERROR) {
                choice = (unsigned)-1;
                /* Refresh */
                // wclear(background_scene);
                // wrefresh(background_scene);
                // show_menu(side_scene, side_title, entries, length, alignment, highlight);
                // show_txtfld(ip_scene, ip_title, buffer, txtindex, highlight);
                // wrefresh(info_scene);
                continue;
            }
            Affiliation side;
            recv(sockfd, &side, sizeof(Affiliation), 0);
            init_client(width, height, side, sockfd);
            close_usocket(sockfd);
            return;
        } else if (choice == 3) { // Back
            return;
        }
    }
}

void show_menu(WINDOW *menu_scene, const char *title, const char *entries[], const unsigned length, const MAlign alignment, const unsigned hlt) {
    /* Clear */
    wclear(menu_scene);
    const unsigned x = getmaxx(menu_scene);
    const unsigned y = getmaxy(menu_scene);
    const unsigned gap = (y - length - 3) / 6;
    box(menu_scene, 0, 0);

    /* Title */
    wattron(menu_scene, A_BOLD);
    mvwprintw(menu_scene, 1, (x - strlen(title)) / 2, title);
    wattroff(menu_scene, A_BOLD);

    /* Body */
    for (unsigned i = 0; i < length; i++) {
        if (hlt == i) {
            wattron(menu_scene, A_REVERSE);
        }
        switch (alignment) {
            case Center:
                mvwprintw(menu_scene, i + 3 + gap, (x - strlen(entries[i])) / 2, entries[i]);
                break;
            case Left:
                mvwprintw(menu_scene, i + 3 + gap, 3, entries[i]);
                break;
        }
        if (hlt == i) {
            wattroff(menu_scene, A_REVERSE);
        }
    }

    /* Refresh */
    wrefresh(menu_scene);
}

void show_txtfld(WINDOW* menu_scene, const char *title, const char *text, const unsigned txtindex, const unsigned hlt) {  
    /* Clear */
    wclear(menu_scene);
    wrefresh(menu_scene);
    box(menu_scene, 0, 0);

    /* Title */
    const unsigned x = getmaxx(menu_scene), y = getmaxy(menu_scene);
    wattron(menu_scene, A_BOLD);
    mvwprintw(menu_scene, 1, (x - strlen(title)) / 2, title);
    wattroff(menu_scene, A_BOLD);

    /* Body */
    if (hlt == txtindex) {
        wattron(menu_scene, A_REVERSE);
        mvwprintw(menu_scene, y / 2, 3, "%s", text);
        wattroff(menu_scene, A_REVERSE);
    } else {
        mvwprintw(menu_scene, y / 2, 3, "%s", text);
    }

    /* Refresh */
    wrefresh(menu_scene);
}

void show_info(WINDOW *info_scene, const MAlign alignment, const TInfo type, const char *msg) { 
    /* Clear */
    wclear(info_scene);

    /* Body */
    const unsigned width = getmaxx(info_scene);
    unsigned x;
    switch (alignment) {
        case Center:
            x = (width - strlen(msg)) / 2;
            break;
        case Left:
            x = 2;
            break;
        default:
            fprintf(stderr, "Unknown alignment option\n");
            exit(1);
    }

    switch(type) {
        case Log:
            mvwprintw(info_scene, 0, x, "LOG: %s", msg);
            break;
        case Message:
            mvwprintw(info_scene, 0, x, msg);
            break; 
        case Error:
            mvwprintw(info_scene, 0, x, "ERROR: %s", msg);
            break;
        default:
            fprintf(stderr, "Unknown message type option\n");
            exit(1);
    }

    /* Refresh */
    wrefresh(info_scene);
}