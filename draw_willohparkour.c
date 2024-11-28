#include "game.h"
#include "raylib.h"
#include <stdlib.h>
#include <time.h>
#include <time.h>

static inline Color GenerateWallColor(){
    return (Color){ rand() % 256, rand() % 256, rand() % 256, 255 };
}

void shuffle(int directions[4][2]) {
    for (int i = 0; i < 4; ++i) {
        int j = rand() % 4;
        int temp[2] = {directions[i][0], directions[i][1]};
        directions[i][0] = directions[j][0];
        directions[i][1] = directions[j][1];
        directions[j][0] = temp[0];
        directions[j][1] = temp[1];
    }
}

void carve_passage(int x, int y, int maze[MAZE_SIZE][MAZE_SIZE]) {
    int directions[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    shuffle(directions);

    for (int i = 0; i < 4; ++i) {
        int dx = directions[i][0];
        int dy = directions[i][1];
        int nx = x + 2 * dx;
        int ny = y + 2 * dy;

        if (nx > 0 && ny > 0 && nx < MAZE_SIZE - 1 && ny < MAZE_SIZE - 1 && maze[ny][nx] == 1) {
            maze[ny - dy][nx - dx] = 0;
            maze[ny][nx] = 0;
            carve_passage(nx, ny, maze);
        }
    }
}

void GenerateMaze(int maze[MAZE_SIZE][MAZE_SIZE]) {
    // Initialize the maze with all walls
    for (int y = 0; y < MAZE_SIZE; ++y) {
        for (int x = 0; x < MAZE_SIZE; ++x) {
            maze[y][x] = 1;
        }
    }

    srand(69);

    // Set the starting point within the maze to carve passages
    int start_x = (rand() % ((MAZE_SIZE - 1) / 2)) * 2 + 1;
    int start_y = (rand() % ((MAZE_SIZE - 1) / 2)) * 2 + 1;
    maze[start_y][start_x] = 0;

    // Ensure the maze has an entrance (e.g., top-left corner)
    maze[1][0] = 0;  // Entrance on the top row

    carve_passage(start_x, start_y, maze);
}

void DrawMaze(int maze[MAZE_SIZE][MAZE_SIZE]) {
    Color wallColor = GenerateWallColor();
    for (int y = 0; y < MAZE_SIZE; ++y) {
        for (int x = 0; x < MAZE_SIZE; ++x) {
            if (maze[y][x] == 1){
                DrawCube((Vector3){x * SPACING + X_OFFSET, 1.0f, y * SPACING + Z_OFFSET}, SPACING, 1.5f, SPACING, wallColor);
                DrawCubeWires((Vector3){x * SPACING + X_OFFSET, 1.0f, y * SPACING + Z_OFFSET}, SPACING, 1.5f, SPACING, BLACK);
            }
        }
    }
}