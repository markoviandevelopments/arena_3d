#include "game.h"

void DrawPrestonparkour() {
    Color squareColor = RED;

    Vector3 position = {
        -10.0f - 2.0f,
        -1.0f,
        102.0f - 2.0f
    };

    DrawCube(position, 4.0f, 1.0f, 4.0f, squareColor);
    DrawCubeWires(position, 4.0f, 1.0f, 4.0f, BLACK);
}
