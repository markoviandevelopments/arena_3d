#include "include/player_handling.h"
#include "game.h"


void Teleports(Player *player) {

    float x = player->position.x;
    float y = player->position.y;
    float z = player->position.z;

    if (z < -35.0f && z > -46.0f && x < -97.0f && x > -98.0f && y > 0.0f && y < 20.0f) {
        player->position.x = 1020.0f;
        player->position.z = 986.0f;
        player->position.y = 1.1f;
        player->velocityY = 0.0f;
        player->yaw = 263.0f;
        player->pitch = 59.0f;
    }

    if (z < 972.0f && z > 971.5f && x < 1023.5f && x > 1008.0f && y > 0.0f && y < 20.0f) {
        player->position.x = -83.1f;
        player->position.z = -39.8f;
        player->position.y = 1.1f;
        player->velocityY = 0.0f;
        player->yaw = 181.4f;
        player->pitch = 64.65f;
    }


}
