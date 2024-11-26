#ifndef PLAYER_HANDLING_H
#define PLAYER_HANDLING_H

#include "raylib.h"
#include "raymath.h"

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

// Function declarations
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

#endif // PLAYER_HANDLING_H
