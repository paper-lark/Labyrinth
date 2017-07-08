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

#ifndef MAIN_H
#define MAIN_H

/* Headers */
#include <stdlib.h>

/* Curses */
#if defined _WIN64 || defined _WIN32
    #include <curses.h>
#elif defined __linux__
    #include <ncurses.h>
#elif defined __APPLE__
    #include <ncursesw/ncurses.h>
#endif

/* Socket Type */
#if defined _WIN64 || defined _WIN32
    #define USock SOCKET 
    #define USOCK_ERROR SOCKET_ERROR
#else
    #define USock int 
    #define USOCK_ERROR -1
#endif

/* MACRO */
#define VERSION "Version 3.0.0 (WIP)"
#define MIN_WIDTH 64
#define MIN_HEIGHT 16
#define ERROR (USock) USOCK_ERROR // -1?

#endif
