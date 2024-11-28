#include "game.h"

int Walls(float x, float y, float z) {


    // arrowpad
    if (y < 1.5f && ( (x < -11.0f && z < 17.0f && x > -20.0f && z > 14.0f ) || (x < -14.0f && z < 20.0f && x > -17.0f && z > 11.0f) ) ) {
        return 1;
    }

    //mansion
    if ( ( x > 3.0f && x < 97.0f && z > 23.0f && z < 117.0f && y < 185.1f ) || ( x > 70.0f && x < 73.0f && z > 20.0f && z < 23.0f && y < 9.0f ) ) {
        return 0;
    }

    if (x > 0.0f && x < 100.0f && z > 20.0f && z < 120.0f && y < 185.1f) {
        return 1;
    }


    if ( x > 51.0f && x < 69 && z > 31.0f && z < 49.0f && y < 20.0f ) { //secrethouse inner
        return 0;
    }

    if ( y > -1.0f && x > 50.0f && x < 70.0f && z > 30.0f && z < 50.0f && y < 20.0f ) { //secrethouse outer
        return 1;
    }


    if (y > 20.0f || (x < 3.0f && x > -3.0f && z > -30.0f) || pow(pow((x) / 50.0f, 2) + pow((z + 40.0f) / 25.0f, 2), 0.5f) <= 1.0f) {
        return 0;
    }


    if (z < -37.0f && z > -43.0f && x < -55.0f) {
        return 0;
    }


    if (pow(pow((x) / 55.0f, 2) + pow((z + 40.0f) / 30.0f, 2), 0.5f) <= 1.0f ) {
        return 1;
    }

    if (pow(pow((x) / 60.0f, 2) + pow((z + 40.0f) / 35.0f, 2), 0.5f) >= 1.0f && pow(pow((x) / 62.5f, 2) + pow((z + 40.0f) / 37.5f, 2), 0.5f) <= 1.0f) {
        return 1;
    }

    //Wills House DOOR
    if (y < 9.0f && x < -58.0f && x > -62.0f && z >= 70.0f && z <= 75.0f) {
        return 0;
    }

    float wall_t = 1.252f;
    if (y > -0.1f && y < 25.0f && x > -85.0f + wall_t && x < -60.0f - wall_t && z > 60.0f + wall_t && z < 85.0f - wall_t) {
        return 0;
    }

    if (y > -0.1f && y < 25.0f && x >= -85.0f && x < -60.0f && z > 60.0f && z < 85.0f) {
        return 1;
    }

    // Remotehouse

    if (y > -0.1f && y < 25.0f && x >= 1001.0f && x < 1029.0f && z > 971.0f && z <999.0f) {
        return 0;
    }

    if (y > -0.1f && y < 25.0f && x >= 1000.0f && x < 1030.0f && z > 970.0f && z <1000.0f) {
        return 1;
    }



    return 0;
}
