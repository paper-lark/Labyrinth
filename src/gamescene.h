#ifndef GAMESCENE_H
#define GAMESCENE_H

/* Headers */
#include <assert.h>
#include "auxiliary.h"
#include "main.h"

/* MACRO */
#define DELAY 2
#define PLAYER 'o'
#define WALL '#'
#define EMPTY ' '
#define EXIT '^'
#define MINOTAUR '@'
#define UNKNOWN '.'
#define min2(x,y) (x < y) ? x : y
#define min3(x, y, z) min2(x, min2(y, z))

/* Prototypes */
typedef enum {
    SinglePlayer = 0, Hotseat = 1, Multiplayer = 2
} GameMode;
void init_game(const int width, const int height, const GameMode mode);

#endif