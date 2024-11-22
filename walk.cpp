//3350
//program: walk.cpp
//author:  Gordon Griesel
//date:    summer 2017 - 2018
//
//Modified: [Your Name] - [Date]
//Added level design, collision detection, and basic physics engine.
//
//Walk cycle using a sprite sheet.
//images courtesy: http://games.ucla.edu/resource/walk-cycles/
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "fonts.h"
#include <map>
#include "rkhosa.h"

using namespace std;

bool checkCollision(float newX, float newY);


//defined types
//typedef double Flt;
typedef double Vec[3];
typedef Flt Matrix[4][4];

//macros
#define rnd() (((double)rand())/(double)RAND_MAX)
#define random(a) (rand()%a)
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b) ((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \\
                      (c)[1]=(a)[1]-(b)[1]; \\
                      (c)[2]=(a)[2]-(b)[2]

//constants
const float timeslice = 1.0f;
const float gravity = -0.2f;
const float friction = 0.9f;
const float jumpStrength = 5.0f;
#define ALPHA 1

// Tile map dimensions
const int MAP_WIDTH = 20;
const int MAP_HEIGHT = 15;
int tileMap[MAP_HEIGHT][MAP_WIDTH] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

Level lev;

class Image {
public:
    int width, height;
    unsigned char *data;
    ~Image() { delete [] data; }
    Image(const char *fname) {
        if (fname[0] == '\0')
            return;
        char name[40];
        strcpy(name, fname);
        int slen = strlen(name);
        name[slen-4] = '\0';
        char ppmname[80];
        sprintf(ppmname,"%s.ppm", name);
        char ts[100];
        sprintf(ts, "convert %s %s", fname, ppmname);
        system(ts);
        // Load image data...
    }
};

class Player {
public:
    Vec pos, vel;
    bool onGround;

    Player() {
        pos[0] = 5.0; pos[1] = 5.0; pos[2] = 0.0;
        vel[0] = vel[1] = vel[2] = 0.0;
        onGround = false;
    }

    void applyPhysics() {
        if (!onGround) {
            vel[1] += gravity;
        } else {
            vel[0] *= friction;
        }
    }

    void jump() {
        if (onGround) {
            vel[1] = jumpStrength;
            onGround = false;
        }
    }

    void move(float xInput) {
        vel[0] += xInput;
        pos[0] += vel[0];
        pos[1] += vel[1];

        if (checkCollision(pos[0], pos[1])) {
            // Handle collision response
            pos[0] -= vel[0];
            pos[1] -= vel[1];
            vel[0] = 0;
            vel[1] = 0;
            onGround = true;
        }
    }
};

// Player instance
Player player;

struct GL_STRUCT
{
	GL_STRUCT()
	{
		walk = true;
		xres = 800;
		yres = 600;
		delay = 1000/60;
		camera[0] = 0.0f;
		camera[1] = 0.0f;
		camera[2] = -1.0f;
		walkFrame = 0;
	
	}
	bool walk;
	int xres;
	int yres;
	map<int, bool> keys;
	int delay;
	float camera[3];
	int walkFrame;
};

GL_STRUCT gl;

/*
void physics(void) {
    if (gl.walk || gl.keys[XK_Right] || gl.keys[XK_Left]) {
        // Character is walking...
        // When time is up, advance the frame.
        timers.recordTime(&timers.timeCurrent);
        double timeSpan = timers.timeDiff(&timers.walkTime, &timers.timeCurrent);
        if (timeSpan > gl.delay) {
            // Advance the frame
            ++gl.walkFrame;
            if (gl.walkFrame >= 16)
                gl.walkFrame -= 16;
            timers.recordTime(&timers.walkTime);
        }
        for (int i = 0; i < 20; i++) {
            if (gl.keys[XK_Left]) {
                gl.box[i][0] += 1.0 * (0.05 / gl.delay);
                if (gl.box[i][0] > gl.xres + 10.0)
                    gl.box[i][0] -= gl.xres + 10.0;
                gl.camera[0] -= 2.0 / lev.tilesize[0] * (0.05 / gl.delay);
                if (gl.camera[0] < 0.0)
                    gl.camera[0] = 0.0;
            } else {
                gl.box[i][0] -= 1.0 * (0.05 / gl.delay);
                if (gl.box[i][0] < -10.0)
                    gl.box[i][0] += gl.xres + 10.0;
                gl.camera[0] += 2.0 / lev.tilesize[0] * (0.05 / gl.delay);
                if (gl.camera[0] < 0.0)
                    gl.camera[0] = 0.0;
            }
        }
    }
}



// Function to check for collision
bool checkCollision(float newX, float newY) {
    int tileX = (int)newX;
    int tileY = (int)newY;
    if (tileX < 0 || tileX >= MAP_WIDTH || tileY < 0 || tileY >= MAP_HEIGHT)
        return true;
    return tileMap[tileY][tileX] != 0;
}

// Rendering and game loop
void render() {
    // Clear screen
    // Render player
    renderTileMap();
    // Draw player (e.g., using OpenGL)

    int tx = lev.tilesize[0];
    int ty = lev.tilesize[1];
    Flt dd = lev.ftsz[0];
    Flt offy = lev.tile_base;
    int ncols_to_render = gl.xres / lev.tilesize[0] + 2;
    int col = (int)(gl.camera[0] / dd);
    col = col % lev.ncols;
    //Partial tile offset must be determined here.
    //The leftmost tile might be partially off-screen.
    //cdd: camera position in terms of tiles.
    Flt cdd = gl.camera[0] / dd;
    //flo: just the integer portion
    Flt flo = floor(cdd);
    //dec: just the decimal portion
    Flt dec = (cdd - flo);
    //offx: the offset to the left of the screen to start drawing tiles
    Flt offx = -dec * dd;
    //Log("gl.camera[0]: %lf   offx: %lf\n",gl.camera[0],offx);
    for (int j=0; j<ncols_to_render; j++) {
        int row = lev.nrows-1;
        for (int i=0; i<lev.nrows; i++) {
            if (lev.arr[row][col] == 'w') {
                glColor3f(0.8, 0.8, 0.6);
                glPushMatrix();
                //put tile in its place
                glTranslated((Flt)j*dd+offx, (Flt)i*lev.ftsz[1]+offy, 0);
                glBegin(GL_QUADS);
                    glVertex2i( 0,  0);
                    glVertex2i( 0, ty);
                    glVertex2i(tx, ty);
                    glVertex2i(tx,  0);
                glEnd();
                glPopMatrix();
            }
            if (lev.arr[row][col] == 'b') {
                glColor3f(0.9, 0.2, 0.2);
                glPushMatrix();
                glTranslated((Flt)j*dd+offx, (Flt)i*lev.ftsz[1]+offy, 0);
                glBegin(GL_QUADS);
                    glVertex2i( 0,  0);
                    glVertex2i( 0, ty);
                    glVertex2i(tx, ty);
                    glVertex2i(tx,  0);
                glEnd();
                glPopMatrix();
            }
            --row;
        }
        col = (col+1) % lev.ncols;
    }
    glColor3f(1.0, 1.0, 0.1);
    glPushMatrix();

}



*/

// Function to render the tile map
void renderTileMap() 
{
	float z = -1.0f;
	float cell_size = gl.xres / (float)MAP_WIDTH;
    for (int y = 0; y < MAP_HEIGHT; y++) 
	{
        for (int x = 0; x < MAP_WIDTH; x++) 
		{
            if (tileMap[y][x] == 1) 
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
			else if (tileMap[y][x] == 2) 
			{
                // Render obstacle (e.g., using OpenGL)
            }
        }
    }
}

void render()
{
	renderTileMap();
}

void gameLoop(Display* display, Window win) {

    while (1) 
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0, gl.xres, 0.0, gl.yres, 1, -1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		

		
        XEvent xev;
        XNextEvent(display, &xev);

        if (xev.type == Expose) 
		{
            glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


			// Handle input
			// Apply physics
			//player.applyPhysics();
			//player.move(0.1f); // Example movement
			// Render
			render();
			// Delay or sleep for frame rate control


            glXSwapBuffers(display, win);
        } 
		else if (xev.type == KeyPress) 
		{
            break;
        }
    }

}

// Function to create an OpenGL context
GLXContext createGLContext(Display* display, Window win) {
    
    //  To stop error
    (void)win; 
    static int visualAttribs[] = { None };
    int numConfigs;
    GLXFBConfig* fbConfigs = glXChooseFBConfig(display, DefaultScreen(display), visualAttribs, &numConfigs);
    if (!fbConfigs) {
        fprintf(stderr, "Failed to get framebuffer config\n");
        exit(1);
    }

    GLXContext context = glXCreateNewContext(display, fbConfigs[0], GLX_RGBA_TYPE, NULL, True);
    if (!context) {
        fprintf(stderr, "Failed to create OpenGL context\n");
        exit(1);
    }

    XFree(fbConfigs);
    return context;
}

// Main function
int main() {
    Display* display = XOpenDisplay(NULL);
    if (!display) {
        fprintf(stderr, "Failed to open X display\n");
        return 1;
    }

    Window root = DefaultRootWindow(display);

    XSetWindowAttributes swa;
    swa.event_mask = ExposureMask | KeyPressMask;

    Window win = XCreateWindow(
        display, root,
        0, 0, 800, 600, 0,
        CopyFromParent, InputOutput,
        CopyFromParent, CWEventMask,
        &swa
    );

    XMapWindow(display, win);
    XStoreName(display, win, "OpenGL with GLX");

    GLXContext context = createGLContext(display, win);
    glXMakeCurrent(display, win, context);

    // Game loop
	gameLoop(display, win);

    glXMakeCurrent(display, None, NULL);
    glXDestroyContext(display, context);
    XDestroyWindow(display, win);
    XCloseDisplay(display);

    return 0;
}

