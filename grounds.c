#include "game.h"

int Grounds(float x, float y, float z) {
    // Roof level is also a ground level
    if (x > -61.0f && x < -59.0f && z > -57.0f && z < -55.0f && y >= 25.0f && y <= 26.0f) {
        return 1;
    }

    // Regular ground level
    if (y > 20.0f && y < 21.5f && x > -101.0f && x < 99.0f && z > -101.0f && z < 99.0f) {
        return 1;
    }

    if (y < PLAYER_HEIGHT  && y < 21.5f && x > -101.0f && x < 99.0f && z > -101.0f && z < 99.0f) {
        return 1;
    }

    return 0; // Not ground
}
