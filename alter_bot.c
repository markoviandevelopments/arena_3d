#include "game.h"

int counter = 0;

void AlterBot(float *data) {

    if (data[8] == 1.0f && !counter) {
        data[9] += 0.1f;
        counter += 10;
    } else if (data[8] == 2.0f && !counter) {
        data[10] -= 0.1f;
        counter += 10;
    } else if (data[8] == 3.0f && !counter) {
        data[9] -= 0.1f;
        counter += 10;
    } else if (data[8] == 4.0f && !counter) {
        data[10] += 0.1f;
        counter += 10;
    }

    if (counter > 0) {
        counter --;
    }

}
