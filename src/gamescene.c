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
int terminate(WINDOW *info_scene, InfoType status, Affiliation side) {
    if (status == HumanWins) {
        switch(side) {
        case Human:
            show_info(info_scene, Center, "<Congratulations! You escaped! Press ENTER to continue>");
            break;
        case Minotaur:
            show_info(info_scene, Center, "<Human escaped. What a shame! Press ENTER to continue>");
            break;
        }
        return TRUE;
    } else if (status == MinotaurWins) {
        switch(side) {
        case Human:
            show_info(info_scene, Center, "<We're caught. Now we'll spend the rest of life here... Press ENTER to continue>");
            break;
        case Minotaur:
            show_info(info_scene, Center, "<Nasty human is caught! Rooooar!!! Press ENTER to continue>");
            break;
        }
        return TRUE;
    } else {
        return FALSE;
    }
}

void setup_game_scene(WINDOW *game_scene, const Point *size) {
    wattron(game_scene, COLOR_PAIR(1));
    for (unsigned y = 0; y < size->y; y++) {
        wmove(game_scene, y, 0);
        for (unsigned x = 0; x < size->x; x++) {
            waddch(game_scene, UNKNOWN);
        }
    }
    wattroff(game_scene, COLOR_PAIR(1));
}

void show_game(WINDOW *game_scene, const State **map, const Hidden **fog, const Point *size, const Point *human, const Point *door, const Point *minotaur) {
    
    /* Reveal current location */
    const static Point directions[] = { {.x = 0, .y = 1},
                                        {.x = 1, .y = 1},
                                        {.x = 1, .y = 0},
                                        {.x = 1, .y = -1},
                                        {.x = 0, .y = -1},
                                        {.x = -1, .y = -1},
                                        {.x = -1, .y = 0},
                                        {.x = -1, .y = 1} };

    /* Reveal current */
    for(unsigned i = 0; i < 8; i++) {
        unsigned x = human->x + directions[i].x;
        unsigned y = human->y + directions[i].y;
        if (fog[x][y] == 0)
            continue;
        switch(map[x][y]) {
        case Empty:
            mvwaddch(game_scene, y, x, EMPTY);
            break;
        case Wall:
            wattron(game_scene, COLOR_PAIR(3));
            mvwaddch(game_scene, y, x, WALL);
            wattroff(game_scene, COLOR_PAIR(3));
            break;
        default:
            exit(1);
        }
    }

    if (minotaur != NULL)
        for(unsigned i = 0; i < 8; i++) {
            unsigned x = minotaur->x + directions[i].x;
            unsigned y = minotaur->y + directions[i].y;
            if (fog[x][y] == 0)
                continue;
            switch(map[x][y]) {
            case Empty:
                mvwaddch(game_scene, y, x, EMPTY);
                break;
            case Wall:
                wattron(game_scene, COLOR_PAIR(3));
                mvwaddch(game_scene, y, x, WALL);
                wattroff(game_scene, COLOR_PAIR(3));
                break;
            default:
                exit(1);
            }
        }

    /* Show human */
    if (fog[human->x][human->y] == 1) {
        wattron(game_scene, A_BOLD);
        wattron(game_scene, COLOR_PAIR(2));
        mvwaddch(game_scene, human->y, human->x, HUMAN);
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

void init_server(const int width, const int height, const GameMode mode, const Affiliation side, USock sockfd) {

    /* Init scene */
    const Point size = {.x = min2(makeodd(width * 0.75), makeodd(MIN_WIDTH - 1)), .y = min2(makeodd(height - 4), makeodd(MIN_HEIGHT - 1))};
    WINDOW *background_scene = newwin(height, width, 0, 0);
    WINDOW *game_scene = newwin(size.y, size.x, 1, (width - size.x) / 2);
    WINDOW *info_scene = newwin(1, width, height - 1, 0);
    keypad(game_scene, TRUE);
    halfdelay(DELAY);

    /* Create labyrinth */
    InfoType status = InProgress;
    State **map = create_labyrinth(size.x, size.y);
    Point *human = rand_position(map, size.x, size.y);
    Point *door = rand_position(map, size.x, size.y);
    Point *minotaur = NULL;
    Hidden **fog = create_hid(size.x, size.y);
    if (mode == Hotseat || mode == Multiplayer)
        minotaur = pointat(makeodd(size.x / 2), makeodd(size.y / 2));
    if (mode == Multiplayer) {
        send_point(sockfd, &size);
        send_initial_info(sockfd, size, map, human, door, minotaur);
    }

    /* Take control */
    Point *client, *server;
    switch(side) {
    case Human:
        server = human;
        client = minotaur;
        break;
    case Minotaur:
        server = minotaur;
        client = human;
        break;
    }

    /* Show scene */
    reveal(fog, server, size.x, size.y);
    if (mode == Hotseat) {
        reveal(fog, client, size.x, size.y);
    }
    wrefresh(background_scene);
    setup_game_scene(game_scene, &size);
    show_game(game_scene, map, fog, &size, human, door, minotaur);

    /* Logic */
    Point *target = malloc(sizeof(Point));
    while (TRUE) {

        /* All Modes */
        int input = wgetch(game_scene);
        switch (input) {
            case KEY_DOWN:
                target->x = server->x;
                target->y = server->y + 1;
                break;
            case KEY_UP:
                target->x = server->x;
                target->y = server->y - 1;
                break;
            case KEY_LEFT:
                target->x = server->x - 1;
                target->y = server->y;
                break;
            case KEY_RIGHT:
                target->x = server->x + 1;
                target->y = server->y;
                break;
            case 27:
                switch(side) {
                case Human:
                    status = MinotaurWins;
                    break;
                case Minotaur:
                    status = HumanWins;
                    break;
                }
            default:
                target->x = server->x;
                target->y = server->y;
        }
        if (isInside(target->x, target->y, size.x, size.y) && map[target->x][target->y] == Empty) { //::TODO Delete isInside()?
            server->x = target->x;
            server->y = target->y;
            reveal(fog, server, size.x, size.y);
            show_game(game_scene, map, fog, &size, human, door, minotaur);
        }

        /* Hotseat */
        if (mode == Hotseat) {
            switch (input) {
                case 'S': case 's':
                   target->x = client->x;
                   target->y = client->y + 1;
                   break;
                case 'W': case 'w':
                    target->x = client->x;
                    target->y = client->y - 1;
                    break;
                case 'A': case 'a':
                    target->x = client->x - 1;
                    target->y = client->y;
                    break;
                case 'D': case 'd':
                    target->x = client->x + 1;
                    target->y = client->y;
                    break;
                default:
                    target->x = client->x;
                    target->y = client->y;
            }
            if (isInside(target->x, target->y, size.x, size.y) && map[target->x][target->y] == Empty) {  //::TODO Delete isInside()?
                client->x = target->x;
                client->y = target->y;
                reveal(fog, minotaur, size.x, size.y);
                show_game(game_scene, map, fog, &size, human, door, minotaur);
            }
        }

        /* Multiplayer Mode */
        if (mode == Multiplayer) {
            /* Exchange status */
            InfoType client_status;
            send_status(sockfd, Location, server);
            recv_status(sockfd, &client_status, client);

            /* Terminate */
            if (terminate(info_scene, client_status, side) == TRUE) {
                break;
            }

            /* Update screen */
            show_game(game_scene, map, fog, &size, human, door, minotaur);
        }

        /* Check termination conditions */
        if (human->x == door->x && human->y == door->y) {
            /* Human won */
            status = HumanWins;
            if (mode == Multiplayer) {
                send_status(sockfd, status, NULL); //Send the message to the client
            }
        } else if (minotaur && human->x == minotaur->x && human->y == minotaur->y) {
            /* Minotaur won */
            status = MinotaurWins;
            if (mode == Multiplayer) {
                send_status(sockfd, status, NULL); //Send the message to the client
            }
        }

        /* Terminate */
        if (terminate(info_scene, status, side) == TRUE) {
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
    free(human);
    free(door);
    free(target);
    free(minotaur);
    nocbreak();
    cbreak();
}

void init_client(const int width, const int height, const Affiliation side, const USock sockfd) {
    /* Receive info */
    Point size;
    recv_point(sockfd, &size);
    State **map = create_st(size.x, size.y);
    Point *human = malloc(sizeof(Point));
    Point *door = malloc(sizeof(Point));
    Point *minotaur = malloc(sizeof(Point));
    recv_initial_info(sockfd, size, map, human, door, minotaur);

    /* Take control */
    Point *client, *server;
    switch(side) {
    case Human:
        client = human;
        server = minotaur;
        break;
    case Minotaur:
        client = minotaur;
        server = human;
        break;
    }

    /* Init scene */
    WINDOW *background_scene = newwin(height, width, 0, 0);
    WINDOW *game_scene = newwin(size.y, size.x, 0, (width - size.x) / 2);
    WINDOW *info_scene = newwin(1, width, height - 1, 0);
    keypad(game_scene, TRUE);
    halfdelay(DELAY);

    /* Create labyrinth */
    InfoType status = InProgress;
    Hidden **fog = create_hid(size.x, size.y);
    reveal(fog, client, size.x, size.y);

    /* Show scene */
    wrefresh(background_scene);
    setup_game_scene(game_scene, &size);
    show_game(game_scene, map, fog, &size, human, door, minotaur);

    /* Logic */
    Point *target = malloc(sizeof(Point));
    while (TRUE) {
        int input = wgetch(game_scene);
        switch (input) {
            case KEY_DOWN:
                target->x = client->x;
                target->y = client->y + 1;
                break;
            case KEY_UP:
                target->x = client->x;
                target->y = client->y - 1;
                break;
            case KEY_LEFT:
                target->x = client->x - 1;
                target->y = client->y;
                break;
            case KEY_RIGHT:
                target->x = client->x + 1;
                target->y = client->y;
                break;
            case 27:
                switch(side) {
                case Human:
                    status = MinotaurWins;
                    break;
                case Minotaur:
                    status = HumanWins;
                    break;
                }
            default:
                target->x = client->x;
                target->y = client->y;
        }
        if (isInside(target->x, target->y, size.x, size.y) && map[target->x][target->y] == Empty) {
            client->x = target->x;
            client->y = target->y;
            reveal(fog, client, size.x, size.y);
        }

        /* Exchange information with the server */
        InfoType server_status;
        recv_status(sockfd, &server_status, server);
        if (terminate(info_scene, server_status, side) == TRUE) {
            break;
        }
        if (terminate(info_scene, status, side) == TRUE) {
            send_status(sockfd, status, client);
            break;
        }
        send_status(sockfd, Location, client);

        /* Update screen */
        show_game(game_scene, map, fog, &size, human, door, minotaur);
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
    free(human);
    free(door);
    free(target);
    free(minotaur);
    nocbreak();
    cbreak();
}