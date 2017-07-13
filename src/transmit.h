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
 
#ifndef TRANSMIT_H
#define TRANSMIT_H

/* Headers */
#include "main.h"
#include "auxiliary.h"

/* Types */
typedef enum {
    InProgress = 0, ForceEnd = 1, Caught = 2, Escaped = 3
} GameStatus;

typedef unsigned char Direction;

/* Prototypes */
int send_point(USock sockfd, const Point *pt);
int recv_point(USock sockfd, Point *pt);
int send_initial_info(USock sockfd, const Point size, State **map, const Point *player, const Point *door, const Point *minotaur);
int recv_initial_info(USock sockfd, const Point size, State **map, Point *player, Point *door, Point *minotaur);
int send_status(USock sockfd, GameStatus type, const Direction delta);
int recv_status(USock sockfd, GameStatus *type, Point *position);

#endif