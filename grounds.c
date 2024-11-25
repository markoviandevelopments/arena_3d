#include "game.h"

int Grounds(float x, float y, float z) {
    // Roof level is also a ground level
    if (x > -61.0f && x < -59.0f && z > -57.0f && z < -55.0f && y >= 25.0f && y <= 26.0f) {
        return 1;
    }

    // Regular ground level
    if (y > 20.0f && y < 21.5f) {
        return 1;
    }

    return 0; // Not ground
}
