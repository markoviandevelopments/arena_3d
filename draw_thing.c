#include "game.h"

void DrawThing(double server_time) {
    Color squareColor = (Color){255, 0, 255, 200};

    Vector3 position = {
        0.0f + 10.3f * sin(2.31f * server_time / 1000.0) + 10.1f * cos(0.4f * server_time / 1000.0),
        0.5f,
        -40.0f + 10.4f * cos(server_time / 1000.0)
         };

    DrawCube(position, 0.6f, 0.55f, 0.6f, squareColor);
    DrawCubeWires(position, 0.6f, 0.55f, 0.6f, BLACK);




    Color square2Color = (Color){25, 255, 255, 180};

    float y = 20.0 * sin(0.0001 * server_time) + 20.0 * sin(0.00014 * server_time) + 20.0 * sin(0.00042 * server_time);
    if (y > 2.0f) {
        y -= 2.0f;
    } else {
        y = 0.0f;
    }


    Vector3 position2= {
        -0.0f,
        0.3f + y,
        -42.0f
         };

    DrawCube(position2, 0.3f, 0.15f, 0.3f, square2Color);
    DrawCubeWires(position2, 0.3f, 0.15f, 0.3f, BLACK);




}
