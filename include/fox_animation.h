#ifndef FOX_ANIMATIONS_H
#define FOX_ANIMATIONS_H

#include "raylib.h"

// Animation globals
extern Model foxModel;
extern ModelAnimation *animations;
extern int animCount;
extern float animFrame;
extern int currentAnimation;
extern int idleAnimation;
extern int walkAnimation;
extern int runAnimation;

// Functions
void LoadFoxAnimations(const char *modelPath);
void UpdateFoxAnimation(float deltaTime, bool isMoving, bool isRunning);
void UnloadFoxAnimations();

#endif // FOX_ANIMATIONS_H
