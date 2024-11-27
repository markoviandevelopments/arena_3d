#include "game.h"

void DrawPrestonparkour() {
    Color squareColor = RED;

    Vector3 position = {
        -10.0f - 0.5f,
        -1.0f,
        101.0f - 0.5f
    };

    DrawCube(position, 1.0f, 1.0f, 1.0f, squareColor);
    DrawCubeWires(position, 1.0f, 1.0f, 1.0f, BLACK);
}
