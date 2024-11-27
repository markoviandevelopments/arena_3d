#include "game.h"


void AlterPrestonparkour(float *data, PlayerShort *playerData) {

    float x;
    float y;
    float z;


    for (int i = 0; i < 2; i++) {
        x = playerData[i].x;
        y = playerData[i].y;
        z = playerData[i].z;

        if (z > data[11] && x > -70.0f && x < 10.0f && y > -1.0f) {
            data[11] = z;
        }

    }


}
