/* Headers */
#include "gamescene.h"
#include "menu.h"

/* Implementation */

void show_game(WINDOW *game_scene, State **map, Hidden **fog, const Point *size, const Point *player, const Point *door, const Point *minotaur) {
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
    {
        wattron(game_scene, A_BOLD);
        wattron(game_scene, COLOR_PAIR(2));
        mvwaddch(game_scene, player->y, player->x, PLAYER);
        wattroff(game_scene, COLOR_PAIR(2));
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

    /* Show exit */
    if (fog[door->x][door->y] == 1) {
        wattron(game_scene, A_BOLD);
        wattron(game_scene, COLOR_PAIR(4));
        mvwaddch(game_scene, door->y, door->x, EXIT);
        wattroff(game_scene, COLOR_PAIR(4)); 
        wattroff(game_scene, A_BOLD);
    }

    wrefresh(game_scene);
}

void init_game(const int width, const int height) {

    /* Init scene */
    const Point size = {.x = makeodd(width * 0.75), .y = makeodd(height - 4)};
    WINDOW *background_scene = newwin(height, width, 0, 0);
    WINDOW *game_scene = newwin(size.y, size.x, 0, (width - size.x) / 2);
    WINDOW *info_scene = newwin(1, width, height - 1, 0);
    keypad(game_scene, TRUE);
    halfdelay(DELAY);

    /* Create labyrinth */
    State **map = create_labyrinth(size.x, size.y);
    Point *player = rand_position(map, size.x, size.y);
    Point *door = rand_position(map, size.x, size.y);
    Point *minotaur = pointat(makeodd(size.x / 2), makeodd(size.y / 2));
    Hidden **fog = create_hid(size.x, size.y);
    reveal(fog, player, size.x, size.y);

    /* Show scene */
    wrefresh(background_scene);
    show_game(game_scene, map, fog, &size, player, door, minotaur);

    /* Logic */
    Point *target = malloc(sizeof(Point));
    while (TRUE) {

        /* Move player */
        int input = wgetch(game_scene);
        switch (input) {
            case KEY_DOWN:
                write_info(info_scene, "Move down");
                target->x = player->x;
                target->y = player->y + 1;
                break;
            case KEY_UP:
                write_info(info_scene, "Move up");
                target->x = player->x;
                target->y = player->y - 1;
                break;
            case KEY_LEFT:
                write_info(info_scene, "Move left");
                target->x = player->x - 1;
                target->y = player->y;
                break;
            case KEY_RIGHT:
                write_info(info_scene, "Move right");
                target->x = player->x + 1;
                target->y = player->y;
                break;
            default:
                write_info(info_scene, "Idling...");
        }
        if (isInside(target->x, target->y, size.x, size.y) && map[target->x][target->y] == Empty) {
            player->x = target->x;
            player->y = target->y;
            reveal(fog, player, size.x, size.y);
            show_game(game_scene, map, fog, &size, player, door, minotaur);
        }

        /* Move minotaur */
        //::ToDo

        /* Check conditions */
        if (player->x == door->x && player->y == door->y) {
            /* Player won! */
            write_info(info_scene, "<Exit Reached. Congratulations! Press ENTER to continue>");
            break;
        } else if (player->x == minotaur->x && player->y == minotaur->y) {
            /* Player lost! */
            write_info(info_scene, "<You were caught by the minotaur. Press ENTER to continue>");
            break;
        }
    }
    
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