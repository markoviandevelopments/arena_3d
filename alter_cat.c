#include "include/player_handling.h"
#include "game.h"

//int counter = 0;

void AlterCat(float *data) {

    float dx;
    float dy;

    if (rand() % 100 == 0) {
        dx = ((rand() % 10 ) / 10.0f - 0.5f) * 0.1f;
        dy = ((rand() % 10 ) / 10.0f - 0.5f) * 0.1f;

        if (Grounds(data[12], data[13] - 0.5f, data[14])) {
        data[12] += dx;
        data[14] += dy;
        }
    }

}
