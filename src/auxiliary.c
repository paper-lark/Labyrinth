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

#include "auxiliary.h"
#include <time.h>
#include <stdlib.h>
#include "list.h"
#include <assert.h>
#define isInside(x, y, size) (x >= 0) && (y >= 0) && (y < size) && (x < size)
#define LOOPS 0.1500

State **create_st(const unsigned size) {
    State **map = malloc(size * sizeof(void *));
    for (int i = 0; i < size; i++) {
        map[i] = malloc(size * sizeof(State));
        for(int j = 0; j < size; j++)
            map[i][j] = Wall;
    }
    return map;
}

Hidden **create_hid(const unsigned size) {
    Hidden **map = malloc(size * sizeof(void *));
    for (int i = 0; i < size; i++) {
        map[i] = malloc(size * sizeof(Hidden));
        for(int j = 0; j < size; j++)
            map[i][j] = 1; //::DEBUG 0
    }
    return map;
}

void free_st(State **map, const unsigned size) {
    for (int i = 0; i < size; i++) {
        free(map[i]);
    }
    free(map);
}

void free_hid(Hidden **map, const unsigned size) {
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

State **create_labyrinth(const unsigned size) {

    /* Init map */
    State **map = create_st(size);

    /* Init auxiliary values */
    List *walls = nlist(); // List of walls
    List *secondary = nlist(); // List of walls between visited chambers
    srand(time(0));
    unsigned cell_x = makeodd(rand() % size), cell_y = makeodd(rand() % size); // Initial cell coordinates
    unsigned wall_x, wall_y;
    Direction dir;
    

    /* Build Prim's labyrinth */
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

        if (map[cell_x][cell_y] == Wall) { // If cell in the direction is a wall
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
        } else if ((wall_y != 0) && (wall_y != size - 1) && (wall_x != 0) && (wall_x != size - 1)) {
            add(secondary, wall_x, wall_y, dir);
        }
    }

    /* Add loops */
    unsigned cnt = secondary->length * LOOPS;
    for (unsigned i = 0; i < cnt; i++) {
        Entry *current = getrandom(secondary);
        unsigned wall_x = current->value.x;
        unsigned wall_y = current->value.y;
        map[wall_x][wall_y] = Empty;
        free(current);
    }

    while (!isEmpty(secondary)) {
        Entry *current = getrandom(secondary);
        free(current);
    }
    free(walls);
    free(secondary);

    return map;
}

Point *rand_position(State **map, const unsigned size) {
    Point *result = malloc(sizeof(Point));
    result->x = makeodd(rand() % size);
    result->y = makeodd(rand() % size);
    return result;
}

void reveal(Hidden **fog, const Point *player, const unsigned size) {
    fog[player->x + 1][player->y] = 1;
    fog[player->x - 1][player->y] = 1;
    fog[player->x][player->y + 1] = 1;
    fog[player->x][player->y - 1] = 1;
    fog[player->x - 1][player->y - 1] = 1;
    fog[player->x + 1][player->y - 1] = 1;
    fog[player->x - 1][player->y + 1] = 1;
    fog[player->x + 1][player->y + 1] = 1;
}