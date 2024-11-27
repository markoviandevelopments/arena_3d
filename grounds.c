#include "game.h"

int Grounds(float x, float y, float z) {

    float x_m;
    float z_m;
    float y_m;

    srand(42);

    // Preston Parkour :)
    for (int i=0; i<180;i++) {
        x_m = -20.0f - 4.0f * (( rand() % 10 ) / 10.0f - 0.5f) * 2.0f * 2.0f *  pow(2, i / 60.0f);
        z_m = 110.0f + 5.0f * pow(i, 1.010f);
        if (x > x_m - 2.0f && x < x_m + 2.0f && z >z_m - 2.0f && z < z_m + 2.0f && y > -1.0f && y < 0.0f) {
            return 1;
        }
    }

    srand(42);
    float ang_off;
    float scale;

    for (int i=0; i<380;i++) {
        scale = 1.0f + i / 270.0f;
        ang_off = (rand() % 1000) / 1000.0f / 10.0f * scale;
        x_m = -80.0f + 30.0f * cos(i / 5.0f * scale - 1.28f + ang_off);
        y_m = -1.5f + 0.2f * i * scale;
        z_m = 130.0f + 30.0f * sin(i / 5.0f * scale -1.28f + ang_off);
        if (x > x_m - 2.0f && x < x_m + 2.0f && z >z_m - 2.0f && z < z_m + 2.0f && y > y_m - 0.5f + PLAYER_HEIGHT && y < y_m + 0.5f + PLAYER_HEIGHT) {
            return 1;
        }
    }



    // top of ladder
    if (x > -61.0f && x < -59.0f && z > -57.0f && z < -55.0f && y >= 25.0f && y <= 26.0f) {
        return 1;
    }

    // castle top
    if (y > 20.0f && y < 21.5f && x > -101.0f && x < 99.0f && z > -101.0f && z < 99.0f && pow(pow((x) / 60.0f, 2) + pow((z + 40.0f) / 60.0f, 2), 0.5f) <= 1.0f) {
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

    //mansion ground
    for (int i=0; i < 9; i++) {
        if (y < PLAYER_HEIGHT + 0.1f + i * 23.0f && y > PLAYER_HEIGHT + 0.1f + i * 23.0f - 1.0f && z > 20.0f && x > 0.0f && z < 120.0f && x < 100.0f) {
            return 1;
        }
    }

    // top of ladder in mansion
    if (x > 72.0f && x < 74.0f && z > 113.0f && z < 115.0f && y >= 200.0f && y <= 201.0f) {
        return 1;
    }





    return 0; // Not ground
}
