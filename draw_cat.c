#include "game.h"



void DrawCat(float *data) {

    float x;
    float y;
    float z;

    x = data[12];
    y = data[13];
    z = data[14];

    static Model model = { 0 };
    static bool isModelLoaded = false;
    if (!isModelLoaded) {
        model = LoadModel("models/cat.obj"); // Load the model
        Texture2D texture = LoadTexture("models/cat.png"); // Load the texture

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
    Vector3 modelPosition = { x, y, z };
    Vector3 modelScale = { 9.0f, 9.0f, 9.0f };
    Vector3 rotationAxis = { 1.0f, 0.0f, 0.0f };   // Rotation around Y-axis
    float rotationAngle =  0.0f;

    // Draw the model with rotation and scaling
    DrawModelEx(model, modelPosition, rotationAxis, rotationAngle, modelScale, WHITE);

/*
    Color squareColor = RED;

    x = data[9];
    z = data[10];


    Vector3 position = {
        x,
        0.4f,
        z
    };
    DrawCube(position, 1.5f, 0.2f, 1.5f, squareColor);
    DrawCubeWires(position, 1.5f, 0.2f, 1.5f, BLACK);
*/



}
