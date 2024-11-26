#include "game.h"

float x;
float y;
float z;

void AlterArrowpad(float *data, PlayerShort *playerData) {

    int arrow = 0;

    for (int i = 0; i < 2; i++) {
        x = playerData[i].x;
        y = playerData[i].y;
        z = playerData[i].z;

        //printf("%f %f %f\n", playerData[i].x, playerData[i].y, playerData[i].z);


        if (x > -14.0f && z > 14.0f && x < -11.0f && z < 17.0f && y < 2.0f && y > 0.0f) {
            arrow = 1;
        } else if (x > -17.0f && z > 11.0f && x < -14.0f && z < 14.0f && y < 2.0f && y > 0.0f) {
            arrow = 2;
        } else if (x > -20.0f && z > 14.0f && x < -17.0f && z < 17.0f && y < 2.0f && y > 0.0f) {
            arrow = 3;
        } else if (x > -17.0f && z > 17.0f && x < -14.0f && z < 20.0f && y < 2.0f && y > 0.0f) {
            arrow = 4;
        }



    }


    data[8] = (float)arrow;

}
