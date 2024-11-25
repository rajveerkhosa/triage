// rkhosa
#ifndef LEVEL_H
#define LEVEL_H

#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>

//defined types
typedef double Vec[3];
typedef float Matrix[4][4];

//macros
#define ESC 0xff1b	//scape
#define rnd() (((double)rand())/(double)RAND_MAX)
#define random(a) (rand()%a)
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b) ((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \\
                      (c)[1]=(a)[1]-(b)[1]; \\
                      (c)[2]=(a)[2]-(b)[2]

#define MAX_WIDTH  20
#define MAX_HEIGHT 15
#define MAX_LEVELS  1

// COLLISION CODES
#define MAP_COLLITION_NONE 0
#define MAP_COLLITION_X 1
#define MAP_COLLITION_Y 2

#define ALPHA 1
//constants
const float timeslice = 1.0f;
const float gravity = -0.2f;
const float friction = 0.9f;
const float jumpStrength = 5.0f;

typedef float Flt;

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

void abortGame(const char *msg);

class Image 
{
public:
    int width, height, depth;
    unsigned char *data;
	GLuint id;
	
    Image(const char *fname, int width, int height, int depth) ;

    ~Image();

	void free();
	
		// load a texture into GPU memory
	GLuint loadTexture(void* pixels, int w, int h, int components);
	
	// load a texture into main memory amd then into GPU memory
	GLuint loadTexture(const char* filename, int w, int h, int components);

	
	void set();
	
	void unset();
};

class Level 
{
public:
    Level();

	int operator() (int x, int y);
	void render(int w, int h);
 	int current_level;
	void loadLevel();
    int a[MAX_HEIGHT][MAX_WIDTH];

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
	int walkFrame;
	int energy;
	int lives;
	Image *sprite;

    Player();
	
	// should be called only one time
	void init();
	
	void setPos(float x, float y);

    void applyPhysics();

    void jump();

    void move(float xInput, Level &L);
	
	// update animation frame
	void nextFrame();

	void render(int screenW, int screenH);
};
#endif // LEVEL_H


