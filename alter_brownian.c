#include "game.h"

void AlterBrownian(float *data) {
    float dx;
    float dz;
    dx = ((rand() % 1000) / 1000.0 - 0.5f) * 2.0f * 0.8f;
    dz = ((rand() % 1000) / 1000.0 - 0.5f) * 2.0f * 0.8f;

    if (pow(pow((data[0] + dx) / 50.0f, 2) + pow((data[1] + dz + 40.0f) / 25.0f, 2), 0.5f) <= 1.0f) {
        data[0] += dx * cos(data[6]);
        data[1] += dz * sin(data[6]);
        data[2] += pow(pow(dx, 2) + pow(dz, 2), 0.5f);
    }
    data[2] *= 0.999f;


    dx = ((rand() % 1000) / 1000.0 - 0.5f) * 2.0f * 0.8f;
    dz = ((rand() % 1000) / 1000.0 - 0.5f) * 2.0f * 0.8f;

    if (pow(pow((data[3] + dx) / 50.0f, 2) + pow((data[4] + dz + 40.0f) / 25.0f, 2), 0.5f) <= 1.0f) {
        data[3] += dx * cos(data[7]);
        data[4] += dz * sin(data[7]);
        data[5] += pow(pow(dx, 2) + pow(dz, 2), 0.5f);
    }
    data[5] *= 0.999f;



}
