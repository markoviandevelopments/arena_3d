#include "game.h"

void DrawThing(double server_time) {
    Color squareColor = (Color){255, 255, 0, 200};

    for (int block = 0; block < 10; block++){
        Vector3 position = {
            0.0f + 6.9f * sin(((server_time * 0.00001f)/sin((server_time * 0.00001f)) + block)/ 10.0f * 3.14f * 2),
            0.5f,
            -40.0f + 6.9f * cos(((server_time * 0.00001f)/(sin((server_time * 0.00001f))) + block)/ 10.0f * 3.14f * 2)
        };
        DrawCube(position, 0.6f, 0.55f, 0.6f, squareColor);
        DrawCubeWires(position, 0.6f, 0.55f, 0.6f, BLACK);
    }

    float k = 0.0f; 
    for (int layer = 0; layer < 100; layer++){
        for (int block = 0; block < 100; block++) {
            Vector3 position = {
                0.0f + pow(100 - pow(k, 2), 0.5f) * sin(((server_time * 0.00001f)/sin((server_time * 0.00001f)) + block)/ 100.0f * 3.14f * 2),
                0.5f + layer / 9.0f,
                -40.0f + pow(100 - pow(k, 2), 0.5f) * cos(((server_time * 0.00001f)/(sin((server_time * 0.00001f))) + block)/ 100.0f * 3.14f * 2)
            };
            DrawCube(position, 0.6f, 0.55f, 0.6f, squareColor);
            DrawCubeWires(position, 0.6f, 0.55f, 0.6f, BLACK);
        }
        k += 0.1f;
    }


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
