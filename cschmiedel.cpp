#include "cschmiedel.h"

HealthBar::HealthBar(float maxHealth, float x, float y, float width, float height) 
    : maxHealth(maxHealth), currentHealth(maxHealth), x(x), y(y), width(width), height(height) {}

void HealthBar::setHealth(float health) {
    currentHealth = (health > maxHealth) ? maxHealth : ((health < 0) ? 0 : health);
}

void HealthBar::draw() {
    float healthRatio = currentHealth / maxHealth;
    
    // Draw the background (gray)
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_QUADS);
        glVertex2f(x, y); 
        glVertex2f(x + width, y); 
        glVertex2f(x + width, y + height);
        glVertex2f(x, y + height);
    glEnd();

    // Draw the current health (green)
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
        glVertex2f(x, y); 
        glVertex2f(x + width * healthRatio, y); 
        glVertex2f(x + width * healthRatio, y + height);
        glVertex2f(x, y + height);
    glEnd();
}

float HealthBar::getHealth() const {
    return currentHealth;
}


bool HealthBar::isGameOver() const {
    return currentHealth <= 0;
}


