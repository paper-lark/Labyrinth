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
#include "gamescene.h"
#include "menu.h"
#include "transmit.h"
#include "auxiliary.h"

/* Implementation */

void show_game(WINDOW *game_scene, const State **map, const Hidden **fog, const Point *size, const Point *player, const Point *door, const Point *minotaur) {
    for (int y = 0; y < size->y; y++) {
        wmove(game_scene, y, 0);
        for (int x = 0; x < size->x; x++) {
            if (fog[x][y] == 1) {
                switch(map[x][y]) {
                    case Empty:
                        waddch(game_scene, EMPTY);
                        break;
                    case Wall:
                        wattron(game_scene, COLOR_PAIR(3));
                        waddch(game_scene, WALL);
                        wattroff(game_scene, COLOR_PAIR(3));
                        break;
                    default:
                        assert(0);
                }
            } else {
                wattron(game_scene, COLOR_PAIR(1));
                waddch(game_scene, UNKNOWN);
                wattroff(game_scene, COLOR_PAIR(1));
            }
        }
    }

    /* Show player */
    if (fog[player->x][player->y] == 1) {
        wattron(game_scene, A_BOLD);
        wattron(game_scene, COLOR_PAIR(2));
        mvwaddch(game_scene, player->y, player->x, PLAYER);
        wattroff(game_scene, COLOR_PAIR(2));
        wattroff(game_scene, A_BOLD);
    }

    /* Show exit */
    if (fog[door->x][door->y] == 1) {
        wattron(game_scene, A_BOLD);
        wattron(game_scene, COLOR_PAIR(4));
        mvwaddch(game_scene, door->y, door->x, EXIT);
        wattroff(game_scene, COLOR_PAIR(4)); 
        wattroff(game_scene, A_BOLD);
    }

    /* Show minotaur if he is present */
    if (minotaur && fog[minotaur->x][minotaur->y] == 1) {
        wattron(game_scene, A_BOLD);
        wattron(game_scene, COLOR_PAIR(5));
        mvwaddch(game_scene, minotaur->y, minotaur->x, MINOTAUR);
        wattroff(game_scene, COLOR_PAIR(5)); 
        wattroff(game_scene, A_BOLD);
    }

    wrefresh(game_scene);
}

void init_server(const int width, const int height, const GameMode mode, USock sockfd) {

    /* Init scene */
    const Point size = {.x = min2(makeodd(width * 0.75), makeodd(MIN_WIDTH - 1)), .y = min2(makeodd(height - 4), makeodd(MIN_HEIGHT - 1))};
    WINDOW *background_scene = newwin(height, width, 0, 0);
    WINDOW *game_scene = newwin(size.y, size.x, 1, (width - size.x) / 2);
    WINDOW *info_scene = newwin(1, width, height - 1, 0);
    keypad(game_scene, TRUE);
    halfdelay(DELAY);

    /* Create labyrinth */
    State **map = create_labyrinth(size.x, size.y);
    Point *player = rand_position(map, size.x, size.y);
    Point *door = rand_position(map, size.x, size.y);
    Point *minotaur = NULL;
    Hidden **fog = create_hid(size.x, size.y);
    reveal(fog, player, size.x, size.y);
    if (mode == Hotseat) {
        minotaur = pointat(makeodd(size.x / 2), makeodd(size.y / 2));
        reveal(fog, minotaur, size.x, size.y);
    } else if (mode == Multiplayer) {
        minotaur = pointat(makeodd(size.x / 2), makeodd(size.y / 2));
        send_point(sockfd, &size);
        send_initial_info(sockfd, size, map, player, door, minotaur); //Trasmit all the data to the client
    }

    /* Show scene */
    wrefresh(background_scene);
    show_game(game_scene, map, fog, &size, player, door, minotaur);

    /* Logic */
    Point *target = malloc(sizeof(Point));
    while (TRUE) {

        /* Single Player */
        int input = wgetch(game_scene);
        switch (input) {
            case KEY_DOWN:
                target->x = player->x;
                target->y = player->y + 1;
                break;
            case KEY_UP:
                target->x = player->x;
                target->y = player->y - 1;
                break;
            case KEY_LEFT:
                target->x = player->x - 1;
                target->y = player->y;
                break;
            case KEY_RIGHT:
                target->x = player->x + 1;
                target->y = player->y;
                break;
            default:
                target->x = player->x;
                target->y = player->y;
        }
        if (isInside(target->x, target->y, size.x, size.y) && map[target->x][target->y] == Empty) {
            player->x = target->x;
            player->y = target->y;
            reveal(fog, player, size.x, size.y);
            show_game(game_scene, map, fog, &size, player, door, minotaur);
        }

        /* Hotseat Logic */
        if (mode == Hotseat) {
            switch (input) {
                case 'S': case 's':
                   target->x = minotaur->x;
                   target->y = minotaur->y + 1;
                   break;
                case 'W': case 'w':
                    target->x = minotaur->x;
                    target->y = minotaur->y - 1;
                    break;
                case 'A': case 'a':
                    target->x = minotaur->x - 1;
                    target->y = minotaur->y;
                    break;
                case 'D': case 'd':
                    target->x = minotaur->x + 1;
                    target->y = minotaur->y;
                    break;
                default:
                    target->x = minotaur->x;
                    target->y = minotaur->y;
            }
            if (isInside(target->x, target->y, size.x, size.y) && map[target->x][target->y] == Empty) {
                minotaur->x = target->x;
                minotaur->y = target->y;
                reveal(fog, minotaur, size.x, size.y);
                show_game(game_scene, map, fog, &size, player, door, minotaur);
            }
        }

        /* Multiplayer Mode */
        if (mode == Multiplayer) {
            InfoType status;
            send_status(sockfd, Location, player);
            recv_status(sockfd, &status, minotaur);

            /* Update screen */
            show_game(game_scene, map, fog, &size, player, door, minotaur);
        }

        /* Check termination conditions */
        if (player->x == door->x && player->y == door->y) {
            /* Player won! */
            show_info(info_scene, Center, "<Exit Reached. Congratulations! Press ENTER to continue>");
            if (mode == Multiplayer) {
                send_status(sockfd, PlayerWins, NULL); //Send the message to the client
            }
            break;
        } else if (minotaur && player->x == minotaur->x && player->y == minotaur->y) {
            /* Player lost! */
            show_info(info_scene, Center, "<You were caught by the minotaur. Press ENTER to continue>");
            if (mode == Multiplayer) {
                send_status(sockfd, MinotaurWins, NULL); //Send the message to the client
            }
            break;
        }
    }
    
    /* Exit */
    while (TRUE) {
        int input = wgetch(info_scene);
        if (input == 10)
            break;
    }

    /* Free memory */
    free_st(map, size.x);
    free_hid(fog, size.x);
    free(player);
    free(door);
    free(target);
    free(minotaur);
}

void init_client(const int width, const int height, const USock sockfd) {
    /* Receive info */
    Point size;
    recv_point(sockfd, &size);
    State **map = create_st(size.x, size.y);
    Point *player = malloc(sizeof(Point));
    Point *door = malloc(sizeof(Point));
    Point *minotaur = malloc(sizeof(Point));
    recv_initial_info(sockfd, size, map, player, door, minotaur);

    /* Init scene */
    WINDOW *background_scene = newwin(height, width, 0, 0);
    WINDOW *game_scene = newwin(size.y, size.x, 0, (width - size.x) / 2);
    WINDOW *info_scene = newwin(1, width, height - 1, 0);
    keypad(game_scene, TRUE);
    halfdelay(DELAY);

    /* Create labyrinth */
    Hidden **fog = create_hid(size.x, size.y);
    reveal(fog, minotaur, size.x, size.y);

    /* Show scene */
    wrefresh(background_scene);
    show_game(game_scene, map, fog, &size, player, door, minotaur);

    /* Logic */
    Point *target = malloc(sizeof(Point));
    while (TRUE) {
        int input = wgetch(game_scene);
        switch (input) {
            case KEY_DOWN:
                target->x = minotaur->x;
                target->y = minotaur->y + 1;
                break;
            case KEY_UP:
                target->x = minotaur->x;
                target->y = minotaur->y - 1;
                break;
            case KEY_LEFT:
                target->x = minotaur->x - 1;
                target->y = minotaur->y;
                break;
            case KEY_RIGHT:
                target->x = minotaur->x + 1;
                target->y = minotaur->y;
                break;
            default:
                target->x = minotaur->x;
                target->y = minotaur->y;
        }
        if (isInside(target->x, target->y, size.x, size.y) && map[target->x][target->y] == Empty) {
            minotaur->x = target->x;
            minotaur->y = target->y;
            reveal(fog, minotaur, size.x, size.y);
        }

        /* Exchange information with the server */
        InfoType status;
        recv_status(sockfd, &status, player);
        if (status == PlayerWins) {
            show_info(info_scene, Center, "<Player escaped. What a shame! Press ENTER to continue>");
            break;
        } else if (status == MinotaurWins) {
            show_info(info_scene, Center, "<Nasty human is caught! Rooooar!!! Press ENTER to continue>");
            break;
        }
        send_status(sockfd, Location, minotaur);

        /* Update screen */
        show_game(game_scene, map, fog, &size, player, door, minotaur);
    }

    /* Exit */
    while (TRUE) {
        int input = wgetch(info_scene);
        if (input == 10)
            break;
    }

    /* Free memory */
    free_st(map, size.x);
    free_hid(fog, size.x);
    free(player);
    free(door);
    free(target);
    free(minotaur);
}