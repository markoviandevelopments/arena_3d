#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include <math.h>
#include <stddef.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "raymath.h"

// Constants
#define MOVE_SPEED 5.0f     // Movement speed
#define TURN_SPEED 170.0f   // Turn speed in degrees per second
#define GRAVITY -9.8f       // Gravity
#define PLAYER_HEIGHT 1.0f  // Player height for grounding
#define BOARD_SIZE 100
#define SQUARE_SIZE 2.0f
#define PORT 12346
#define BUFFER_SIZE 256
#define SERVER_IP "50.188.120.138"
#define NO_ROTATION 0.0f
#define MAZE_SIZE 21
#define SPACING 2.0f
#define X_OFFSET -95
#define Z_OFFSET -10

extern Model foxModel;              // Declare the global fox model
extern ModelAnimation *animations;  // Declare global animations
extern int animCount;               // Declare animation count
extern float animFrame;             // Declare animation frame

extern int maze[MAZE_SIZE][MAZE_SIZE];

// Player structure
typedef struct {
    int id;
    float x, y, z;
} PlayerShort;

typedef struct Player {
    Vector3 position;
    Vector3 moveDirection;
    float velocityY;
    bool isGrounded;
    bool isOnLadder;
    float yaw;
    float pitch;
} Player;

 typedef struct {
    Vector3 position;
    int id;
} OtherPlayer;

// Function prototypes
void DrawChessboard(int boardSize, float squareSize);
void InitializePlayer(Player *player, Vector3 startPosition);
void HandlePlayerMovement(
    Player *player, float deltaTime, bool *isWalking, bool *isRunning,
    bool *isJumping, int (*Walls)(float, float, float),
    int (*Ladders)(float, float, float),
    int *sensitivity,
    int *sensitivity_r,
    float spawn_x,
    float spawn_z
);
void ApplyGravity(Player *player, float deltaTime);
void UpdatePlayerCamera(Camera3D *camera, Player *player, float deltaTime, int *sensitivity_r);
void Teleports(Player *player);
void DrawPlayers(
    int id1, float x, float y, float z, int animIndex1, float animFrame1, float yaw1, bool isGhostMode,
    float x1, float y1, float z1, int animIndex2, float animFrame2
);
void DrawThing(double server_time);
void DrawArena();
void DrawMansion();
void DrawRemotehouse();
void DrawPrestonparkour(float *data, double server_time);
void GenerateMaze(int maze[MAZE_SIZE][MAZE_SIZE]);
void DrawMaze(int maze[MAZE_SIZE][MAZE_SIZE]);
void DrawSecrethouse();
void DrawAHouse();
void DrawArrowpad(float *data);
void DrawBot(float *data);
int Ladders(float x, float y, float z);
int Grounds(float x, float y, float z);
int Walls(float x, float y, float z);
bool IsPlayerInMaze(float x, float y, float z);
int MazeCollision(float x, float y, float z, int maze[MAZE_SIZE][MAZE_SIZE]);
void DrawPicture();
void AlterBrownian(float *data);
void AlterArrowpad(float *data, PlayerShort *playerData);
void AlterBot(float *data);
void AlterCat(float *data, float *cat_dx, float *cat_dz, float *cat_ddx, float *cat_ddz, int *cat_counter);
void AlterPrestonparkour(float *data, PlayerShort *playerData);
void DrawBrownian(float *data);
void DrawCat(float *data);

#endif
