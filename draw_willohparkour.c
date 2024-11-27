#include "game.h"
#include "raylib.h"
#include <stdlib.h>


static inline Color GenerateRandomBlockColor(){
    return (Color){ rand() % 256, rand() % 256, rand() % 256, 255 };
}
static inline void DrawWillohBlock(WillsBlocks block) {
    DrawCube(block.position, 2.0f, 4.0f, 2.0f, block.blockColor);
    DrawCubeWires(block.position, 2.0f, 4.0f, 2.0f, BLACK);
}

static void GenerateWillsBlocks(WillsBlocks *blocks) {
    blocks[0].position = (Vector3){ 86.0f, 2.0f, -70.0f };
    blocks[0].blockColor = GenerateRandomBlockColor();

    for (int i = 1; i < BLOCKCOUNT; i++) {
        blocks[i].position.x = blocks[i - 1].position.x - 2;
        blocks[i].position.y = 2.0f;
        blocks[i].position.z = blocks[i - 1].position.z - 1;
        blocks[i].blockColor = GenerateRandomBlockColor();
    }
}

void DrawWillsBlocks(WillsBlocks *blocks) {
    GenerateWillsBlocks(blocks);
    for(int block = 0; block < BLOCKCOUNT; block++) {
        DrawWillohBlock(blocks[block]);
    }
}