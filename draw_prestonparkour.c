#include "game.h"

void DrawPrestonparkour(float *data) {
    Color squareColor = RED;

    Vector3 position;


    srand(0);

    for (int i=0;i<40;i++) {
        position = (Vector3){
            -20.0f - 4.0f * ( rand() % 10 ),
            -1.5f,
            110.0f + 8.0f * pow(i, 1.01)
        };

        DrawCube(position, 4.0f, 1.0f, 4.0f, squareColor);
        DrawCubeWires(position, 4.0f, 1.0f, 4.0f, BLACK);

    }

    squareColor = (Color){0, 0, 255, 150};


    position = (Vector3){
        -20.0f - 4.0f * ( 5 ),
        -1.5f,
        data[11]
    };

    DrawCube(position, 20.0f, 3.0f, 2.0f, squareColor);
    DrawCubeWires(position, 20.0f, 3.0f, 2.0f, BLACK);


}
