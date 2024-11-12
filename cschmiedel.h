#ifndef CSCHMIEDEL_H
#define CSCHMIEDEL_H

#include <GL/gl.h>

class HealthBar {
private:
    float maxHealth;
    float currentHealth;
    float x, y, width, height;

public:
    HealthBar(float maxHealth = 100.0f, float x =650.0f, float y = 550.0f, float width = 200.0f, float height = 20.0f); 
    void setHealth(float health);
    void draw();
    bool isGameOver() const;
    float getHealth() const;
};

#endif // CSCHMIEDEL_H


