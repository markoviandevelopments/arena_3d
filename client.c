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

#include "include/fox_animation.h"
#include "include/player_handling.h"
#include "game.h"

float animFramePlayer1 = 0.0f;
float animFramePlayer2 = 0.0f;

double server_time;

float data[20] = {0.0f, -40.0f, 0.0f};

int sensitivity = 0;
int sensitivity_r = 0;

bool isGhostMode = false;


WillsBlocks willsBlocks[BLOCKCOUNT];

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

    float animationSpeed = 40.0f; // Adjust this value based on the desired speed

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

    InitWindow(1200, 900, "Chessboard POV");
    SetTargetFPS(60);

    LoadFoxAnimations("models/Fox.glb");
    if (animCount == 0) {
        CloseWindow();
        return EXIT_FAILURE;
    }

    // Initialize player
    Player player;
    InitializePlayer(&player, (Vector3){ 0.0f, 1.0f, 0.0f });
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

        HandlePlayerMovement(&player, deltaTime, &isWalkingPlayer1, &isRunningPlayer1, &is_jumping, Walls, Ladders, &sensitivity, &sensitivity_r, data[9], data[10]);
        Teleports(&player);

        isMoving = isWalkingPlayer1 || isRunningPlayer1;
        isRunning = isRunningPlayer1;

        // Apply movement to the player's position with wall collision check
        ApplyGravity(&player, deltaTime);

        // Update animation
        UpdateFoxAnimation(deltaTime, isMoving, isRunning);

        UpdatePlayerCamera(&camera, &player, deltaTime, &sensitivity_r);

        // Determine animation state for Player 1
        int animIndex1 = idleAnimation; // Default to idle
        int animIndex2 = idleAnimation; // Default to idle for the other player
        if (isRunningPlayer1) {
            animIndex1 = runAnimation;
        } else if (isWalkingPlayer1) {
            animIndex1 = walkAnimation;
        }

        if (isRunningPlayer2) {
            animIndex2 = runAnimation;
        } else if (isWalkingPlayer2) {
            animIndex2 = walkAnimation;
        }

        // Update animation frame
        animFramePlayer1 += deltaTime * animationSpeed;
        if (animFramePlayer1 >= animations[animIndex1].frameCount) {
            animFramePlayer1 = 0; // Loop the animation
        }

        // Update animation frame
        animFramePlayer2 += deltaTime * animationSpeed;
        if (animFramePlayer2 >= animations[animIndex1].frameCount) {
            animFramePlayer2 = 0; // Loop the animation
        }

        if (IsKeyPressed(KEY_G)) {
            isGhostMode = 1 - isGhostMode;
            if(isGhostMode) {
                printf("Ghost Mode activated...\n\n\n\n");
            } else {printf("Ghost Mode deactivated...\n\n\n\n\n");}
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

        // Send player position to server
        char buffer[BUFFER_SIZE];
        snprintf(buffer, BUFFER_SIZE, "%f %f %f", player.position.x, player.position.y, player.position.z);
        int bytesSent = send(clientSocket, buffer, strlen(buffer), 0);
        if (bytesSent > 0) dataSent += bytesSent;

        // Receive data from server
        ssize_t bytesRead = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
        if (bytesRead > 0) {
            buffer[bytesRead] = '\0';
            // Parse the fixed part of the message
            sscanf(buffer, "%d %f %f %f %f %f %f %lf",
                             &player_id,
                             &player.position.x, &player.position.y, &player.position.z,
                             &otherPlayer.position.x, &otherPlayer.position.y, &otherPlayer.position.z,
                             &server_time);


            // Parse the dynamic `data` part of the message
            char *dynamicPart = buffer;
            for (int i = 0; i < 8; i++) {  // Leave fixed at 8
                dynamicPart = strchr(dynamicPart, ' ');
                if (dynamicPart) {
                    dynamicPart++;
                }
            }

            int dataIndex = 0;
            while (dynamicPart && dataIndex < 12) {
                if (sscanf(dynamicPart, "%f", &data[dataIndex]) == 1) {
                    dataIndex++;
                }
                dynamicPart = strchr(dynamicPart, ' ');
                if (dynamicPart) {
                    dynamicPart++;
                }
            }

            // Increment data stats for debugging
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

        // Render frame
        BeginDrawing();

        ClearBackground((Color){0, 0, 0, 255});

        Color topColor = (Color){135, 206, 250, 255};    // Light blue (sky)
        Color bottomColor = (Color){25, 25, 112, 255};   // Dark blue (near horizon)
        DrawRectangleGradientV(0, 0, GetScreenWidth(), GetScreenHeight(), topColor, bottomColor);


        BeginMode3D(camera);

        DrawChessboard(BOARD_SIZE, SQUARE_SIZE);
        DrawPlayers(
            player_id,
            player.position.x, player.position.y, player.position.z, animIndex1, animFramePlayer1, player.yaw, isGhostMode,
            otherPlayer.position.x, otherPlayer.position.y, otherPlayer.position.z, animIndex2, animFramePlayer2
        );
        DrawArena();
        DrawMansion();
        DrawPrestonparkour(data, server_time);
        DrawSecrethouse();
        DrawAHouse();
        DrawArrowpad(data);
        DrawBot(data);
        DrawThing(server_time);
        DrawBrownian(data);
        //DrawPicture();
        DrawWillsBlocks(willsBlocks);
        EndMode3D();

        Color text_color = (Color){150, 0, 100, 255};

        DrawText("Move with WASD, look with arrow keys, jump with SPACE", 10, 10, 20, text_color);
        DrawText(TextFormat("Frequency: %.2f Hz", frequency), 10, 40, 20, text_color);
        DrawText(TextFormat("Avg bits/s: %.2f", avgBitsPerSecond), 10, 70, 20, text_color);
        DrawText(TextFormat("ID: %d  X: %.2f  Y: %.2f  Z: %.2f", player_id, player.position.x, player.position.y, player.position.z), 10, 100, 20, text_color);
        DrawText(TextFormat("Session Time: %.2f  Server Time: %.2lf", GetTime(), server_time / 1000.0), 10, 130, 20, text_color);
        DrawText(TextFormat("Agent 1 Score: %.2f  Agent 2 Score: %.2f", data[2], data[5]), 10, 150, 20, text_color);
        if (sensitivity >= 0 && sensitivity <= 15) {
            DrawText(TextFormat("Sensitivity: %d  Data item at this index: %f", sensitivity, data[sensitivity]), 10, 180, 20, text_color);
        } else {
            DrawText(TextFormat("Sensitivity: %d", sensitivity), 10, 180, 20, text_color);
        }
        DrawText(TextFormat("Rotation Sensitivity: %d", sensitivity_r), 10, 210, 20, text_color);
        DrawText(TextFormat("Yaw: %.2f  Pitch: %.2f", player.yaw, player.pitch), 10, 240, 20, text_color);

        EndDrawing();
    }
    UnloadFoxAnimations();

    CloseWindow();
    close(clientSocket);
    return EXIT_SUCCESS;
}
