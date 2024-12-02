//3350
//program: walk.cpp
//author:  Gordon Griesel
//date:    summer 2017 - 2018
//
//Modified: Rajveer Khosa - November 2024
//Added level design, collision detection, and basic physics engine.
//
//Walk cycle using a sprite sheet.
//images courtesy: http://games.ucla.edu/resource/walk-cycles/
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <map>
#include <vector>
#ifdef LINUX_X11_code
#include "fonts.h"
#endif
#include "rkhosa.h"
#include <string>

using namespace std;

//  Defines game states to control flow
enum
{
	Start_Screen = 0,
	Menu_Screen = 1,
	Paused_Screen = 2,
	Playing_Screen = 3,
};

// opengl status
struct GL_STRUCT
{
	GL_STRUCT()
	{
        //  Tracks the current game state
		play_mode = Start_Screen;
		
        //  Screen resolution
        xres = 800;
		yres = 600;
#ifdef LINUX_X11_code
		delay = 100;
		projectile_delay = delay * 5;
		player_projectile_delay = delay * 3;
#else
		delay = 1000;   //  Controls frame rate
		projectile_delay = delay * 5000;
		player_projectile_delay = delay * 1000;
#endif
		camera[0] = 0.0f;   //  Virtual camera position for scrolling
		camera[1] = 0.0f;
		camera[2] = -1.0f;
		walkFrame = 0;  //  Current animation frame 
		generateProjectile = false; //Tracks if a new projectile should be generated
		space_pending = false;
	}
	int play_mode;
	bool space_pending;
	int xres;
	int yres;
	map<int, bool> keys;
	int delay;
	int projectile_delay;
	int player_projectile_delay;
	float camera[3];
	int walkFrame;
	bool generateProjectile;
};

//  Predefines UI textures for the game
SpriteInfo text_items[8] = {
	{NULL,128, 64, 2, 1, "images/lives128x64x4.raw"},
	{NULL,128, 64, 2, 1, "images/score128x64x4.raw"},
	{NULL,128, 64, 2, 1, "images/health128x64x4.raw"},
	{NULL,128, 64, 2, 1, "images/level128x64x4.raw"},
	{NULL,512, 64, 2, 1, "images/numbers512x64x4.raw"},
	{NULL,512, 128, 16, 8, "images/GAMEOVER512x128x4.raw"},
	{NULL,800, 600, 16, 8, "images/paused_screen.raw"},
	{NULL,800, 600, 16, 8, "images/start_screen.raw"},
};



// Current Level
Level lev;

// Player instance
Player player;

// opengl instance
GL_STRUCT gl;

//  Stores interactive objects
vector<Sprite> sprites;

//  Stores projectiles (player or enemy)
vector<Projectile> projectiles;

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


struct timespec prevTime, currTime, projectileTime, playerProjectileTime;

//  Calculates the difference between two timestamps
//  Used to control frame rates and projectile delays
long calculateTimeDifference(struct timespec start, struct timespec end) 
{ 
	long secondsDiff = end.tv_sec - start.tv_sec; 
	long nanoSecondsDiff = end.tv_nsec - start.tv_nsec; 
	long microSecondsDiff = (secondsDiff * 1000000) + (nanoSecondsDiff / 1000); 
	return microSecondsDiff; 
}

//  Updates physics (e.g., player movement and jumping)
void physics(void)
{
	// Character is walking...
	// When time is up, advance the frame.
#ifdef LINUX_X11_code
	clock_gettime(CLOCK_REALTIME, &currTime);
#else
	timespec_get(&currTime, TIME_UTC);
#endif
	long timeSpan = calculateTimeDifference(projectileTime, currTime);
	if (timeSpan > gl.projectile_delay)
	{
		projectileTime = currTime;

		// generate a new projectile
		gl.generateProjectile = true;
	}

	timeSpan = calculateTimeDifference(playerProjectileTime, currTime);
	if (timeSpan > gl.player_projectile_delay)
	{
		playerProjectileTime = currTime;
		if (gl.space_pending) // player attack
		{
			gl.space_pending = false;
			float x = player.pos[0];
			float y = player.pos[1];

			bool dir = player.lastDir;

			if (dir == 0) // left
				x -= 0.5f * player_half + 0.1f;
			else
				x += 0.5f * player_half + 0.1f;

			// generating player projectile
			Projectile p(x, y, 1, dir, 0);

			// adding it into the vector
			projectiles.push_back(p);
		}
	}

	timeSpan = calculateTimeDifference(prevTime, currTime);

	if (timeSpan > gl.delay)
	{

		for (int i = 0; i < projectiles.size(); i++)
		{
			bool res = projectiles[i].update(2*(4+lev.current_level)*move_speed, lev, player);
			if (res && player.lives > 0)
			{
				projectiles.erase(projectiles.begin() + i);
				--i;
			}
		}

		prevTime = currTime;
		player.nextFrame();
		if (gl.keys[XK_Left])
		{
			//gl.box[i][0] += 1.0 * (0.05 / gl.delay);
			//if (gl.box[i][0] > gl.xres + 10.0)
			//    gl.box[i][0] -= gl.xres + 10.0;
			//gl.camera[0] -= 2.0 / lev.tilesize[0] * (0.05 / gl.delay);
			//if (gl.camera[0] < 0.0)
			//    gl.camera[0] = 0.0;
			player.move(-move_speed, lev);

		}
		else if (gl.keys[XK_Right])
		{
			//gl.box[i][0] -= 1.0 * (0.05 / gl.delay);
			//if (gl.box[i][0] < -10.0)
			//    gl.box[i][0] += gl.xres + 10.0;
			//gl.camera[0] += 2.0 / lev.tilesize[0] * (0.05 / gl.delay);
			//if (gl.camera[0] < 0.0)
			//    gl.camera[0] = 0.0;
			player.move(+move_speed, lev);
		}
		else
		{
			player.move(0.0f, lev);
		}
	}
	if (gl.keys[XK_Up]) 
	{
		//printf("jump\n"); fflush(stdout);
		player.jump();
	}
	//printf("%.2f %.2f\r", player.pos[0], player.pos[1]);
}


void render_texture(int screenW, int screenH, int x, int y, int index)
{
	text_items[index].texture->set();

	float cell_size = screenW / (float)MAX_WIDTH;
	float w = cell_size * text_items[index].cell_width;
	float h = cell_size * text_items[index].cell_height;
	if (index >= 6)
	{
		w = 800;
		h = 600;
	}
	float X = x * cell_size;
	float Y = y * cell_size;
	glColor3f(1, 1, 1);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 1); glVertex2f(X, Y);
		glTexCoord2f(1, 1); glVertex2f(X + w, Y);
		glTexCoord2f(1, 0); glVertex2f(X + w, Y + h);
		glTexCoord2f(0, 0); glVertex2f(X, Y + h);
	glEnd();
	glDisable(GL_BLEND);
}

void drawNumber(int x, int y, int number)
{
	// activating the texture
	text_items[4].texture->set();

	int digits[10];
	int s = number;
	int length = 0;

	// computing number of digits
	while (s)
	{
		digits[length] = s % 10;
		length++;
		s /= 10;
	}
	if (length == 0)
	{
		length = 1;
		digits[0] = 0;
	}

	glColor3f(1, 1, 1);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// display digit by digit
	int L = 20;
	glBegin(GL_QUADS);
	for (int i = length - 1; i >= 0; i--, x+=L)
	{
		int d = digits[i];
		glTexCoord2f(d * 51.2f / 512.0f, 1);
		glVertex3i(x, y, 0);

		glTexCoord2f((d + 1) * 51.2f / 512.0f, 1);
		glVertex3i(x + L, y, 0);

		glTexCoord2f((d + 1) * 51.2f / 512.0f, 0);
		glVertex3i(x + L, y + L, 0);

		glTexCoord2f(d * 51.2f / 512.0f, 0);
		glVertex3i(x, y + L, 0);
	}
	glEnd();
	glDisable(GL_BLEND);
}

//  Draws the current game frame
void render()
{
	// Clear screen

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    //  2D orthographic projection
	glOrtho(0.0, gl.xres, 0.0, gl.yres, 1, -1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    //  Depending on play_mode, it renders the start screen, 
    //  pause screen, or active gam
	if (gl.play_mode == Start_Screen)
	{
		if (text_items[7].texture == NULL)
		{
			text_items[7].texture = new Image(text_items[7].filelame, text_items[7].im_width, text_items[7].im_height, 3);
			if (text_items[7].texture->id == 0)
				abortGame((string("Error loading ") + string(text_items[7].filelame)).c_str());
		}
		render_texture(gl.xres, gl.yres, 0, 0, 7);

		return;
	}
	if (gl.play_mode == Paused_Screen)
	{
		// draw lives
		if (text_items[6].texture == NULL)
		{
			text_items[6].texture = new Image(text_items[6].filelame, text_items[6].im_width, text_items[6].im_height, 3);
			if (text_items[6].texture->id == 0)
				abortGame((string("Error loading ") + string(text_items[6].filelame)).c_str());
		}
		render_texture(gl.xres, gl.yres, 0, 0, 6);

		return;
	}

	



	lev.render(gl.xres, gl.yres);
	for (unsigned int i = 0; i < sprites.size(); i++)
	{
		if (sprites[i].spriteIndex == DATA_SPIKES)
		{
			if (gl.generateProjectile && player.lives)
			{
				float x = sprites[i].x;
				float y = sprites[i].y;

				bool dir;
				if (lev(sprites[i].x + 1, sprites[i].y) == CELL_WALL)
					dir = 0;
				else if (lev(sprites[i].x - 1, sprites[i].y) == CELL_WALL)
					dir = 1;
				else
					dir = ((rand() & 1) == 1);

				if (dir == 0) // left
					x -= Sprite::data[sprites[i].spriteIndex].cell_width + 0.1f;
				else
					x += Sprite::data[sprites[i].spriteIndex].cell_width + 0.1f;

				// generating enemy projectile
				Projectile p( x,  y, 1, dir, 1);

				// adding it into the vector
				projectiles.push_back(p);
			}
		}
		sprites[i].render(gl.xres, gl.yres);
	}
	gl.generateProjectile = false;

	for (unsigned int i = 0; i < projectiles.size(); i++)
		projectiles[i].render(gl.xres, gl.yres);

	player.render(gl.xres, gl.yres);

	if (text_items[4].texture == NULL)
	{
		text_items[4].texture = new Image(text_items[4].filelame, text_items[4].im_width, text_items[4].im_height, 4);
		if (text_items[4].texture->id == 0)
			abortGame((string("Error loading ") + string(text_items[4].filelame)).c_str());
	}


	// draw lives
	if (text_items[0].texture == NULL)
	{
		text_items[0].texture = new Image(text_items[0].filelame, text_items[0].im_width, text_items[0].im_height, 4);
		if (text_items[0].texture->id == 0)
			abortGame((string("Error loading ") + string(text_items[0].filelame)).c_str());
	}
	render_texture(gl.xres, gl.yres, 1, 0, 0);
	drawNumber(120, 10, player.lives);

	// score
	if (text_items[1].texture == NULL)
	{
		text_items[1].texture = new Image(text_items[1].filelame, text_items[1].im_width, text_items[1].im_height, 4);
		if (text_items[1].texture->id == 0)
			abortGame((string("Error loading ") + string(text_items[1].filelame)).c_str());
	}
	render_texture(gl.xres, gl.yres, 4, 0, 1);
	drawNumber(250, 10, player.score);

	// health
	if (text_items[2].texture == NULL)
	{
		text_items[2].texture = new Image(text_items[2].filelame, text_items[2].im_width, text_items[2].im_height, 4);
		if (text_items[2].texture->id == 0)
			abortGame((string("Error loading ") + string(text_items[2].filelame)).c_str());
	}
	render_texture(gl.xres, gl.yres, 11, 0, 2);
	drawNumber(550, 10, player.energy);

	// level
	if (text_items[3].texture == NULL)
	{
		text_items[3].texture = new Image(text_items[3].filelame, text_items[3].im_width, text_items[3].im_height, 4);
		if (text_items[3].texture->id == 0)
			abortGame((string("Error loading ") + string(text_items[3].filelame)).c_str());
	}
	render_texture(gl.xres, gl.yres, 16, 0, 3);
	drawNumber(750, 10, lev.current_level);

	if (player.lives == 0)
	{
		if (text_items[5].texture == NULL)
		{
			text_items[5].texture = new Image(text_items[5].filelame, text_items[5].im_width, text_items[5].im_height, 4);
			if (text_items[5].texture->id == 0)
				abortGame((string("Error loading ") + string(text_items[5].filelame)).c_str());
		}
		// game over message
		render_texture(gl.xres, gl.yres, 2, 5, 5);
	}

}

void nextLevel()
{
	//Init all
#ifdef LINUX_X11_code
	clock_gettime(CLOCK_REALTIME, &prevTime);
#else
	timespec_get(&prevTime, TIME_UTC);
#endif
	projectileTime = prevTime;
	playerProjectileTime = projectileTime;

	sprites.clear();
	player.init();
	projectiles.clear();
	gl.space_pending = false;
	// more difficult while the level increases
	gl.projectile_delay = gl.delay * (6 - lev.current_level) * 1000;


	lev.loadLevel(sprites);
	player.init();
	player.setPos((float)lev.srcPlayerX, (float)lev.srcPlayerY);
}

void update()
{
	physics();
	for (unsigned int i = 0; i < sprites.size(); i++)
	{
		if (lev(sprites[i].x, sprites[i].y) == CELL_EMPTY) // sprite was killed
		{
			sprites.erase(sprites.begin() + i);
			i--;
			continue;
		}
		if (sprites[i].collide(player))
		{
			float a = sprites[i].collide_area(player);
			if (a > 0.7f)
			{
				if (sprites[i].spriteIndex == DATA_COIN)
				{
					player.score+=10;
					sprites.erase(sprites.begin() + i);
					break;
				}
				if (sprites[i].spriteIndex == DATA_DOOR)
				{
					if (a > 0.9f)
					{
						printf("level completed\n", lev.current_level);
						nextLevel();
						break;
					}
				}
				else
				{
					player.energy -= ENEMY_ENERGY_DAMAGE;
					if (player.energy <= 0)
					{
						player.lives--;
						if (player.lives > 0)
							player.energy = 100;
						else
							player.energy = 0;
					}
					break;
				}
			}
		}
	}
}



#ifdef LINUX_X11_code
//  Implements the main game loop
void gameLoop(Display* display, Window win) 
{

    while (1) 
	{
		
		if (gl.play_mode == Playing_Screen && player.lives > 0)
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
				gl.keys[key] = true;

				if (key == 32 && gl.play_mode == Playing_Screen)
					gl.space_pending = true;
				else if (key == 'q' || key == 'Q')
					exit(1);
				else if (key == ESC)
				{
					if (gl.play_mode == Start_Screen)
					{
						lev.current_level = -1;
						nextLevel();
						gl.play_mode = Playing_Screen;
					}
					else if (gl.play_mode == Paused_Screen)
					{
						gl.play_mode = Playing_Screen;
					}
					else if (gl.play_mode == Playing_Screen)
					{
						if (player.lives == 0)
							gl.play_mode = Start_Screen;
						else
							gl.play_mode = Paused_Screen;
					}
				}
			}
			else if (xev.type == KeyRelease) 
			{
				KeySym key = XLookupKeysym(&xev.xkey, 0); 
				gl.keys[key] = false;
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
int main(int argc, char** argv)
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


	if (argc > 1)
	{
		int jumpTo = argv[1][0] - '1';
		if (jumpTo >= 0 && jumpTo < 5)
			lev.current_level = jumpTo - 1;
		else
			printf("Invalid cheat code \"%s\": please, select a level between 1 and 5\n", argv[1]);
	}



    // Game loop
	gameLoop(display, win);

	// destroying opengl and window
    glXMakeCurrent(display, None, NULL);
    glXDestroyContext(display, context);
    XDestroyWindow(display, win);
    XCloseDisplay(display);

    return 0;
}

#else



//function to handle keyboard input
void keyboard(unsigned char c, int x, int y) 
{
	gl.keys[c] = true;
	if (c == 32 && gl.play_mode == Playing_Screen)
		gl.space_pending = true;
	else if (c == 'q' || c == 'Q')
		exit(1);

	if (c == ESC)
	{
		if (gl.play_mode == Start_Screen)
		{
			lev.current_level = -1;
			nextLevel();
			gl.play_mode = Playing_Screen;
		}
		else if (gl.play_mode == Paused_Screen)
		{
			gl.play_mode = Playing_Screen;
		}
		else if (gl.play_mode == Playing_Screen)
		{
			if (player.lives == 0)
				gl.play_mode = Start_Screen;
			else
				gl.play_mode = Paused_Screen;
		}
	}

}

//function to handle key up
void keyboardUp(unsigned char c, int x, int y) 
{
	gl.keys[c] = false;
}

void specialupKey(int key, int x, int y) 
{
	gl.keys[key] = false;
}

//function to handle special key input
void specilaKey(int key, int x, int y)
{
	gl.keys[key] = true;
	if (gl.keys[ESC])
		exit(1);
}

void display(void)
{
	if (gl.play_mode == Playing_Screen && player.lives > 0)
		update();
	render();
	glutSwapBuffers();
	Sleep(1000 / 10);
}


// Main function
int main(int argc, char **argv)
{
	//set flut window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(1024, 768);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Game");

	if (argc > 1)
	{
		int jumpTo = argv[1][0] - '1';
		if (jumpTo >= 0 && jumpTo < 5)
			lev.current_level = jumpTo - 1;
		else
			printf("Invalid cheat code \"%s\": please, select a level between 1 and 5\n", argv[1]);
	}

	//set glut functions
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardUp);
	glutSpecialFunc(specilaKey);
	glutSpecialUpFunc(specialupKey);
	glutIdleFunc(display);
	glutMainLoop();


	
	return 0;
}


#endif
