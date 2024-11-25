#include "game.h"
#include "rlgl.h"

void DrawPicture() {
    static Texture2D texture = { 0 };

    // Load the texture only once
    if (texture.id == 0) {
        texture = LoadTexture("models/img1.png");  // Ensure the path to the texture is correct

        if (texture.id == 0) {
            TraceLog(LOG_ERROR, "Failed to load texture.");
            return;
        }

        // Generate mipmaps and configure filtering
        GenTextureMipmaps(&texture);
        SetTextureFilter(texture, TEXTURE_FILTER_TRILINEAR);
        TraceLog(LOG_INFO, "Texture loaded successfully with ID: %d", texture.id);
    }

    // Set default shader explicitly for your version of rlgl
    rlSetShader(rlGetShaderIdDefault(), NULL);

    // Disable depth testing (ensures the texture is always rendered)
    rlDisableDepthTest();

    // Push transformation matrix
    rlPushMatrix();
    rlTranslatef(0.0f, 0.0f, -5.0f);  // Place the quad in front of the camera

    // Enable the texture
    rlEnableTexture(texture.id);

    // Draw the textured quad
    rlBegin(RL_QUADS);
        rlTexCoord2f(0.0f, 1.0f); rlVertex3f(-2.0f, -2.0f, 0.0f); // Bottom-left
        rlTexCoord2f(1.0f, 1.0f); rlVertex3f( 2.0f, -2.0f, 0.0f); // Bottom-right
        rlTexCoord2f(1.0f, 0.0f); rlVertex3f( 2.0f,  2.0f, 0.0f); // Top-right
        rlTexCoord2f(0.0f, 0.0f); rlVertex3f(-2.0f,  2.0f, 0.0f); // Top-left
    rlEnd();

    // Disable the texture
    rlDisableTexture();

    // Restore depth testing and pop the matrix
    rlEnableDepthTest();
    rlPopMatrix();
}
