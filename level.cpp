#include "level.h"

Level::Level() 
{
	current_level = -1;
	srcPlayerX = 0;
	srcPlayerY = 0;
};

void Level::loadLevel()
{
	current_level = (current_level + 1) % MAX_LEVELS;
	memcpy(&a[0][0], &tileMap[current_level][0][0], MAX_WIDTH*MAX_HEIGHT);
	
	srcPlayerX = 0;
	srcPlayerY = 0;
	for (int y=0; y<MAX_HEIGHT; y++) for (int x =0; x<MAX_WIDTH; x++)
	{
		if (a[y][x] == 2)
		{
			srcPlayerX = x;
			srcPlayerY = y;
		}
	}
}

int Level::operator() (int x, int y)
{
	return a[y][x];
}

// Function to check for collision
int Level::checkCollision(float oldX, float oldY, float newX, float newY) 
{
    int tileX = (int)newX;
    int tileY = (int)newY;
	int code = MAP_COLLITION_NONE;
    if (tileX < 0 || tileX >= MAX_WIDTH)
		code |= MAP_COLLITION_X;
	if (tileY < 0 || tileY >= MAX_HEIGHT)
		code |= MAP_COLLITION_Y;
	int srcX = (int)oldX;
	int srcY = (int)oldY;
    if (a[tileY][tileX] == 1)
	{
		if (a[tileY][srcX] == 1)
			code |= MAP_COLLITION_Y;
		if (a[srcY][tileX] == 1)
			code |= MAP_COLLITION_X;
		if (code == MAP_COLLITION_NONE)
			code = MAP_COLLITION_X|MAP_COLLITION_Y;	// passing in diagonal from save position into collision position
	}
	return code;
}

// Function to render the tile map
void Level::render(int w, int) 
{
	float z = -1.0f;
	float cell_size = w / (float)MAP_WIDTH;
    for (int y = 0; y < MAX_HEIGHT; y++) 
	{
        for (int x = 0; x < MAX_WIDTH; x++) 
		{
            if (a[y][x] == 1) 
			{
				glColor3f(1,1,1);
				glBegin(GL_QUADS);
					// Render platform (e.g., using OpenGL)
					glVertex3f(x * cell_size, y * cell_size,  z);
					glVertex3f((x+1) * cell_size, y * cell_size, z);
					glVertex3f((x+1) * cell_size, (y+1) * cell_size, z);
					glVertex3f(x * cell_size, (y+1) * cell_size, z);
				glEnd();

				glColor3f(1,0,0);
				glBegin(GL_LINE_LOOP);
					// Render platform (e.g., using OpenGL)
					glVertex3f(x * cell_size, y * cell_size,  z);
					glVertex3f((x+1) * cell_size, y * cell_size, z);
					glVertex3f((x+1) * cell_size, (y+1) * cell_size, z);
				glVertex3f(x * cell_size, (y+1) * cell_size, z);
				glEnd();
            } 
			else if (a[y][x] == 2) 
			{
                // Render obstacle (e.g., using OpenGL)
            }
        }
    }
}

