#include "game.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <math.h>
#include <time.h>
#include "raylib.h"
#include "raymath.h"


#define MOVE_SPEED 5.0f     // Movement speed
#define TURN_SPEED 170.0f   // Turn speed in degrees per second
#define GRAVITY -9.8f       // Gravity

typedef struct {
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

// Globals
Model foxModel;
ModelAnimation *animations;
int animCount = 0;
float animFrame = 0.0f;

// Globals for animation states
int currentAnimation = 0; // Default to Idle animation
int idleAnimation = 0;    // Animation index for Idle
int walkAnimation = 1;    // Animation index for Walk
int runAnimation = 2;     // Animation index for Run

float animFramePlayer1 = 0.0f;
float animFramePlayer2 = 0.0f;

double server_time;

int main() {

    // Networking variables
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    int dataSent = 0;
    int dataReceived = 0;
    int frameCount = 0;
    double lastTime = 0.0;
    int player_id = 0;

    float frequency = 0.0f;
    float avgBitsPerSecond = 0.0f;

    bool isMoving = false;
    bool isRunning = false;

    bool isRunningPlayer1 = false;
    bool isWalkingPlayer1 = false;
    bool isRunningPlayer2 = false;
    bool isWalkingPlayer2 = false;

    float animationSpeed = 30.0f; // Adjust this value based on the desired speed


    if (clientSocket < 0) {
        perror("Socket creation failed");
        return EXIT_FAILURE;
    }

    // Server connection setup
    struct sockaddr_in serverAddress = { 0 };
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT); // Replace with your server port
    inet_pton(AF_INET, SERVER_IP, &serverAddress.sin_addr); // Replace with your server IP

    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Connection failed");
        close(clientSocket);
        return EXIT_FAILURE;
    }

    printf("Connected to server.\n");

    InitWindow(800, 600, "Chessboard POV");
    SetTargetFPS(60);

    // Load the fox model and animations
    foxModel = LoadModel("models/Fox.glb");
    animations = LoadModelAnimations("models/Fox.glb", &animCount);
    printf("Animation count: %d\n", animCount);
    if (foxModel.meshCount == 0) {
        printf("Failed to load fox model!\n");
        CloseWindow();
        return EXIT_FAILURE;
    }
    if (animCount == 0) {
        printf("No animations found in fox model!\n");
    } else {
        animFrame = 0.0f; // Initialize animation frame to 0
    }


    // Initialize player
    Player player = { .position = { 0.0f, 1.0f, 0.0f }, .velocityY = 0.0f, .isGrounded = false, .yaw = 0.0f, .pitch = 0.0f };
    OtherPlayer otherPlayer = { .position = { 2.0f, 1.0f, 2.0f } };

    // Initialize camera
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 0.0f, 5.0f, 10.0f };
    camera.target = player.position;
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    lastTime = GetTime();
    avgBitsPerSecond = 0.0f;
    bool is_jumping = false;


    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

 //       if (fabs(moveDirection.x) > 0.0f || fabs(moveDirection.z) > 0.0f) {
 //           isMoving = true;
 //           if (IsKeyDown(KEY_E)) { // Example key for running
//                isRunning = true;
//            }
//        }

        // Switch animations based on movement state
        if (isRunning) {
            if (currentAnimation != runAnimation) {
                currentAnimation = runAnimation;
                animFrame = 0; // Reset frame when switching animations
            }
        } else if (isMoving) {
            if (currentAnimation != walkAnimation) {
                currentAnimation = walkAnimation;
                animFrame = 0; // Reset frame when switching animations
            }
        } else {
            if (currentAnimation != idleAnimation) {
                currentAnimation = idleAnimation;
                animFrame = 0; // Reset frame when switching animations
            }
        }

        // Update animation
        if (animCount > 0) {
            animFrame += deltaTime * 30.0f; // Adjust speed for animations
            if (animFrame >= animations[currentAnimation].frameCount) animFrame = 0;
            UpdateModelAnimation(foxModel, animations[currentAnimation], (int)animFrame);
        }

        // Apply gravity if not grounded
        if (!player.isGrounded) {
            player.velocityY += GRAVITY * deltaTime;
        }

        //Check for other grounds
        if (Grounds(player.position.x, player.position.y, player.position.z) && !is_jumping) {
            player.velocityY = 0.0f;
        }


        // Update player's vertical position
        player.position.y += player.velocityY * deltaTime;

        // Check if the player is grounded (on the chessboard)
        if (player.position.y <= PLAYER_HEIGHT) {
            player.position.y = PLAYER_HEIGHT;
            player.velocityY = 0.0f;
            player.isGrounded = true;
        } else {
            player.isGrounded = false;
        }

        // Determine animation state for Player 1
        int animIndex1 = idleAnimation; // Default to idle
        if (isRunningPlayer1) {
            animIndex1 = runAnimation;
        } else if (isWalkingPlayer1) {
            animIndex1 = walkAnimation;
        }

        // Update animation frame
        animFramePlayer1 += deltaTime * animationSpeed;
        if (animFramePlayer1 >= animations[animIndex1].frameCount) {
            animFramePlayer1 = 0; // Loop the animation
        }

        // Jump
        if (IsKeyPressed(KEY_SPACE) && (player.isGrounded || Ladders(player.position.x, player.position.y, player.position.z) || Grounds(player.position.x, player.position.y, player.position.z)) ) {
            player.velocityY = 5.0f;
            player.isGrounded = false;
            is_jumping = true;
        } else {
            is_jumping = false;
        }


        //Take a screenshot
        if (IsKeyPressed(KEY_P)) {
            time_t current_time = time(NULL);
            char time_str[128];
            snprintf(time_str, sizeof(time_str), "%ld", current_time - 1466112000);
            char filename_string[64];
            strcpy(filename_string, "screenshot_");
            strcat(filename_string, time_str);
            strcat(filename_string, ".png");
            printf("Saving screenshot to: %s\n", filename_string);
            TakeScreenshot(filename_string);
            char command[300];
            snprintf(command, sizeof(command), "mv %s screenshots/", filename_string);
            system(command);
        }

        // Handle camera/player rotation with consistent direction
        if (IsKeyDown(KEY_LEFT)) player.yaw -= TURN_SPEED * deltaTime; // Rotate left
        if (IsKeyDown(KEY_RIGHT)) player.yaw += TURN_SPEED * deltaTime; // Rotate right
        if (IsKeyDown(KEY_UP)) player.pitch -= TURN_SPEED * deltaTime;
        if (IsKeyDown(KEY_DOWN)) player.pitch += TURN_SPEED * deltaTime;

        // Limit pitch to avoid flipping
        if (player.pitch > 89.0f) player.pitch = 89.0f;
        if (player.pitch < -89.0f) player.pitch = -89.0f;

        // Reset movement flags
        isWalkingPlayer1 = false;
        isRunningPlayer1 = false;

        Vector3 moveDirection = { 0.0f, 0.0f, 0.0f };

        // Handle movement (relative to player's yaw)
        if (IsKeyDown(KEY_W)) { // Move forward
            moveDirection.x += cos(DEG2RAD * player.yaw) * MOVE_SPEED * deltaTime;
            moveDirection.z += sin(DEG2RAD * player.yaw) * MOVE_SPEED * deltaTime;
            isWalkingPlayer1 = true; // Player is walking
        }
        if (IsKeyDown(KEY_S)) { // Move backward
            moveDirection.x -= cos(DEG2RAD * player.yaw) * MOVE_SPEED * deltaTime;
            moveDirection.z -= sin(DEG2RAD * player.yaw) * MOVE_SPEED * deltaTime;
            isWalkingPlayer1 = true;
        }
        if (IsKeyDown(KEY_E)) {
            moveDirection.x += 3.0f * cos(DEG2RAD * player.yaw) * MOVE_SPEED * deltaTime;
            moveDirection.z += 3.0f * sin(DEG2RAD * player.yaw) * MOVE_SPEED * deltaTime;
            isRunningPlayer1 = true; // Player is running
            isWalkingPlayer1 = false; // Override walking state if running
        }
        if (IsKeyDown(KEY_A)) { // Strafe left
            moveDirection.x += cos(DEG2RAD * (player.yaw - 90)) * MOVE_SPEED * deltaTime;
            moveDirection.z += sin(DEG2RAD * (player.yaw - 90)) * MOVE_SPEED * deltaTime;
            isWalkingPlayer1 = true;
        }
        if (IsKeyDown(KEY_D)) { // Strafe right
            moveDirection.x += cos(DEG2RAD * (player.yaw + 90)) * MOVE_SPEED * deltaTime;
            moveDirection.z += sin(DEG2RAD * (player.yaw + 90)) * MOVE_SPEED * deltaTime;
            isWalkingPlayer1 = true;
        }

        // Apply movement to the player's position
        player.position.x += moveDirection.x;
        player.position.z += moveDirection.z;

        // Calculate the forward direction based on the player's yaw
        Vector3 forward = {
            cos(DEG2RAD * player.yaw),
            0.0f,
            sin(DEG2RAD * player.yaw)
        };

        // Update the camera's position (slightly behind and above the player)
        camera.position = (Vector3){
            player.position.x - forward.x * 10.0f, // Move the camera back along the forward direction
            player.position.y + 3.0f,              // Slightly above the player
            player.position.z - forward.z * 10.0f  // Move the camera back along the forward direction
        };

        // Update the camera's target to point in front of the player
        camera.target = (Vector3){
            player.position.x + forward.x,
            player.position.y + 1.0f, // Focus on the player's head height
            player.position.z + forward.z
        };

        // Send player position to server
        char buffer[BUFFER_SIZE];
        snprintf(buffer, BUFFER_SIZE, "%f %f %f", player.position.x, player.position.y, player.position.z);
        int bytesSent = send(clientSocket, buffer, strlen(buffer), 0);
        if (bytesSent > 0) dataSent += bytesSent;

        // Receive data from server
        ssize_t bytesRead = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
        if (bytesRead > 0) {
            buffer[bytesRead] = '\0';
            sscanf(buffer, "%d %f %f %f %f %f %f %lf", &player_id, &player.position.x, &player.position.y, &player.position.z,
                   &otherPlayer.position.x, &otherPlayer.position.y, &otherPlayer.position.z, &server_time);
            dataReceived += bytesRead;
            frameCount++;
        }

        printf("%s\n", buffer);

        // Update frequency and average bits per second
        double currentTime = GetTime();
        if (currentTime - lastTime >= 1.0) {
            frequency = (float)frameCount / (currentTime - lastTime);
            avgBitsPerSecond = ((dataSent + dataReceived) * 8.0f) / (currentTime - lastTime);
            frameCount = 0;
            dataSent = 0;
            dataReceived = 0;
            lastTime = currentTime;

            // printf("Frequency: %.2f Hz, Avg bits/s: %.2f\n", frequency, avgBitsPerSecond);
        }

        // Determine animation state for Player 1
        if (isRunningPlayer1) animIndex1 = runAnimation;
        else if (isWalkingPlayer1) animIndex1 = walkAnimation;

        // Determine animation state for Player 2
        int animIndex2 = 0; // Default to idle animation
        if (isRunningPlayer2) animIndex2 = runAnimation;
        else if (isWalkingPlayer2) animIndex2 = walkAnimation;

        // Calculate rotation and translation matrix
        Matrix rotationMatrix = MatrixRotateY(DEG2RAD * player.yaw); // Rotate the model based on yaw
        Matrix translationMatrix = MatrixTranslate(player.position.x, player.position.y, player.position.z);

        // Apply combined transformation to the model
        foxModel.transform = MatrixMultiply(rotationMatrix, translationMatrix);

        // Render frame
        BeginDrawing();

        ClearBackground((Color){0, 0, 0, 255});

        Color topColor = (Color){135, 206, 250, 255};    // Light blue (sky)
        Color bottomColor = (Color){25, 25, 112, 255};   // Dark blue (near horizon)
        DrawRectangleGradientV(0, 0, GetScreenWidth(), GetScreenHeight(), topColor, bottomColor);


        BeginMode3D(camera);

        DrawChessboard(BOARD_SIZE, SQUARE_SIZE);
        // DrawPlayers(player, otherPlayer, foxModel);
        // DrawPlayers(player_id, player.position.x, player.position.y, player.position.z, otherPlayer.position.x, otherPlayer.position.y, otherPlayer.position.z);
        DrawPlayers(
            player_id,
            player.position.x, player.position.y, player.position.z, animIndex1, animFramePlayer1,
            otherPlayer.position.x, otherPlayer.position.y, otherPlayer.position.z, animIndex2, animFramePlayer2
        );
        DrawArena();
        DrawThing(server_time);
        DrawPicture();
        EndMode3D();

        Color text_color = BLACK;

        DrawText("Move with WASD, look with arrow keys, jump with SPACE", 10, 10, 20, text_color);
        DrawText(TextFormat("Frequency: %.2f Hz", frequency), 10, 40, 20, text_color);
        DrawText(TextFormat("Avg bits/s: %.2f", avgBitsPerSecond), 10, 70, 20, text_color);
        DrawText(TextFormat("ID: %d  X: %.2f  Y: %.2f  Z: %.2f", player_id, player.position.x, player.position.y, player.position.z), 10, 100, 20, text_color);
        DrawText(TextFormat("Session Time: %.2f  Server Time: %.2lf", GetTime(), server_time / 1000.0), 10, 130, 20, text_color);
        EndDrawing();
    }

    UnloadModel(foxModel);
    if (animCount > 0) UnloadModelAnimations(animations, animCount);

    CloseWindow();
    close(clientSocket);
    return EXIT_SUCCESS;
}
