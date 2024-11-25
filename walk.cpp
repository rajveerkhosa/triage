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
#include <map>
#include "fonts.h"
#include "rkhosa.h"

using namespace std;

// opengl status
struct GL_STRUCT
{
	GL_STRUCT()
	{
		walk = true;
		xres = 800;
		yres = 600;
		delay = 1000/30;
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





// level instance
Level lev;

// Player instance
Player player;

// opengl instance
GL_STRUCT gl;

/*





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


struct timespec prevTime, currTime;

long calculateTimeDifference(struct timespec start, struct timespec end) 
{ 
	long secondsDiff = end.tv_sec - start.tv_sec; 
	long nanoSecondsDiff = end.tv_nsec - start.tv_nsec; 
	long microSecondsDiff = (secondsDiff * 1000000) + (nanoSecondsDiff / 1000); 
	return microSecondsDiff; 
}

void physics(void) 
{
	printf(".");
    if (gl.walk || gl.keys[XK_Right] || gl.keys[XK_Left]) 
	{
        // Character is walking...
        // When time is up, advance the frame.
		clock_gettime(CLOCK_REALTIME, &currTime);
        long timeSpan = calculateTimeDifference(prevTime, currTime);
		printf("timeSpan = %ld\n", timeSpan);
        if (timeSpan > gl.delay) 
		{
			player.nextFrame();
        }
		if (gl.keys[32]) // space
		{
			printf("jump\n"); fflush(stdout);
			player.jump();
		}
		if (gl.keys[XK_Left]) 
		{
			//gl.box[i][0] += 1.0 * (0.05 / gl.delay);
			//if (gl.box[i][0] > gl.xres + 10.0)
			//    gl.box[i][0] -= gl.xres + 10.0;
			//gl.camera[0] -= 2.0 / lev.tilesize[0] * (0.05 / gl.delay);
			//if (gl.camera[0] < 0.0)
			//    gl.camera[0] = 0.0;
			player.move(-0.1f, lev);

		} 
		else if (gl.keys[XK_Right]) 
		{
			//gl.box[i][0] -= 1.0 * (0.05 / gl.delay);
			//if (gl.box[i][0] < -10.0)
			//    gl.box[i][0] += gl.xres + 10.0;
			//gl.camera[0] += 2.0 / lev.tilesize[0] * (0.05 / gl.delay);
			//if (gl.camera[0] < 0.0)
			//    gl.camera[0] = 0.0;
			player.move(+0.1f, lev);
		}
		else
		{
			player.move(0.0f, lev);
		}
		printf("%.2f %.2f\r", player.pos[0], player.pos[1]);
    }
}

void render()
{
    // Clear screen
	printf("clear\n"); fflush(stdout);

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


	// Handle input
	// Apply physics
	//player.applyPhysics();
	//player.move(0.1f); // Example movement
	// Render
	printf("lev.render\n"); fflush(stdout);
	lev.render(gl.xres, gl.yres);
	printf("player.render\n"); fflush(stdout);
	player.render(gl.xres, gl.yres);
	printf("end render\n"); fflush(stdout);
}

void update()
{
	physics();
}

void gameLoop(Display* display, Window win) 
{

    while (1) 
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0, gl.xres, 0.0, gl.yres, 1, -1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		update();
		render();
		glXSwapBuffers(display, win);
		
		
		if (XPending(display))
		{
			XEvent xev;
			XNextEvent(display, &xev);

			if (xev.type == Expose) 
			{
				// Delay or sleep for frame rate control
			} 
			else if (xev.type == KeyPress) 
			{
				KeySym key = XLookupKeysym(&xev.xkey, 0); 
				if (key != NoSymbol) 
				{ 
					printf("Key pressed: %s, %lx\n", XKeysymToString(key), key); 
				} 
				else 
				{ 
					printf("Key pressed: unknown\n"); 
				}
				gl.keys[key] = true;
			}
			else if (xev.type == KeyRelease) 
			{
				KeySym key = XLookupKeysym(&xev.xkey, 0); 
				if (key != NoSymbol) 
				{ 
					printf("Key released: %s, %lx\n", XKeysymToString(key), key); 
				} 
				else 
				{ 
					printf("Key released: unknown\n"); 
				}
				gl.keys[key] = false;
				if (key == ESC)	
					break;
			}
		}

    }

}

// Function to create an OpenGL context
GLXContext createGLContext(Display* display) 
{
    static int visualAttribs[] = { None };
    int numConfigs;
    GLXFBConfig* fbConfigs = glXChooseFBConfig(display, DefaultScreen(display), visualAttribs, &numConfigs);
    if (!fbConfigs) 
	{
        fprintf(stderr, "Failed to get framebuffer config\n");
        exit(1);
    }

    GLXContext context = glXCreateNewContext(display, fbConfigs[0], GLX_RGBA_TYPE, NULL, True);
    if (!context) 
	{
        fprintf(stderr, "Failed to create OpenGL context\n");
        exit(1);
    }

    XFree(fbConfigs);
    return context;
}

// Main function
int main() 
{
    Display* display = XOpenDisplay(NULL);
    if (!display) 
	{
        fprintf(stderr, "Failed to open X display\n");
        return 1;
    }

    Window root = DefaultRootWindow(display);

    XSetWindowAttributes swa;
    swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask;

    Window win = XCreateWindow
	(
        display, root,
        0, 0, 800, 600, 0,
        CopyFromParent, InputOutput,
        CopyFromParent, CWEventMask,
        &swa
    );

    XMapWindow(display, win);
    XStoreName(display, win, "Jumping game");

    GLXContext context = createGLContext(display);
    glXMakeCurrent(display, win, context);

	lev.loadLevel();

	player.init();
	
	clock_gettime(CLOCK_REALTIME, &prevTime);
    // Game loop
	gameLoop(display, win);

	// destroying opengl and window
    glXMakeCurrent(display, None, NULL);
    glXDestroyContext(display, context);
    XDestroyWindow(display, win);
    XCloseDisplay(display);

    return 0;
}

