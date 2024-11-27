#include "game.h"

void DrawPrestonparkour(float *data) {
    Color squareColor = (Color){43, 255, 0, 200};

    Vector3 position;


    srand(42);

    for (int i=0;i<180;i++) {
        position = (Vector3){
            -20.0f - 4.0f * (( rand() % 10 ) / 10.0f - 0.5f) * 2.0f * 2.0f * pow(2, i / 60.0f) ,
            -1.5f,
            110.0f + 5.0f * pow(i, 1.010f)
        };

        DrawCube(position, 4.0f, 1.0f, 4.0f, squareColor);
        DrawCubeWires(position, 4.0f, 1.0f, 4.0f, BLACK);

    }

    squareColor = (Color){255, 255, 0, 150};


    position = (Vector3){
        -20.0f,
        -3.1f,
        data[11]
    };

    DrawCube(position, 20.0f, 3.0f, 2.0f, squareColor);
    DrawCubeWires(position, 20.0f, 3.0f, 2.0f, BLACK);


}
