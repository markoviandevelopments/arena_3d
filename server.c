#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

/*
// Player structure
typedef struct {
    int id;
    float x, y, z;
} Player;
*/

// Client information structure
typedef struct {
    int clientSocket;
    int player_id;
} ClientInfo;


// Globals
PlayerShort playerData [2] = {{ .id = 0, .x = 0.0f, .y = 0.0f, .z = 0.0f }, { .id = 1, .x = 0.0f, .y = 0.0f, .z = 0.0f }};
int nextPlayerId = 0;

float data[20] = {0.0f, -40.0f, 1.0f, 0.0f, -40.0f, 1.0f, 0.7f, 1.6f, 0.0f}; // (x-pos, z-pos, food ... x2), agent 1 angle, agent 2 angle

pthread_mutex_t playerMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t dataMutex = PTHREAD_MUTEX_INITIALIZER;


double get_epoch_time_in_milliseconds() {
    struct timespec ts;

    // Get the current time
    if (clock_gettime(CLOCK_REALTIME, &ts) != 0) {
        // Handle error
        perror("clock_gettime");
        return 0;
    }

    // Convert seconds and nanoseconds to milliseconds
    double milliseconds = ts.tv_sec * 1000.0 + ts.tv_nsec / 1e6;

    return milliseconds;
}


double time_ref;
double current_time;




// Thread function to update the data array
void *update_data_thread(void *arg) {
    (void)arg;
    while (1) {
        pthread_mutex_lock(&dataMutex);

        AlterBrownian(data); // Call the external function to update the data
        AlterArrowpad(data, playerData);

        pthread_mutex_unlock(&dataMutex);

        struct timespec ts;
        ts.tv_sec = 0;
        ts.tv_nsec = 10000000; // 10ms
        nanosleep(&ts, NULL);

    }
    return NULL;
}



void *handle_client(void *arg) {
    ClientInfo *clientInfo = (ClientInfo *)arg;
    int clientSocket = clientInfo->clientSocket;
    int player_id = clientInfo->player_id;
    free(clientInfo);
    nextPlayerId = (nextPlayerId + 1) % 2;
    char buffer[BUFFER_SIZE];

    while (1) {
        ssize_t bytesRead = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
        if (bytesRead <= 0) {
            printf("Client %d disconnected.\n", player_id);
            break;
        }
        buffer[bytesRead] = '\0';

        current_time = get_epoch_time_in_milliseconds() - time_ref;

        //printf("%lf  %f  %f\n", current_time, data[6], data[7]);

        pthread_mutex_lock(&playerMutex);
        sscanf(buffer, "%f %f %f %f %f %f", &playerData[player_id].x, &playerData[player_id].y, &playerData[player_id].z,  &playerData[1 - player_id].x, &playerData[1 - player_id].y, &playerData[1 - player_id].z);
        pthread_mutex_unlock(&playerMutex);

        snprintf(buffer, BUFFER_SIZE, "%d %f %f %f %f %f %f %lf %f %f %f %f %f %f", player_id, playerData[player_id].x, playerData[player_id].y, playerData[player_id].z, playerData[1 - player_id].x, playerData[1 - player_id].y, playerData[1 - player_id].z, current_time, data[0], data[1], data[2], data[3], data[4], data[5]);

        for (int i = 0; i < 3; i++) {
            char temp[32];
            snprintf(temp, sizeof(temp), " %f", data[i + 6]);
            strncat(buffer, temp, BUFFER_SIZE - strlen(buffer) - 1);
        }

        send(clientSocket, buffer, strlen(buffer), 0);
        printf("%s\n", buffer);
    }

    close(clientSocket);
    return NULL;
}

int main() {
    time_ref = get_epoch_time_in_milliseconds();
    current_time = time_ref;
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Socket creation failed");
        return EXIT_FAILURE;
    }

    struct sockaddr_in serverAddress = { 0 };
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Bind failed");
        close(serverSocket);
        return EXIT_FAILURE;
    }

    if (listen(serverSocket, 1) < 0) {
        perror("Listen failed");
        close(serverSocket);
        return EXIT_FAILURE;
    }

    printf("Server listening on port %d...\n", PORT);

    // Start the data update thread
    pthread_t dataThread;
    if (pthread_create(&dataThread, NULL, update_data_thread, NULL) != 0) {
        perror("Failed to create data update thread");
        close(serverSocket);
        return EXIT_FAILURE;
    }

    while (1) {
        struct sockaddr_in clientAddress;
        socklen_t clientLen = sizeof(clientAddress);
        int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientLen);
        if (clientSocket < 0) {
            perror("Accept failed");
            continue;
        }

        printf("Client %d connected.\n", nextPlayerId);


        // Allocate memory for client information
        ClientInfo *clientInfo = malloc(sizeof(ClientInfo));
        if (!clientInfo) {
            perror("Memory allocation failed");
            close(clientSocket);
            continue;
        }

        // Set client information
        clientInfo->clientSocket = clientSocket;
        clientInfo->player_id = nextPlayerId;


        pthread_t clientThread;
        pthread_create(&clientThread, NULL, handle_client, clientInfo);
        pthread_detach(clientThread);

        //nextPlayerId = (nextPlayerId + 1) % 2;
    }

    close(serverSocket);
    return EXIT_SUCCESS;
}
