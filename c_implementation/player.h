#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>
#include "utils.h"

typedef struct Postition {
    int x;
    int y;
} position_t;


typedef struct Player {
    position_t position;
    char avatar;
} player_t;


void player_move(player_t *player, int velocity) {
    if (player->position.x + velocity < 5) {
        player->position.x += velocity;
    } else if (player->position.x - velocity >= 0) {
        player->position.x -= velocity;
    } else if (player->position.y + velocity < 5) {
        player->position.y += velocity;
    } else if (player->position.y - velocity >= 0) {
        player->position.y -= velocity;
    } else {
        die("Player is out of bounds (%i, %i)\n", player->position.x, player->position.y);
    }
    printf("Player position: (%i) (%i, %i)\n", velocity, player->position.x, player->position.y);
}

#endif