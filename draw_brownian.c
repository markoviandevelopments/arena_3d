#include "game.h"

void DrawBrownian(float *data) {
    Color squareColor = (Color){255, 255, 0, 200};

    Vector3 position = {
        data[0],
        0.8f,
        data[1]
    };

    DrawCube(position, 0.6f, 0.55f, 0.6f, squareColor);
    DrawCubeWires(position, 0.6f, 0.55f, 0.6f, BLACK);


    Color square2Color = (Color){100, 100, 255, 200};

    Vector3 position2 = {
        data[3],
        0.8f,
        data[4]
    };

    DrawCube(position2, 0.6f, 0.55f, 0.6f, square2Color);
    DrawCubeWires(position2, 0.6f, 0.55f, 0.6f, BLACK);



}
