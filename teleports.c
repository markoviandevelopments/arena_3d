#include "include/player_handling.h"
#include "game.h"


void Teleports(Player *player) {

    float x = player->position.x;
    float y = player->position.y;
    float z = player->position.z;

    if (z < -35.0f && z > -46.0f && x < -97.0f && x > -98.0f && y > 0.0f && y < 20.0f) {
        player->position.x = 0.0f;
        player->position.z = 0.0f;
        player->position.y = 5.0f;
        player->yaw = 90.0f;
        player->pitch = 0.0f;
    }

}
