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
#include "transmit.h"

/* Implementation */
int send_point(USock sockfd, const Point *pt) {
    if (send(sockfd, (const char *)pt, sizeof(Point), 0) == USOCK_ERROR)
        return -1;
    return 0;
}
int recv_point(USock sockfd, Point *pt) {
    if (recv(sockfd, (char *)pt, sizeof(Point), 0) == USOCK_ERROR)
        return -1;
    return 0;
}

int send_initial_info(USock sockfd, const Point size, State **map, const Point *player, const Point *door, const Point *minotaur) {
    /* Send map */
    for (unsigned i = 0; i < size.x; i++)
        for(unsigned j = 0; j < size.y; j++) {
            if (send(sockfd, (const char *)&map[i][j], sizeof(State), 0) == -1)
                return -1;
        }

    /* Send player, door, minotaur locations */
    if (send(sockfd, (const char *)player, sizeof(Point), 0) == USOCK_ERROR)
        return -1;
    if (send(sockfd, (const char *)door, sizeof(Point), 0) == USOCK_ERROR)
        return -1;
    if (send(sockfd, (const char *)minotaur, sizeof(Point), 0) == USOCK_ERROR)
        return -1;

    return 0;
}

int recv_initial_info(USock sockfd, const Point size, State **map, Point *player, Point *door, Point *minotaur) {
    /* Receive map */
    for (unsigned i = 0; i < size.x; i++)
        for(unsigned j = 0; j < size.y; j++) {
            if (recv(sockfd, (char *)&map[i][j], sizeof(State), 0) == USOCK_ERROR)
                return -1;
        }

    /* Receive player, door, minotaur locations */
    if (recv(sockfd, (char *)player, sizeof(Point), 0) == USOCK_ERROR)
        return -1;
    if (recv(sockfd, (char *)door, sizeof(Point), 0) == USOCK_ERROR)
        return -1;
    if (recv(sockfd, (char *)minotaur, sizeof(Point), 0) == USOCK_ERROR)
        return -1;

    return 0;
}

int send_status(USock sockfd, GameStatus type, const Direction value) {
    if (send(sockfd, (const char *)&type, sizeof(GameStatus), 0) == USOCK_ERROR)
        return -1;
    if (type == InProgress)
        if (send(sockfd, (const char *)&value, 1, 0) == USOCK_ERROR)
            return -1;
    return 0;
}

int recv_status(USock sockfd, GameStatus *type, Point *position) {
    if (recv(sockfd, (char *)type, sizeof(GameStatus), 0) == USOCK_ERROR)
        return -1;
    if (*type == InProgress) {
        static const Point direction[] = { {.x = 0, .y = 0},
                                           {.x = 0, .y = -1},
                                           {.x = 0, .y = 1},
                                           {.x = -1, .y = 0},
                                           {.x = 1, .y = 0} };
        Direction delta;
        if (recv(sockfd, (char *)&delta, 1, 0) == USOCK_ERROR)
            return -1;
        position->x += direction[delta].x;
        position->y += direction[delta].y;
    }
    return 0;
}

