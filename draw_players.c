#include "game.h"
#include "game.h"
#include "raylib.h"

extern Model foxModel;
extern ModelAnimation *animations;
extern int animCount;

void DrawSinglePlayer(Vector3 position, int animIndex, float animFrame, Color color, float yaw) {
    float scale = 0.02f;

    if (animCount > animIndex) {
        UpdateModelAnimation(foxModel, animations[animIndex], (int)animFrame);
    }
    // DrawModel(foxModel, position, scale, color);

    DrawModelEx(
        foxModel,                          // Model to draw
        position,                          // Position to draw
        (Vector3){ 0.0f, 1.0f, 0.0f },     // Rotation axis (Y-axis)
        (-1.0f * yaw + 90.0f),             // Rotation angle (degrees)
        (Vector3){ scale, scale, scale },  // Scaling factor
        color                              // Tint color
    );

    // Debugging visual cues: Draw cubes
    DrawCube(position, 0.15f, 0.3f, 0.15f, color);
    DrawCubeWires(position, 0.15f, 0.3f, 0.15f, BLACK);
}

void DrawPlayers(
    int id1, float x, float y, float z, int animIndex1, float animFrame1, float yaw1, bool isGhostMode,
    float x1, float y1, float z1, int animIndex2, float animFrame2
) {
    Vector3 position1 = { x, y - PLAYER_HEIGHT + 0.2f, z };
    Vector3 position2 = { x1, y1 - PLAYER_HEIGHT + 0.2f, z1 };

    if(!isGhostMode) {
        // Player 1
        DrawSinglePlayer(position1, animIndex1, animFrame1, id1 ? RED : BLUE, yaw1);
        DrawCube(position1, 0.15f, 0.3f, 0.15f, YELLOW);
        DrawCubeWires(position1, 0.15f, 0.3f, 0.15f, BLACK);
    }

    // Player 2
    DrawSinglePlayer(position2, animIndex2, animFrame2, YELLOW, 0.0f);

}
