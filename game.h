#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include <math.h>
#include <stddef.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

// Constants
#define BOARD_SIZE 64
#define SQUARE_SIZE 2.0f
#define PORT 12346
#define BUFFER_SIZE 256
#define PLAYER_HEIGHT 1.6f
#define SERVER_IP "50.188.120.138"

extern Model foxModel;              // Declare the global fox model
extern ModelAnimation *animations;  // Declare global animations
extern int animCount;               // Declare animation count
extern float animFrame;             // Declare animation frame

// Function prototypes
void DrawChessboard(int boardSize, float squareSize);
// void DrawPlayers(int id, float x, float y, float z, float x1, float y1, float z1);
void DrawPlayers(
    int id1, float x, float y, float z, int animIndex1, float animFrame1,
    float x1, float y1, float z1, int animIndex2, float animFrame2
);
void DrawThing(double server_time);
void DrawArena();
int Ladders(float x, float y, float z);
int Grounds(float x, float y, float z);
int Walls(float x, float y, float z);
void DrawPicture();
void AlterBrownian(float *data);
void DrawBrownian(float x, float z);

#endif
