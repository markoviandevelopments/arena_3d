#include "game.h"

int Grounds(float x, float y, float z) {
    if (x > -61.0f && x < -59.0f && z > -57.0f && z < -55.0f && y < 25.0f) {
        return 0;
    }

    if (y > 20.0f && y < 21.5f) {
        return 1;
    }

    return 0;
}
