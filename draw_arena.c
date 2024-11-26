#include "game.h"

void DrawArena() {
    static Model model = { 0 };
    static bool isModelLoaded = false;
    if (!isModelLoaded) {
        model = LoadModel("models/uv_arena.obj"); // Load the model
        Texture2D texture = LoadTexture("models/less_clear.png"); // Load the texture

        // After loading the texture
        SetTextureFilter(texture, TEXTURE_FILTER_TRILINEAR); // Or TEXTURE_FILTER_BILINEAR

        // After loading the texture
        SetTextureWrap(texture, TEXTURE_WRAP_REPEAT);


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
    Vector3 modelPosition = { -0.0f, -0.82f, -40.0f }; // Adjust position as needed
    Vector3 modelScale = { 1.0f, 1.0f, 1.0f };
    Vector3 rotationAxis = { 1.0f, 0.0f, 0.0f };   // Rotation around Y-axis
    float rotationAngle =  0.0f * sin(GetTime() * 0.5f * ( sin(GetTime() * 0.1f) + 1)) * 10.0f - 90.0f;

    // Draw the model with rotation and scaling
    DrawModelEx(model, modelPosition, rotationAxis, rotationAngle, modelScale, WHITE);



    Color squareColor = (Color){255, 0, 255, 200};

    Vector3 position = {
        -61.0f + 1.0f,
        0.0f + 12.5f,
        -57.0f + 1.0f
         };

    DrawCube(position, 2.0f, 25.0f, 2.0f, squareColor);
    DrawCubeWires(position, 2.0f, 25.0f, 2.0f, BLACK);


}
