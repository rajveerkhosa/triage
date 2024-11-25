// Level.h
#ifndef LEVEL_H
#define LEVEL_H

#include <memory.h>
#include "constants.h"
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>



static 	int tileMap[MAX_LEVELS][MAX_HEIGHT][MAX_WIDTH] = {
	{
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
	}
};


class Level 
{
public:
    Level();

	int operator() (int x, int y);
	void render(int w, int h);
 	int current_level;
	void loadLevel();
    int a[MAX_WIDTH][MAX_HEIGHT];

	// return the collision code (0, 1, 2, 3)
	int checkCollision(float oldX, float oldY, float newX, float newY);
	
	int curr_level;
	
	// original player position in the map´
	int srcPlayerX;
	int srcPlayerY;

};


class Player 
{
public:
    Vec pos, vel;
    bool onGround;

    Player() 
	{
        pos[0] = 0.0; pos[1] = 0.0; pos[2] = 0.0;
        vel[0] = vel[1] = vel[2] = 0.0;
        onGround = true;
    }
	
	void setPos(float x, float y)
	{
		pos[0] = x;
		pos[1] = y;
	}

    void applyPhysics() 
	{
        if (!onGround) 
		{
            vel[1] += gravity;
        } 
		else 
		{
			// friction is used to de-accelerate
            vel[0] *= friction;
        }
    }

    void jump() 
	{
        if (onGround) 
		{
            vel[1] = jumpStrength;
            onGround = false;
        }
    }

    void move(float xInput, Level &L) 
	{
        vel[0] += xInput;
		applyPhysics();

        int code = L.checkCollision(pos[0], pos[1], pos[0]+vel[0], pos[1]+vel[1]);
		if (code == MAP_COLLITION_NONE)
		{
			pos[0] += vel[0];
			pos[1] += vel[1];
		}
		else 
		{
			if (code == MAP_COLLITION_X)
			{
				pos[1] += vel[1];
				vel[0] = 0;
			}
			if (code == MAP_COLLITION_Y)
			{
				pos[0] += vel[0];
				vel[1] = 0;
				onGround = true;
			}
		}
    }
};

#endif // LEVEL_H


