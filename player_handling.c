#include "include/player_handling.h"
#include "game.h"

#include <math.h>


int wait_c = 0;

// Update the camera's position and handle rotation
void UpdatePlayerCamera(Camera3D *camera, Player *player, float deltaTime, int *sensitivity_r) {
    // Handle camera/player rotation
    if (IsKeyDown(KEY_LEFT)) player->yaw -= TURN_SPEED * deltaTime * pow(2, *sensitivity_r);   // Rotate left
    if (IsKeyDown(KEY_RIGHT)) player->yaw += TURN_SPEED * deltaTime * pow(2, *sensitivity_r);  // Rotate right
    if (IsKeyDown(KEY_DOWN)) player->pitch -= TURN_SPEED * deltaTime * pow(2, *sensitivity_r);   // Look up
    if (IsKeyDown(KEY_UP)) player->pitch += TURN_SPEED * deltaTime * pow(2, *sensitivity_r); // Look down

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
    bool *isJumping, int (*Walls)(float, float, float),
    int (*Ladders)(float, float, float),
    int *sensitivity,
    int *sensitivity_r,
    float spawn_x,
    float spawn_z
) {
    *isWalking = false;
    *isRunning = false;

    player->moveDirection = (Vector3){ 0.0f, 0.0f, 0.0f };

    player->isOnLadder = Ladders(player->position.x, player->position.y, player->position.z);

    int is_strafing = 0;

    // Movement input handling
    if (IsKeyDown(KEY_W)) { // Move forward
        player->moveDirection.x += cos(DEG2RAD * player->yaw) * MOVE_SPEED * deltaTime * pow(2, (double)(*sensitivity));
        player->moveDirection.z += sin(DEG2RAD * player->yaw) * MOVE_SPEED * deltaTime * pow(2, (double)(*sensitivity));
        *isWalking = true;
    }
    if (IsKeyDown(KEY_S)) { // Move backward
        player->moveDirection.x -= cos(DEG2RAD * player->yaw) * MOVE_SPEED * deltaTime * pow(2, (double)(*sensitivity));
        player->moveDirection.z -= sin(DEG2RAD * player->yaw) * MOVE_SPEED * deltaTime * pow(2, (double)(*sensitivity));
        *isWalking = true;
    }
    if (IsKeyDown(KEY_E)) { // Sprint
        player->moveDirection.x += 3.0f * cos(DEG2RAD * player->yaw) * MOVE_SPEED * deltaTime;
        player->moveDirection.z += 3.0f * sin(DEG2RAD * player->yaw) * MOVE_SPEED * deltaTime;
        *isRunning = true;
        *isWalking = false; // Override walking
    }
    if (IsKeyDown(KEY_A)) { // Strafe left
        player->moveDirection.x += cos(DEG2RAD * (player->yaw - 90)) * MOVE_SPEED * deltaTime * pow(2, (double)(*sensitivity));
        player->moveDirection.z += sin(DEG2RAD * (player->yaw - 90)) * MOVE_SPEED * deltaTime * pow(2, (double)(*sensitivity));
        *isWalking = true;
        is_strafing = 1;
    }
    if (IsKeyDown(KEY_D)) { // Strafe right
        player->moveDirection.x += cos(DEG2RAD * (player->yaw + 90)) * MOVE_SPEED * deltaTime * pow(2, (double)(*sensitivity));
        player->moveDirection.z += sin(DEG2RAD * (player->yaw + 90)) * MOVE_SPEED * deltaTime * pow(2, (double)(*sensitivity));
        *isWalking = true;
        is_strafing = 1;
    }

    // Update yaw based on moveDirection
    if (Vector3Length(player->moveDirection) > 0.01f && !is_strafing) {

        float baseYaw = atan2(player->moveDirection.z, player->moveDirection.x) * RAD2DEG;

        // If moving backward, invert the yaw
        if (IsKeyDown(KEY_S)) {
            player->yaw = baseYaw - 180.0f; // Face the opposite direction
        } else {
            player->yaw = baseYaw; // Regular direction
        }

        // Normalize yaw to [0, 360)
        if (player->yaw < 0.0f) player->yaw += 360.0f;
        if (player->yaw >= 360.0f) player->yaw -= 360.0f;
    }

    if (IsKeyDown(KEY_M) && !wait_c) { // Increase movement sensitivity
        (*sensitivity)++;
        wait_c += 10;
    }
    if (IsKeyDown(KEY_N) && !wait_c) { // Decrease movement sensitivity
        (*sensitivity)--;
        wait_c += 10;
    }

    if (IsKeyDown(KEY_K) && !wait_c) { // Increase rotational sensitivity
        (*sensitivity_r)++;
        wait_c += 10;
    }
    if (IsKeyDown(KEY_J) && !wait_c) { // Decrease rotational sensitivity
        (*sensitivity_r)--;
        wait_c += 10;
    }


    if (wait_c > 0) {
        wait_c--;
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
        player->position.x = spawn_x;
        player->position.z = spawn_z;
        player->position.y = 5.0f;
        player->velocityY = 0.0f;
        player->yaw = 90.0f;
    }

    // Collision handling using proposed position
    Vector3 proposedPosition = {
        player->position.x + player->moveDirection.x,
        player->position.y,
        player->position.z + player->moveDirection.z,
    };

    if (!Walls(proposedPosition.x, player->position.y, player->position.z) &&
        !MazeCollision(proposedPosition.x, player->position.y, player->position.z, maze)) {
        player->position.x = proposedPosition.x; // Apply X movement
    } else {
        player->moveDirection.x = 0.0f; // Stop X movement
    }

    if (!Walls(player->position.x, player->position.y, proposedPosition.z) &&
        !MazeCollision(player->position.x, player->position.y, proposedPosition.z, maze)) {
        player->position.z = proposedPosition.z; // Apply Z movement
    } else {
        player->moveDirection.z = 0.0f; // Stop Z movement
    }


}

// Apply gravity to the player
void ApplyGravity(Player *player, float deltaTime) {
    if ((!player->isGrounded && !Grounds(player->position.x, player->position.y - 0.1f, player->position.z)) && !player->isOnLadder && !IsKeyDown(KEY_O)) {
        player->velocityY += GRAVITY * deltaTime;
    } else if (IsKeyDown(KEY_O)) {
        player->velocityY = 7.5f;
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
