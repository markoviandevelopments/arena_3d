#include "game.h"

float width;

void DrawBrownian(float *data) {
    Color squareColor = (Color){255, 255, 0, 200};
    width = 0.6f + data[2] / 100.0f;

    Vector3 position = {
        data[0],
        0.2f + width / 4.0f,
        data[1]
    };


    DrawCube(position, width, width / 2.0f, width, squareColor);
    DrawCubeWires(position, width, width / 2.0f, width, BLACK);


    Color square2Color = (Color){100, 100, 255, 200};
    width = 0.6f + data[5] / 100.0f;

    Vector3 position2 = {
        data[3],
        0.2f + width / 4.0f,
        data[4]
    };



    DrawCube(position2, width, width / 2.0f, width, square2Color);
    DrawCubeWires(position2, width, width / 2.0f, width, BLACK);



}
