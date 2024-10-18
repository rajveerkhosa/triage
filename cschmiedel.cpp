// cschmiedel.cpp

#include "cschmiedel.h"
#include <GL/gl.h>
#include <GL/glx.h>  // Required for OpenGL on X11
#include <GL/glut.h> // Required for rendering text

// Define global health variables
float currentHealth = 100.0f;
const float maxHealth = 100.0f;
bool gameOver = false;

// Initialize health bar (if needed)
void initializeHealthBar() {
    currentHealth = 100.0f;
    gameOver = false;
}

// Draw the health bar on the screen
void drawHealthBar(int xres, int yres) {
    float barX = 50;
    float barY = yres - 50;
    float barWidth = 200;
    float barHeight = 20;

    float healthPercentage = currentHealth / maxHealth;
    float currentBarWidth = barWidth * healthPercentage;

    // Draw the background of the health bar
    glBegin(GL_QUADS);
        glColor3f(0.5f, 0.5f, 0.5f);  // Grey background
        glVertex2f(barX, barY);
        glVertex2f(barX + barWidth, barY);
        glVertex2f(barX + barWidth, barY + barHeight);
        glVertex2f(barX, barY + barHeight);
    glEnd();

    // Draw the current health
    glBegin(GL_QUADS);
        glColor3f(0.0f, 1.0f, 0.0f);  // Green for health
        glVertex2f(barX, barY);
        glVertex2f(barX + currentBarWidth, barY);
        glVertex2f(barX + currentBarWidth, barY + barHeight);
        glVertex2f(barX, barY + barHeight);
    glEnd();
}

// Deal damage to the health bar
void dealDamage(float amount) {
    currentHealth -= amount;
    if (currentHealth < 0) {
        currentHealth = 0;
        gameOver = true;
    }
}

// Heal the health bar
void heal(float amount) {
    currentHealth += amount;
    if (currentHealth > maxHealth) {
        currentHealth = maxHealth;
    }
}

// Check if the game is over
void checkGameOver() {
    if (currentHealth <= 0) {
        gameOver = true;
    }
}

// Render text on the screen
void renderText(float x, float y, const char *text, float r, float g, float b) {
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    for (const char *c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}
