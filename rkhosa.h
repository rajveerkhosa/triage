// rkhosa
#ifndef LEVEL_H
#define LEVEL_H


#include <memory.h>
#include <stdlib.h>
#include <stdio.h>

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <unistd.h>
#define ESC 0xff1b	//scape
//constants
const float gravity = -0.03f;
const float friction = 0.9f;
const float jumpStrength = 0.6f;
const float player_half = 0.8f;
const float move_speed = 0.02f;

#include <algorithm>
#include <vector>

using namespace std;

//defined types
typedef float Vec[3];
typedef float Matrix[4][4];

//macros
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
#define MAX_LEVELS  5

// COLLISION CODES
#define MAP_COLLITION_NONE 0
#define MAP_COLLITION_X 1
#define MAP_COLLITION_Y 2

#define MAX_SPRITES 6
#define BULLET_ENERGY_DAMAGE 25
#define ENEMY_ENERGY_DAMAGE 5

#define ALPHA 1

enum
{
	CELL_EMPTY = 0,
	CELL_WALL = 1,
	CELL_PLAYER = 2,
	CELL_MUSHROOM = 3,
	CELL_STAR = 4,
	CELL_SPIKES = 5,
	CELL_COIN = 6,
	CELL_DOOR = 9,
};

enum
{
	DATA_MUSHROOM = 0,
	DATA_DOOR = 1,
	DATA_STAR = 2,
	DATA_SPIKES = 3,
	DATA_COIN = 4,
	DATA_PROJECTILE = 5
};


typedef float Flt;

static 	int tileMap[MAX_LEVELS][MAX_HEIGHT][MAX_WIDTH] = {
	{
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 3, 0, 3, 0, 3, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 6, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 4, 0, 1, 1, 1, 1, 1, 0, 6, 0, 0, 1},
		{1, 0, 5, 6, 0, 6, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 6, 9, 1},
		{1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1},
		{1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 1, 1, 0, 6, 0, 6, 0, 6, 0, 6, 0, 0, 0, 0, 0, 2, 0, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 6, 0, 0, 0, 1},
		{1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 0, 6, 0, 6, 0, 6, 0, 6, 0, 1, 0, 4, 0, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
	},
	{
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 9, 1},
		{1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 3, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 1},
		{1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1},
		{1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1},
		{1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
	},
		{
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1},
		{1, 9, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 6, 5, 0, 0, 0, 0, 1},
		{1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 2, 0, 1},
		{1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1},
		{1, 1, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
		{1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 6, 1, 1, 0, 0, 3, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 6, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 6, 0, 1},
		{1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1},
		{1, 3, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 1},
		{1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 1, 1, 0, 6, 0, 6, 0, 0, 3, 5, 0, 0, 6, 0, 6, 0, 6, 0, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
	},
		{
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 3, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 6, 0, 0, 6, 0, 0, 6, 0, 0, 6, 0, 0, 6, 0, 9, 1},
		{1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 3, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 1},
		{1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 6, 0, 4, 0, 6, 5, 6, 0, 6, 0, 6, 5, 0, 2, 0, 1},
		{1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 6, 0, 6, 0, 6, 0, 6, 0, 0, 0, 0, 6, 0, 6, 0, 6, 0, 6, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
	},
		{
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 2, 0, 1},
		{1, 9, 6, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 1, 1, 0, 6, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 6, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
		{1, 5, 0, 0, 0, 0, 6, 0, 0, 6, 6, 0, 0, 0, 6, 1, 0, 0, 0, 1},
		{1, 0, 0, 0, 6, 1, 3, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1},
		{1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 4, 4, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 1, 1, 0, 0, 4, 4, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1},
		{1, 3, 3, 1, 1, 3, 3, 1, 1, 1, 1, 1, 1, 3, 3, 1, 1, 3, 3, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
	}
};

class Box2D;
class Sprite;
class Image;
class Level;
class Player;
class Projectile;

class SpriteInfo
{
public:
	Image* texture;
	int im_width;
	int im_height;
	float cell_width;
	float cell_height;
	char filelame[100];


	~SpriteInfo();

};

void abortGame(const char* msg);

class Box2D
{
public:
	float x0;
	float y0;
	float x1;
	float y1;

	Box2D();

	Box2D(float x0, float y0, float x1, float y1);
	
	bool isBox() const;
	
	Box2D Union(const Box2D& b) const;

	bool inside(float x, float y);
	
	float area();
	
	Box2D Inter(const Box2D& b) const;
	
};

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
	Image *backGround;
	int & operator() (int x, int y);
	void render(int w, int h);
 	int current_level;
	void loadLevel(vector<Sprite> &sprites);
    int a[MAX_HEIGHT][MAX_WIDTH];

	// return the collision code (0, 1, 2, 3)
	// int checkCollision(float oldX, float oldY, float newX, float newY, float spr_w, float spr_h);
	bool checkCollision(float oldX, float oldY, float newX, float newY);
	
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
	int lastDir; // 0=left, 1=right
	int score;

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

	void getBox(Box2D& b);


};



class Sprite
{
public:
	float x, y;
	int spriteIndex;

	static SpriteInfo data[MAX_SPRITES];

	Sprite();
	~Sprite();
	virtual void set(float x, float y, int spriteIndex);
	void moveTo(float x, float y);
	float collide_area(Player& p);
	bool collide(Player& p);
	float collide_area(int x, int y);
	virtual void render(int screenW, int screenH);
	void getBox(Box2D& b);
};


class Projectile : public Sprite
{
public:
	float speed; 
	bool dir;		// true = right, false= left
	bool proj_type; // 0=player, 1=enemy

	Projectile(float x, float y, float speed, bool dir, bool proj_type);

	bool update(float deltaTime, Level& level, Player& player);

	void render(int screenW, int screenH) const;
};


#endif // LEVEL_H

