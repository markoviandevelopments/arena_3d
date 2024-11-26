#include "game.h"



void DrawBot(float *data) {

    float x;
    float z;

    x = data[9];
    z = data[10];

    static Model model = { 0 };
    static bool isModelLoaded = false;
    if (!isModelLoaded) {
        model = LoadModel("models/man.obj"); // Load the model
        Texture2D texture = LoadTexture("models/man.png"); // Load the texture

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
    Vector3 modelPosition = { x, 0.4f, z }; // Adjust position as needed
    Vector3 modelScale = { 1.0f, 1.0f, 1.0f };
    Vector3 rotationAxis = { 1.0f, 0.0f, 0.0f };   // Rotation around Y-axis
    float rotationAngle =  0.0f;

    // Draw the model with rotation and scaling
    DrawModelEx(model, modelPosition, rotationAxis, rotationAngle, modelScale, WHITE);


    Color squareColor = RED;

    x = data[9];
    z = data[10];


    Vector3 position = {
        x,
        0.4f,
        z
    };
    DrawCube(position, 0.5f, 0.2f, 0.5f, squareColor);
    DrawCubeWires(position, 0.5f, 0.2f, 0.5f, BLACK);




}
