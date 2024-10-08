//program: rkhosa.cpp
//author:  Rajveer Khosa
//date:    October 8, 2024
//
//This program includes:
//	implementation of jumping mechanism

#include "jump.h"

void handleJump(Vec &pos, Vec &vel, bool &isJumping) {
    if (isJumping) {
        // Apply upward velocity if jump is initiated
        pos[1] += vel[1];

        // Apply gravity to bring the character down
        vel[1] += gravity;
/*
        // Check if the player has landed
        if (pos[1] <= 0.0) {
            pos[1] = 0.0;   // Keep player on the ground
            isJumping = false;
            vel[1] = 0.0;   // Reset vertical velocity
        }
       
*/
    }
}

void initiateJump(Vec &vel, bool &isJumping) {
    if (!isJumping) {
        vel[1] = 5.0;  // Initial upward velocity
        isJumping = true;
    }   
}
