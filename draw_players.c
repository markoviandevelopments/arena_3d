#include "game.h"
#include "raylib.h"

extern Model foxModel;
extern ModelAnimation *animations;
extern int animCount;

void DrawPlayers(
    int id1, float x, float y, float z, int animIndex1, float animFrame1,
    float x1, float y1, float z1, int animIndex2, float animFrame2
) {
    float scale = 0.02f;

    // Player 1
    Vector3 position1 = { x, y - PLAYER_HEIGHT + 0.2f, z };
    if (animCount > animIndex1) {
        UpdateModelAnimation(foxModel, animations[animIndex1], (int)animFrame1);
    }
    DrawModel(foxModel, position1, scale, id1 ? RED : BLUE);



    Color squareColor = RED;

    Vector3 position = {
        x,
        y,
        z
    };
    DrawCube(position, 0.1f, 0.1f, 0.1f, squareColor);
    DrawCubeWires(position, 0.1f, 0.1f, 0.1f, BLACK);



    // Player 2
    Vector3 position2 = { x1, y1 - PLAYER_HEIGHT + 0.2f, z1 };
    if (animCount > animIndex2) {
        UpdateModelAnimation(foxModel, animations[animIndex2], (int)animFrame2);
    }
    DrawModel(foxModel, position2, scale, YELLOW);
}
