#include "include/fox_animation.h"
#include <stdio.h>
#include <math.h> // Include math.h for fmod

// Animation globals
Model foxModel;
ModelAnimation *animations = NULL;
int animCount = 0;
float animFrame = 0.0f;
int currentAnimation = 0; // Default to Idle animation
int idleAnimation = 0;    // Animation index for Idle
int walkAnimation = 1;    // Animation index for Walk
int runAnimation = 2;     // Animation index for Run

// Function to load the fox model and animations
void LoadFoxAnimations(const char *modelPath) {
    foxModel = LoadModel(modelPath);
    animations = LoadModelAnimations(modelPath, &animCount);
    
    if (foxModel.meshCount == 0) {
        fprintf(stderr, "Failed to load fox model!\n");
        return;
    }

    if (animCount == 0) {
        fprintf(stderr, "No animations found in fox model!\n");
    } else {
        animFrame = 0.0f; // Initialize animation frame
    }
}

// Function to update the fox animation based on movement states
void UpdateFoxAnimation(float deltaTime, bool isMoving, bool isRunning) {
    if (animCount == 0) return; // No animations loaded

    // Determine the animation state
    if (isRunning) {
        if (currentAnimation != runAnimation) {
            currentAnimation = runAnimation;
            animFrame = 0; // Reset frame
        }
    } else if (isMoving) {
        if (currentAnimation != walkAnimation) {
            currentAnimation = walkAnimation;
            animFrame = 0; // Reset frame
        }
    } else {
        if (currentAnimation != idleAnimation) {
            currentAnimation = idleAnimation;
            animFrame = 0; // Reset frame
        }
    }

    // Update animation
    if (animCount > 0) {
        animFrame += deltaTime * 30.0f; // Adjust speed for animations
        if (animFrame >= animations[currentAnimation].frameCount) animFrame = 0;
        UpdateModelAnimation(foxModel, animations[currentAnimation], (int)animFrame);
    }

    // Update model animation
    UpdateModelAnimation(foxModel, animations[currentAnimation], (int)animFrame);
}

// Function to unload animations and model
void UnloadFoxAnimations() {
    UnloadModel(foxModel);
    if (animCount > 0) UnloadModelAnimations(animations, animCount);
}
