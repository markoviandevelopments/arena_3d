#include "game.h"


float x;
float z;

void DrawArrowpad(float *data) {
    static Model model = { 0 };
    static bool isModelLoaded = false;
    if (!isModelLoaded) {
        model = LoadModel("models/arrowpad.obj"); // Load the model
        Texture2D texture = LoadTexture("models/arrowpad_texture.png"); // Load the texture

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
    Vector3 modelPosition = { -20.0f, 0.00f, 20.0f }; // Adjust position as needed
    Vector3 modelScale = { 1.0f, 1.0f, 1.0f };
    Vector3 rotationAxis = { 1.0f, 0.0f, 0.0f };   // Rotation around Y-axis
    float rotationAngle =  0.0f;

    // Draw the model with rotation and scaling
    DrawModelEx(model, modelPosition, rotationAxis, rotationAngle, modelScale, WHITE);


    Color squareColor = BLUE;


    if (data[7] == 1.0f) {
        x = -11.3f;
        z = 15.5f;
    } else if (data[7] == 2.0f) {
        x = -15.5f;
        z = 11.3f;
    } else if (data[7] == 3.0f) {
        x = -19.75f;
        z = 15.5f;
    } else if (data[7] == 4.0f) {
        x = -15.5f;
        z = 19.75f;
    }  else {
        x = -15.75f;
        z = 15.3f;
     }


    Vector3 position = {
        x,
        1.4f,
        z
    };
    DrawCube(position, 0.3f, 0.1f, 0.3f, squareColor);
    DrawCubeWires(position, 0.3f, 0.1f, 0.3f, BLACK);




}
