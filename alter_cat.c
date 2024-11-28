#include "game.h"



void AlterCat(float *data, float *cat_dx, float *cat_dz, float *cat_ddx, float *cat_ddz, int *cat_counter) {

    for (int i=0; i < 200; i++) {
        srand(rand() % 100000 + *cat_counter);
    }



    //srand((*cat_counter) * 12345);
    *cat_ddx += sin(((float)rand() / RAND_MAX - 0.5f) * 1000.0f) * 0.00002f;
    *cat_ddz += sin(((float)rand() / RAND_MAX - 0.5f) * 1000.0f) * 0.00002f;
    (*cat_counter)++;

    if (1) {
        *cat_dx += *cat_ddx;
        *cat_dz += *cat_ddz;

        *cat_dx *= 0.99f;
        *cat_dz *= 0.99f;

        if (Grounds(data[12] + *cat_dx, data[13] - 0.5f, data[14] + *cat_dz) && !Walls(data[12] + *cat_dx, data[13], data[14] + *cat_dz)) {
            data[12] += *cat_dx;
            data[14] += *cat_dz;
        }
        if(Walls(data[12] + *cat_dx, data[13], data[14]) || !Grounds(data[12] + *cat_dx, data[13] - 0.5f, data[14])) {
            *cat_dx = 0.0f;
            *cat_ddx = 0.0f;
        }
        if(Walls(data[12], data[13], data[14] + *cat_dz) || !Grounds(data[12], data[13] - 0.5f, data[14] + *cat_dz)) {
            *cat_dz = 0.0f;
            *cat_ddz = 0.0f;
        }

    }

}
