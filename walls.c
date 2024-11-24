#include "game.h"

int Walls(float x, float y, float z) {

    if (y > 20.0f || (x < 3.0f && x > -3.0f && z > -30.0f) || pow(pow((x) / 50.0f, 2) + pow((z + 40.0f) / 25.0f, 2), 0.5f) <= 1.0f) {
        return 0;
    }

    if (z < -37.0f && z > -43.0f && x < -55.0f) {
        return 0;
    }


    if (pow(pow((x) / 55.0f, 2) + pow((z + 40.0f) / 30.0f, 2), 0.5f) <= 1.0f ) {
        return 1;
    }

    if ( x > 51.0f && x < 69 && z > 31.0f && z < 49.0f ) { //secrethouse inner
        return 0;
    }

    if ( y > -1.0f && x > 50.0f && x < 70.0f && z > 30.0f && z < 50.0f ) { //secrethouse outer
        return 1;
    }

    if (pow(pow((x) / 60.0f, 2) + pow((z + 40.0f) / 35.0f, 2), 0.5f) >= 1.0f && pow(pow((x) / 62.5f, 2) + pow((z + 40.0f) / 37.5f, 2), 0.5f) <= 1.0f) {
        return 1;
    }

    return 0;
}
