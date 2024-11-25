#include "game.h"

void AlterBrownian(float *data) {
    float dx;
    float dz;
    dx = ((rand() % 1000) / 1000.0 - 0.5f) * 2.0f * 0.2f;
    dz = ((rand() % 1000) / 1000.0 - 0.5f) * 2.0f * 0.2f;

    if (pow(pow((data[0] + dx) / 50.0f, 2) + pow((data[1] + dz + 40.0f) / 25.0f, 2), 0.5f) <= 1.0f) {
        data[0] += dx;
        data[1] += dz;
        data[2] += pow(pow(dx, 2) + pow(dz, 2), 0.5f);
    }


    dx = ((rand() % 1000) / 1000.0 - 0.5f) * 2.0f * 0.2f;
    dz = ((rand() % 1000) / 1000.0 - 0.5f) * 2.0f * 0.2f;

    if (pow(pow((data[0] + dx) / 50.0f, 2) + pow((data[1] + dz + 40.0f) / 25.0f, 2), 0.5f) <= 1.0f) {
        data[3] += dx;
        data[4] += dz;
        data[5] += pow(pow(dx, 2) + pow(dz, 2), 0.5f);
    }



}
