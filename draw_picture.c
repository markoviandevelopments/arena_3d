#include "game.h"
#include "rlgl.h"

void DrawPicture() {
    // Static variables to ensure resources are loaded only once
    static Texture2D texture = { 0 };
    static bool isTextureLoaded = false;

    if (!isTextureLoaded) {
        // Load the texture
        texture = LoadTexture("models/surreal.png");
        printf("%d", texture.id);
        // Check if the texture loaded successfully
        if (texture.id == 0) {
            TraceLog(LOG_ERROR, "Failed to load texture");
            return;
        }

        // Set texture filtering and wrapping if necessary
        SetTextureFilter(texture, TEXTURE_FILTER_BILINEAR);
        SetTextureWrap(texture, TEXTURE_WRAP_CLAMP);

        isTextureLoaded = true;
    }

    // Define position, size, rotation axis, and angle
    Vector3 position = { 10.0f, 3.0f, 0.0f };         // Position in 3D space
    Vector2 size = { 4.0f, 3.0f };                    // Width and height of the quad
    Vector3 rotationAxis = { 0.0f, 1.0f, 0.0f };      // Rotate around Y-axis
    float rotationAngle = GetTime() * 30.0f;          // Rotate over time (degrees)

    // Save current matrix
    rlPushMatrix();

    // Apply transformations
    rlTranslatef(position.x, position.y, position.z);
    rlRotatef(rotationAngle, rotationAxis.x, rotationAxis.y, rotationAxis.z);

    // Disable backface culling to ensure the quad is visible
    rlDisableBackfaceCulling();

    // Set blending mode to alpha blending
    rlSetBlendMode(RL_BLEND_ALPHA);

    // Enable texture
    rlEnableTexture(texture.id);

    // Draw the quad
    rlBegin(RL_QUADS);
        // Bottom-left corner
        rlTexCoord2f(0.0f, 1.0f);
        rlVertex3f(-size.x / 2, -size.y / 2, 0.0f);

        // Bottom-right corner
        rlTexCoord2f(1.0f, 1.0f);
        rlVertex3f(size.x / 2, -size.y / 2, 0.0f);

        // Top-right corner
        rlTexCoord2f(1.0f, 0.0f);
        rlVertex3f(size.x / 2, size.y / 2, 0.0f);

        // Top-left corner
        rlTexCoord2f(0.0f, 0.0f);
        rlVertex3f(-size.x / 2, size.y / 2, 0.0f);
    rlEnd();

    // Disable texture
    rlDisableTexture();

    // Reset blending mode to default (if necessary)
    rlSetBlendMode(RL_BLEND_ALPHA_PREMULTIPLY);

    // Re-enable backface culling
    rlEnableBackfaceCulling();

    // Restore previous matrix
    rlPopMatrix();
}
