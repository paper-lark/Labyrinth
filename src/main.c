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
 #include "main.h"
 #include "menu.h"

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
    init_pair(2, COLOR_MAGENTA, COLOR_BLACK); // Player
    init_pair(3, COLOR_GREEN, COLOR_BLACK); // Walls
    init_pair(4, COLOR_YELLOW, COLOR_BLACK); // Exit
    init_pair(5, COLOR_RED, COLOR_BLACK); // Minotaur
    init_pair(6, COLOR_RED, COLOR_BLACK); // Heart

    /* Check for proper size of the window and start */
    const int width = getmaxx(stdscr), height = getmaxy(stdscr);
    if (width < MIN_WIDTH || height < MIN_HEIGHT) {
        endwin();
        fprintf(stderr, "Looks like your terminal is way too small.\nWe need at least a %dx%d screen for the app to work properly.\nSorry about that :c\n", 
                MIN_WIDTH, MIN_HEIGHT);
        exit(1);
    } else {
        menu(width, height);
        endwin();
    }
    
    return 0;
}