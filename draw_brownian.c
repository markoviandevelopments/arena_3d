#include "game.h"

void DrawBrownian(float x, float z) {
    Color squareColor = (Color){255, 255, 0, 200};

    Vector3 position = {
        x,
        0.8f,
        z
    };

    DrawCube(position, 0.6f, 0.55f, 0.6f, squareColor);
    DrawCubeWires(position, 0.6f, 0.55f, 0.6f, BLACK);

}
