#ifndef MAIN_H
#define MAIN_H

/* Headers */
#include <stdlib.h>
#if defined _WIN64 || defined _WIN32
    #include <curses.h>
#elif defined __linux__
    #include <ncurses.h>
#elif defined __APPLE__
    #include <ncursesw/ncurses.h>
#endif

/* MACRO */
#define VERSION "Version 2.0.1"
#define MIN_WIDTH 64
#define MIN_HEIGHT 15

#endif
