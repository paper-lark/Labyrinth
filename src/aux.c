#include "aux.h"
#include <time.h>
#include <stdlib.h>
#include "list.h"
#include <assert.h>
#define isInside(x, y, size) (x >= 0) && (y >= 0) && (y < size) && (x < size)

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
            map[i][j] = 0;
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