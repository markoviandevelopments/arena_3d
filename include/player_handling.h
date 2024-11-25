#ifndef PLAYER_HANDLING_H
#define PLAYER_HANDLING_H

#include "raylib.h"
#include "raymath.h"

typedef struct Player {
    Vector3 position;
    float velocityY;
    bool isGrounded;
    float yaw;    // Horizontal angle
    float pitch;  // Vertical angle
} Player;

 typedef struct {
    Vector3 position;
    int id;
} OtherPlayer;

// Function declarations
void InitializePlayer(Player *player, Vector3 startPosition);
void HandlePlayerMovement(
    Player *player, float deltaTime, bool *isWalking, bool *isRunning,
    bool *isJumping, Vector3 *moveDirection, int (*Walls)(float, float, float)
);
void ApplyGravity(Player *player, float deltaTime);
void UpdatePlayerCamera(Camera3D *camera, Player *player, float deltaTime);

#endif // PLAYER_HANDLING_H
