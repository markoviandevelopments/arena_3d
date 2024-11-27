# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic
LDFLAGS = -lraylib -lm -pthread

# Source files
SERVER_SRC = server.c alter_brownian.c alter_arrowpad.c alter_bot.c alter_prestonparkour.c
CLIENT_SRC = client.c draw_chessboard.c draw_players.c draw_thing.c draw_arena.c ladders.c grounds.c walls.c draw_picture.c fox_animation.c player_handling.c draw_brownian.c draw_secrethouse.c draw_a_house.c draw_arrowpad.c draw_bot.c draw_mansion.c draw_prestonparkour.c draw_willohparkour.c
COMMON_HEADER = game.h

# Output binaries
SERVER_BIN = server
CLIENT_BIN = client

# Default target
all: $(SERVER_BIN) $(CLIENT_BIN)

# Compile the server
$(SERVER_BIN): $(SERVER_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile the client
$(CLIENT_BIN): $(CLIENT_SRC) $(COMMON_HEADER)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Clean up compiled files
clean:
	rm -f $(SERVER_BIN) $(CLIENT_BIN)

# Phony targets
.PHONY: all clean
