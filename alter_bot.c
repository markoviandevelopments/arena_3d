#include "game.h"

int counter = 0;

void AlterBot(float *data) {

    float sens = 1.0f

    if (data[8] == 1.0f && !counter) {
        data[9] += sens;
        counter += 10;
    } else if (data[8] == 2.0f && !counter) {
        data[10] -= sens;
        counter += 10;
    } else if (data[8] == 3.0f && !counter) {
        data[9] -= sens;
        counter += 10;
    } else if (data[8] == 4.0f && !counter) {
        data[10] += sens;
        counter += 10;
    }

    if (counter > 0) {
        counter --;
    }

}
