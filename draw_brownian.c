#include "game.h"

float width;

void DrawBrownian(float *data) {
    Color squareColor = (Color){255, 255, 0, 200};

    Vector3 position = {
        data[0],
        0.8f,
        data[1]
    };

    width = 0.6f + data[2] / 100.0f;

    DrawCube(position, width, 0.55f, width, squareColor);
    DrawCubeWires(position, width, 0.55f, width, BLACK);


    Color square2Color = (Color){100, 100, 255, 200};

    Vector3 position2 = {
        data[3],
        0.8f,
        data[4]
    };

    width = 0.6f + data[5] / 100.0f;


    DrawCube(position2, width, 0.55f, width, square2Color);
    DrawCubeWires(position2, width, 0.55f, width, BLACK);



}
