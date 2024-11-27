#include "game.h"

void DrawPrestonparkour() {
    Color squareColor = RED;

    Vector3 position;


    srand(0);

    for (int i=0;i<20;i++) {
        position = (Vector3){
            -20.0f - 4.0f * ( rand() % 10 ),
            -1.5f,
            110.0f + 8.0f * pow(i, 1.01)
        };

        DrawCube(position, 4.0f, 1.0f, 4.0f, squareColor);
        DrawCubeWires(position, 4.0f, 1.0f, 4.0f, BLACK);

    }
}
