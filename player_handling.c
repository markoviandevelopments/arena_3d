#include "include/player_handling.h"
#include "game.h"

#include <math.h>

// Update the camera's position and handle rotation
void UpdatePlayerCamera(Camera3D *camera, Player *player, float deltaTime) {
    // Handle camera/player rotation
    if (IsKeyDown(KEY_LEFT)) player->yaw -= TURN_SPEED * deltaTime; // Rotate left
    if (IsKeyDown(KEY_RIGHT)) player->yaw += TURN_SPEED * deltaTime; // Rotate right
    if (IsKeyDown(KEY_UP)) player->pitch -= TURN_SPEED * deltaTime;
    if (IsKeyDown(KEY_DOWN)) player->pitch += TURN_SPEED * deltaTime;

    // Limit pitch to avoid flipping
    if (player->pitch > 89.0f) player->pitch = 89.0f;
    if (player->pitch < -89.0f) player->pitch = -89.0f;

    // Calculate forward vector based on yaw and pitch
    Vector3 forward = {
        cos(DEG2RAD * player->yaw) * cos(DEG2RAD * player->pitch),
        sin(DEG2RAD * player->pitch),
        sin(DEG2RAD * player->yaw) * cos(DEG2RAD * player->pitch)
    };

    // Update camera position and target
    camera->position = (Vector3){
        player->position.x - forward.x * 10.0f, // Move back
        player->position.y + 3.0f,             // Slightly above the player
        player->position.z - forward.z * 10.0f  // Move back
    };

    camera->target = (Vector3){
        player->position.x + forward.x,
        player->position.y + 1.0f, // Look at player's head height
        player->position.z + forward.z
    };
}

// Initialize player properties
void InitializePlayer(Player *player, Vector3 startPosition) {
    player->position = startPosition;
    player->velocityY = 0.0f;
    player->isGrounded = false;
    player->yaw = 0.0f;
    player->pitch = 0.0f;
}

// Handle player movement and input
void HandlePlayerMovement(
    Player *player, float deltaTime, bool *isWalking, bool *isRunning,
    bool *isJumping, Vector3 *moveDirection, int (*Walls)(float, float, float)
) {
    *isWalking = false;
    *isRunning = false;
    *moveDirection = (Vector3){ 0.0f, 0.0f, 0.0f }; // Reset moveDirection

    // Movement input handling
    if (IsKeyDown(KEY_W)) { // Move forward
        moveDirection->x += cos(DEG2RAD * player->yaw) * MOVE_SPEED * deltaTime;
        moveDirection->z += sin(DEG2RAD * player->yaw) * MOVE_SPEED * deltaTime;
        *isWalking = true;
    }
    if (IsKeyDown(KEY_S)) { // Move backward
        moveDirection->x -= cos(DEG2RAD * player->yaw) * MOVE_SPEED * deltaTime;
        moveDirection->z -= sin(DEG2RAD * player->yaw) * MOVE_SPEED * deltaTime;
        *isWalking = true;
    }
    if (IsKeyDown(KEY_E)) { // Sprint
        moveDirection->x += 3.0f * cos(DEG2RAD * player->yaw) * MOVE_SPEED * deltaTime;
        moveDirection->z += 3.0f * sin(DEG2RAD * player->yaw) * MOVE_SPEED * deltaTime;
        *isRunning = true;
        *isWalking = false; // Override walking
    }
    if (IsKeyDown(KEY_A)) { // Strafe left
        moveDirection->x += cos(DEG2RAD * (player->yaw - 90)) * MOVE_SPEED * deltaTime;
        moveDirection->z += sin(DEG2RAD * (player->yaw - 90)) * MOVE_SPEED * deltaTime;
        *isWalking = true;
    }
    if (IsKeyDown(KEY_D)) { // Strafe right
        moveDirection->x += cos(DEG2RAD * (player->yaw + 90)) * MOVE_SPEED * deltaTime;
        moveDirection->z += sin(DEG2RAD * (player->yaw + 90)) * MOVE_SPEED * deltaTime;
        *isWalking = true;
    }

    // Jumping
    if (IsKeyPressed(KEY_SPACE) && player->isGrounded) {
        player->velocityY = 5.0f;
        player->isGrounded = false;
        *isJumping = true;
    } else {
        *isJumping = false;
    }

    // Collision handling using proposed position
    Vector3 proposedPosition = {
        player->position.x + moveDirection->x,
        player->position.y,
        player->position.z + moveDirection->z
    };

    if (!Walls(proposedPosition.x, player->position.y, player->position.z)) {
        player->position.x = proposedPosition.x; // Apply X movement
    } else {
        moveDirection->x = 0.0f; // Stop X movement
    }

    if (!Walls(player->position.x, player->position.y, proposedPosition.z)) {
        player->position.z = proposedPosition.z; // Apply Z movement
    } else {
        moveDirection->z = 0.0f; // Stop Z movement
    }
}

// Apply gravity to the player
void ApplyGravity(Player *player, float deltaTime) {
    if (!player->isGrounded) {
        player->velocityY += GRAVITY * deltaTime;
    }
    player->position.y += player->velocityY * deltaTime;

    // Simulate ground collision
    if (player->position.y <= PLAYER_HEIGHT) {
        player->position.y = PLAYER_HEIGHT;
        player->velocityY = 0.0f;
        player->isGrounded = true;
    } else {
        player->isGrounded = false;
    }
}

// Update the camera's position based on the player's position
void UpdateCameraPosition(Camera3D *camera, const Player *player) {
    Vector3 forward = {
        cos(DEG2RAD * player->yaw),
        0.0f,
        sin(DEG2RAD * player->yaw)
    };

    camera->position = (Vector3){
        player->position.x - forward.x * 10.0f,
        player->position.y + 3.0f,
        player->position.z - forward.z * 10.0f
    };

    camera->target = (Vector3){
        player->position.x + forward.x,
        player->position.y + 1.0f,
        player->position.z + forward.z
    };
}
