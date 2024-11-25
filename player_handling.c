#include "include/player_handling.h"
#include "game.h"

#include <math.h>

// Update the camera's position and handle rotation
void UpdatePlayerCamera(Camera3D *camera, Player *player, float deltaTime) {
    // Handle camera/player rotation
    if (IsKeyDown(KEY_LEFT)) player->yaw -= TURN_SPEED * deltaTime;   // Rotate left
    if (IsKeyDown(KEY_RIGHT)) player->yaw += TURN_SPEED * deltaTime;  // Rotate right
    if (IsKeyDown(KEY_DOWN)) player->pitch -= TURN_SPEED * deltaTime;   // Look up
    if (IsKeyDown(KEY_UP)) player->pitch += TURN_SPEED * deltaTime; // Look down

    // Clamp the pitch to avoid flipping
    if (player->pitch > 89.0f) player->pitch = 89.0f;
    if (player->pitch < -89.0f) player->pitch = -89.0f;

    // Define third-person parameters
    const float THIRD_PERSON_DISTANCE = 10.0f; // Distance behind the player
    const float THIRD_PERSON_HEIGHT = 3.0f;    // Height above the player

    // Calculate the forward vector based on yaw and pitch
    Vector3 forward = {
        cos(DEG2RAD * player->yaw) * cos(DEG2RAD * player->pitch),
        sin(DEG2RAD * player->pitch),
        sin(DEG2RAD * player->yaw) * cos(DEG2RAD * player->pitch)
    };

    // Smoothly transition to first-person view when looking up
    float pitchFactor = fabs(player->pitch) / 89.0f; // Scale between 0.0 and 1.0 based on pitch
    float distanceFactor = 1.0f - pitchFactor;       // Inverse for smooth zoom-in effect

    // Calculate the hybrid camera position
    camera->position = (Vector3){
        player->position.x - forward.x * THIRD_PERSON_DISTANCE * distanceFactor,
        player->position.y + THIRD_PERSON_HEIGHT * distanceFactor + 1.0f * pitchFactor, // Transition height
        player->position.z - forward.z * THIRD_PERSON_DISTANCE * distanceFactor
    };

    // Update the camera's target based on pitch and yaw
    camera->target = (Vector3){
        player->position.x + forward.x,
        player->position.y + forward.y + 1.0f, // Head height adjustment
        player->position.z + forward.z
    };

    // Ensure the camera's up vector is stable
    camera->up = (Vector3){ 0.0f, 1.0f, 0.0f };
}

// Initialize player properties
void InitializePlayer(Player *player, Vector3 startPosition) {
    player->position = startPosition;
    player->velocityY = 0.0f;
    player->isGrounded = false;
    player->yaw = 90.0f;
    player->pitch = 0.0f;
}

// Handle player movement and input
void HandlePlayerMovement(
    Player *player, float deltaTime, bool *isWalking, bool *isRunning,
    bool *isJumping, Vector3 *moveDirection, int (*Walls)(float, float, float),
    int (*Ladders)(float, float, float) // Pass the ladder check function
) {
    *isWalking = false;
    *isRunning = false;
    *moveDirection = (Vector3){ 0.0f, 0.0f, 0.0f }; // Reset moveDirection

    player->isOnLadder = Ladders(player->position.x, player->position.y, player->position.z);

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

    // Smooth ladder climbing
    if (player->isOnLadder) {
        if (IsKeyDown(KEY_SPACE)) { // Move up the ladder smoothly
            player->position.y += MOVE_SPEED * deltaTime * 2.0f; // Adjust the speed for a smooth climb
            player->velocityY = 0.0f; // Disable gravity while climbing
        }
        if (IsKeyDown(KEY_S)) { // Allow moving down the ladder
            player->position.y -= MOVE_SPEED * deltaTime * 2.0f; // Move down smoothly
            player->velocityY = 0.0f; // Disable gravity while descending
        }
        player->isGrounded = false; // Disable grounded state while on a ladder
    }

    // Jumping (only if not on a ladder)
    if (IsKeyPressed(KEY_SPACE) && (player->isGrounded || Grounds(player->position.x, player->position.y - 0.1f, player->position.z) ) && !player->isOnLadder) {
        player->velocityY = 5.0f;
        player->isGrounded = false;
        *isJumping = true;
    } else {
        *isJumping = false;
    }

    if (player->position.y < -20.0f) {
        player->position.x = 0.0f;
        player->position.z = 0.0f;
        player->position.y = 5.0f;
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
    if ((!player->isGrounded && !Grounds(player->position.x, player->position.y - 0.1f, player->position.z)) && !player->isOnLadder) {
        player->velocityY += GRAVITY * deltaTime;
    }

    if (!player->isOnLadder) { // Disable gravity effect on a ladder
        player->position.y += player->velocityY * deltaTime;
    }

    if (Grounds(player->position.x, player->position.y - 0.1f, player->position.z)) {
        // Roof level: Acceptable if it matches the roof's bounds
        if (!Grounds(player->position.x, player->position.y, player->position.z)) {
            player->velocityY = 0.00f;
        } else {
            player->velocityY = 0.05f;
        }
        player->isGrounded = true;
    } else {
        // Not grounded
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
