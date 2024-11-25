//  Rajveer Khosa
#include "rkhosa.h"


void abortGame(const char *msg)
{
	printf("%s\nPress enter to finish-->", msg);
	getchar();
	exit(1);
}

////////////////////////// IMAGE /////////////////////


void Image::free()
{
	if (data)
	{
		delete [] data; 
	}
	if (id>0)
	{
		glDeleteTextures(1, &id);
	}
}

Image::~Image() 
{ 
	free();
}
	
	// load a texture into GPU memory
GLuint Image::loadTexture(void* pixels, int w, int h, int components)
{
	// generating a texture identifier
	GLuint textureID;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// linear texture filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// when the texture is outside [0,1], we just repeat the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// modulate means combine the image with the current color
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	if (components == 3) 
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);  
	else if (components == 4) 
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);  

	return textureID;
}

// load a texture into main memory amd then into GPU memory
GLuint Image::loadTexture(const char* filename, int w, int h, int components)
{
	FILE* f = NULL;
	f = fopen(filename, "rb");
	if (f == NULL)
	{
		printf("File not found: %s\n", filename);
		return 0;
	}
	void* p = (void*)malloc(w * h * components);
	if (fread(p, components, unsigned(w * h), f) != unsigned(w*h))
	{
		printf("Error reading %d bytes from: %s\n", w*h*components, filename);
		fclose(f);
		return 0;
	}
	fclose(f);
	GLuint ret = loadTexture(p, w, h, components);
	::free(p);
	return ret;
}

Image::Image(const char *fname, int width, int height, int depth) 
{
	data = NULL;
	id = loadTexture(fname, width, height, depth);
	if (id == 0)
		return;
}

void Image::set()
{
	if (id)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, id);
	}
	else
	{
		abortGame("Error, texture has not been loaded properly and cannot be set!");
	}
}

void Image::unset()
{
	if (id)
		glBindTexture(GL_TEXTURE_2D, 0);
}


//////////////////////////// LEVEL /////////////////////////////////


Level::Level() 
{
	current_level = -1;
	srcPlayerX = 0;
	srcPlayerY = 0;
};

void Level::loadLevel()
{
	current_level = (current_level + 1) % MAX_LEVELS;
	
	srcPlayerX = 0;
	srcPlayerY = 0;
	for (int y=0; y<MAX_HEIGHT; y++) for (int x =0; x<MAX_WIDTH; x++)
	{
		a[y][x] = tileMap[current_level][y][x];
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
	float z = 0.0f;
	float cell_size = w / (float)MAX_WIDTH;
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




/////////////////////// PLAYER /////////////////////////


Player::Player() 
{
	pos[0] = 3.0; pos[1] = 4.0; pos[2] = 0.0;
	vel[0] = vel[1] = vel[2] = 0.0;
	onGround = true;
	walkFrame = 0;
	energy = 100;
	lives = 3;
	sprite = NULL;
}

void Player::setPos(float x, float y)
{
	pos[0] = x;
	pos[1] = y;
}

void Player::nextFrame()
{
	// Advance the frame
	walkFrame++;
	if (walkFrame >= 12)
		walkFrame = 0;
}

void Player::applyPhysics() 
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

void Player::init()
{
	if (sprite == NULL)
	{
		sprite = new Image("images/player1024x128x4.raw", 1024, 128, 4);
		if (sprite->id == 0)
			abortGame("Error loading images/player1024x128x4.raw");
	}
}

void Player::jump() 
{
	if (onGround) 
	{
		vel[1] = jumpStrength;
		onGround = false;
	}
}

void Player::render(int screenW, int)
{
	sprite->set();
	
	float cell_size = screenW / (float)MAX_WIDTH;
	const int nFrames = 12;
	const float deltaX = 1.0f / nFrames;
	float x = pos[0] * cell_size;
	float y = pos[1] * cell_size;

	glColor3f(1,1,1);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_QUADS);
		glTexCoord2f(deltaX*walkFrame, 1); 			glVertex2f(x-cell_size, y-cell_size);
		glTexCoord2f(deltaX*walkFrame + deltaX, 1); glVertex2f(x+cell_size, y-cell_size);
		glTexCoord2f(deltaX*walkFrame + deltaX, 0); glVertex2f(x+cell_size, y+cell_size);
		glTexCoord2f(deltaX*walkFrame, 0); 			glVertex2f(x-cell_size, y+cell_size);
	glEnd();
	glDisable(GL_BLEND);
}

void Player::move(float xInput, Level &L) 
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

