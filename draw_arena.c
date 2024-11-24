#include "game.h"

void DrawArena() {
    static Model model = { 0 };
    static bool isModelLoaded = false;
    if (!isModelLoaded) {
        model = LoadModel("models/arena.obj"); // Load the model
        Texture2D texture = LoadTexture("models/arena_texture.png"); // Load the texture

        // Attach the texture to the material
        model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

        // Set texture scaling using a shader value
        Shader shader = LoadShader("resources/scaling.vs", "resources/scaling.fs"); // Load a fragment shader for scaling
        model.materials[0].shader = shader;


        isModelLoaded = true;
    }

    // Update texture scaling factor
    Shader shader = model.materials[0].shader;
    float textureScale = 1.0f; // Use a constant for testing
    SetShaderValue(shader, GetShaderLocation(shader, "textureScale"), &textureScale, SHADER_UNIFORM_FLOAT);


    // Draw the loaded model
    Vector3 modelPosition = { 91.1f, 0.0f, 0.0f }; // Adjust position as needed
    Vector3 modelScale = { 1.0f, 1.0f, 1.0f };    // Scale factor (1/5 = 0.2)
    Vector3 rotationAxis = { 0.0f, 0.5f, 0.5f };   // Rotation around Y-axis
    float rotationAngle =  0.0f * sin(GetTime() * 0.5f * ( sin(GetTime() * 0.1f) + 1)) * 10.0f + 180.0f;       // Rotate over time (30 degrees per second)

    // Draw the model with rotation and scaling
    DrawModelEx(model, modelPosition, rotationAxis, rotationAngle, modelScale, WHITE);


}
