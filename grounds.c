#include "game.h"

int Grounds(float x, float y, float z) {
    // top of ladder
    if (x > -61.0f && x < -59.0f && z > -57.0f && z < -55.0f && y >= 25.0f && y <= 26.0f) {
        return 1;
    }

    // castle top
    if (y > 20.0f && y < 21.5f && x > -101.0f && x < 99.0f && z > -101.0f && z < 99.0f) {
        return 1;
    }

    // chessboard ground
    if (y < PLAYER_HEIGHT  && y < 21.5f && x > -101.0f && x < 99.0f && z > -101.0f && z < 99.0f) {
        return 1;
    }

    // arrowpad ground
    if (y < 1.5f && ( (x < -11.0f && z < 17.0f && x > -20.0f && z > 14.0f ) || (x < -14.0f && z < 20.0f && x > -17.0f && z > 11.0f) ) ) {
        return 1;
    }

    return 0; // Not ground
}
