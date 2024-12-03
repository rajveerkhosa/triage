//  Rajveer Khosa
#include "rkhosa.h"
#include <math.h>
#include <vector>
#include <string>

void abortGame(const char *msg)
{
	printf("%s\nPress enter to finish-->", msg);
	getchar();
	exit(1);
}


SpriteInfo::~SpriteInfo()
{

	if (texture)
		delete texture;
}

Box2D::Box2D()
{
	x0 = y0 = 0;
	x1 = y1 = -1;
}

Box2D::Box2D(float x0, float y0, float x1, float y1)
{
	this->x0 = x0;
	this->y0 = y0;
	this->x1 = x1;
	this->y1 = y1;
}

bool Box2D::isBox() const
{
	if (x0 >= x1)
		return false;
	if (y0 >= y1)
		return false;
	return true;
}

Box2D Box2D::Union(const Box2D& b) const
{
	float minx, miny, maxx, maxy;
	if (x0 < b.x0)
		minx = x0;
	else
		minx = b.x0;

	if (y0 < b.y0)
		miny = y0;
	else
		miny = b.y0;

	if (x1 > b.x1)
		maxx = x1;
	else
		maxx = b.x1;

	if (y1 > b.y1)
		maxy = y1;
	else
		maxy = b.y1;

	return Box2D(minx, miny, maxx, maxy);
}

bool Box2D::inside(float x, float y)
{
	return x0 <= x && x <= x1 && y0 <= y && y <= y1;
}

float Box2D::area()
{
	if (!isBox())
		return 0.0;
	return (x1 - x0) * (y1 - y0);
}

Box2D Box2D::Inter(const Box2D& b) const
{
	Box2D res(0, 0, -1, -1);
	if (!isBox() || !b.isBox())
		return res;
	if (x1 <= b.x0 || b.x1 <= x0)	// no x intersection
		return res;
	if (y1 <= b.y0 || b.y1 <= y0)	// no y intersection
		return res;
	res.x0 = max(x0, b.x0);
	res.y0 = max(y0, b.y0);

	res.x1 = min(x1, b.x1);
	res.y1 = min(y1, b.y1);
	return res;
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
	backGround = NULL;
};

void Level::loadLevel(vector<Sprite>& sprites)
{
	current_level = (current_level + 1) % MAX_LEVELS;
	if (backGround)
		delete backGround;
	char  name[100];
	sprintf(name, "images/background%d.raw", current_level+1);
	backGround = new Image(name, 800, 600, 3);
	if (backGround->id == 0)
	{
		sprintf(name, "Error loading images/background%d.raw", current_level+1);
		abortGame(name);
	}

	
	srcPlayerX = 0;
	srcPlayerY = 0;
	sprites.clear();

	for (int y=0; y<MAX_HEIGHT; y++) for (int x =0; x<MAX_WIDTH; x++)
	{
		a[MAX_HEIGHT-1-y][x] = tileMap[current_level][y][x];
		if (a[MAX_HEIGHT-1-y][x] == 2)
		{
			srcPlayerX = float(x);
			srcPlayerY = float(MAX_HEIGHT-1-y);
		}
		else if (a[MAX_HEIGHT - 1 - y][x] == CELL_MUSHROOM) // mushroom
		{
			Sprite s;
			s.set((float)x, float(MAX_HEIGHT - 1 - y), DATA_MUSHROOM);
			sprites.push_back(s);
		}
		else if (a[MAX_HEIGHT - 1 - y][x] == CELL_DOOR) // door
		{
			Sprite s;
			s.set((float)x, float(MAX_HEIGHT - 1 - y), DATA_DOOR);
			sprites.push_back(s);
		}
		else if (a[MAX_HEIGHT - 1 - y][x] == CELL_STAR) // door
		{
			Sprite s;
			s.set((float)x, float(MAX_HEIGHT - 1 - y), DATA_STAR);
			sprites.push_back(s);
		}
		else if (a[MAX_HEIGHT - 1 - y][x] == CELL_SPIKES) // door
		{
			Sprite s;
			s.set((float)x, float(MAX_HEIGHT - 1 - y), DATA_SPIKES);
			sprites.push_back(s);
		}
		else if (a[MAX_HEIGHT - 1 - y][x] == CELL_COIN) // door
		{
			Sprite s;
			s.set((float)x, float(MAX_HEIGHT - 1 - y), DATA_COIN);
			sprites.push_back(s);
		}
	}
}

int &Level::operator() (int x, int y)
{
	return a[y][x];
}

// Function to check for collision with map
bool Level::checkCollision(float oldX, float oldY, float newX, float newY) 
{
	// integer bounding box to check in the map
	int minx, miny, maxx, maxy;
	
	if (fabs( newX - float(int(newX))) == 0.5f) // only 2 cells
	{
		minx = int(newX);
		maxx = minx + 1;
	}
	else
	{
		minx = int(newX+0.5f) - 1;
		maxx = minx + 2;

	}
	if (fabs(newY - float(int(newY))) == 0.5f) // only 2 cells
	{
		miny = int(newY);
		maxy = miny + 1;
	}
	else
	{
		miny = int(newY + 0.5f) - 1;
		maxy = miny + 2;
	}

	/*
	minx = int(newX-0.5f);
	maxx = minx + 1;
	miny = int(newY-0.5f);
	maxy = miny + 1;*/

	for (int x = minx; x <= maxx; x++) for (int y = miny; y <= maxy; y++) if (a[y][x] == 1)
	{
		Box2D cell(x - 0.5f, y - 0.5f, x + 0.5f, y + 0.5f);
		Box2D ply(newX - player_half, newY - player_half, newX + player_half, newY + player_half);
		Box2D I = cell.Inter(ply);
		if (I.area() > 0.0f)
			return true;
	}


	return false;
}

// Function to render the tile map
void Level::render(int w, int) 
{

	backGround->set();
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 1); glVertex3f(0, 0, 0);
		glTexCoord2f(1, 1); glVertex3f(800, 0, 0);
		glTexCoord2f(1, 0); glVertex3f(800, 600, 0);
		glTexCoord2f(0, 0); glVertex3f(0, 600, 0);
	glEnd();
	backGround->unset();

	glEnable(GL_BLEND);
	float z = 0.0f;
	float cell_size = w / (float)MAX_WIDTH;
	float r = cell_size;
    for (int y = 0; y < MAX_HEIGHT; y++) 
	{
        for (int x = 0; x < MAX_WIDTH; x++) 
		{
            if (a[y][x] == 1) 
			{
				glColor4f(1,1,1,0.75f);
				glBegin(GL_QUADS);
					// Render platform (e.g., using OpenGL)
					glVertex3f(x * cell_size, y * cell_size, z);
					glVertex3f(x * cell_size + r, y * cell_size, z);
					glVertex3f(x * cell_size + r, y * cell_size + r, z);
					glVertex3f(x * cell_size, y * cell_size + r, z);
				glEnd();

				glColor4f(1,0,0,0.75f);
				glBegin(GL_LINE_LOOP);
					// Render platform (e.g., using OpenGL)
					glVertex3f(x * cell_size, y * cell_size, z);
					glVertex3f(x * cell_size + r, y * cell_size, z);
					glVertex3f(x * cell_size + r, y * cell_size + r, z);
					glVertex3f(x * cell_size, y * cell_size + r, z);
				glEnd();
            } 
			else if (a[y][x] == 2) 
			{
                // Render obstacle (e.g., using OpenGL)
            }
        }
    }
	glDisable(GL_BLEND);
}




/////////////////////// PLAYER /////////////////////////


Player::Player() 
{
	pos[0] = 3.0; pos[1] = 4.0; pos[2] = 0.0;
	vel[0] = vel[1] = vel[2] = 0.0;
	onGround = false;
	walkFrame = 0;
	energy = 100;
	lives = 3;
	sprite = NULL;
	lastDir = 1;
	score = 0;
}

void Player::setPos(float x, float y)
{
	pos[0] = x;
	pos[1] = y;
}

void Player::nextFrame()
{
	// Advance the frame
	if (onGround && vel[0] != 0.0)
	{
		walkFrame++;
		if (walkFrame >= 12)
			walkFrame = 0;
	}
}

void Player::applyPhysics() 
{
	vel[1] += gravity;
	if (!onGround)
	{
	} 
	else 
	{
		// friction is used to de-accelerate
		vel[0] *= friction;
		if (fabs(vel[0]) < 0.5e-2)
			vel[0] = 0;
			
	}

	// quantization of velocity
	//vel[0] = int(vel[0] * 100.0f) / 100.0f;
	//vel[1] = int(vel[1] * 100.0f) / 100.0f;
}

void Player::init()
{
	if (sprite == NULL)
	{
		sprite = new Image("images/player1024x128x4.raw", 1024, 128, 4);
		if (sprite->id == 0)
			abortGame("Error loading images/player1024x128x4.raw");
	}
	vel[0] = vel[1] = vel[2] = 0.0;
	onGround = false;
	walkFrame = 0;
	energy = 100;
	lastDir = 1;
	lives = 3;
	score = 0;
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
	float x = pos[0] * cell_size + 0.5f*cell_size;
	float y = pos[1] * cell_size + 0.5f*cell_size;

	cell_size *= 0.8f;
	glColor3f(1,1,1);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_QUADS);
		if (lastDir==1)
		{
			glTexCoord2f(deltaX*walkFrame, 1); 			glVertex2f(x-cell_size, y-cell_size);
			glTexCoord2f(deltaX*walkFrame + deltaX, 1); glVertex2f(x+cell_size, y-cell_size);
			glTexCoord2f(deltaX*walkFrame + deltaX, 0); glVertex2f(x+cell_size, y+cell_size);
			glTexCoord2f(deltaX*walkFrame, 0); 			glVertex2f(x-cell_size, y+cell_size);
		}
		else
		{
			glTexCoord2f(deltaX*walkFrame + deltaX, 1); glVertex2f(x-cell_size, y-cell_size);
			glTexCoord2f(deltaX*walkFrame, 1); 			glVertex2f(x+cell_size, y-cell_size);
			glTexCoord2f(deltaX*walkFrame, 0); 			glVertex2f(x+cell_size, y+cell_size);
			glTexCoord2f(deltaX*walkFrame + deltaX, 0); glVertex2f(x-cell_size, y+cell_size);
		}
	glEnd();
	glDisable(GL_BLEND);
}

void Player::move(float xInput, Level &L) 
{
	if (onGround)
		vel[0] += xInput;
	else
		vel[0] += xInput*0.6f;

	applyPhysics();
	
	// quantization of position
	//pos[0] = int(pos[0] * 100.0f) / 100.0f;
	//pos[1] = int(pos[1] * 100.0f) / 100.0f;

	//printf("%f %f    \n", pos[0], pos[1]);

	if (vel[0] == 0.0f)
	{
		if (vel[1] == 0.0f)
		{
			// nothing to do
		}
		else if (!L.checkCollision(pos[0], pos[1], pos[0], pos[1] + vel[1])) // do not collide in y axis
		{
			pos[1] += vel[1];
			onGround = false;
		}
		else // collides in y axis
		{
			if (vel[1] < 0.0f)
				onGround = true;
			vel[1] = 0.0f;
			//pos[1] = int(pos[1]) + 0.5f;
		}
	}
	else if (vel[0] != 0.0f) // moving in x axis
	{
		if (vel[1] == 0.0f)
		{
			if (!L.checkCollision(pos[0], pos[1], pos[0] + vel[0], pos[1])) // do not collide in x axis
			{
				pos[0] += vel[0];
			}
			else
			{
				// nothing to do
				//pos[0] = int(pos[0]) + 0.5f;
				vel[0] = 0.0f;
			}
		}
		else if (!L.checkCollision(pos[0], pos[1], pos[0] + vel[0], pos[1] + vel[1])) // do not collide in x,y axis
		{
			pos[0] += vel[0];
			pos[1] += vel[1];
			onGround = false;
		}
		else // collides in x.y axis
		{
			// try to move in y axis first
			if (!L.checkCollision(pos[0], pos[1], pos[0], pos[1] + vel[1])) // do not collide in y axis
			{
				//pos[0] = int(pos[0]) + 0.5f;
				pos[1] += vel[1];
				//vel[1] = 0.0f;
			}
			else
			{
				if (vel[1] < 0.0f)
					onGround = true;
				vel[1] = 0.0f;
				//pos[1] = int(pos[1]) + 0.5f;

				// try to move in x axis 
				if (!L.checkCollision(pos[0], pos[1], pos[0] + vel[0], pos[1])) // do not collide in x axis
					pos[0] += vel[0];
				else
				{
					//pos[0] = int(pos[0]) + 0.5f;
					vel[0] = 0.0f;
				}
			}
		}
	}


	if (vel[0]>0)
		lastDir = 1;
	else if (vel[0]<0)
		lastDir = 0;
}


void Player::getBox(Box2D& b)
{
	b.x0 = pos[0] - player_half;
	b.y0 = pos[1] - player_half;
	b.x1 = pos[0] + player_half;
	b.y1 = pos[1] + player_half;
}

///////////////////////////// SPRITE /////////////////////////////////

SpriteInfo Sprite::data[MAX_SPRITES] = {
	{NULL, 64, 64, 1, 1, "images/mushroom64x64x4.raw"},
	{NULL, 64,128, 1, 2, "images/door64x128x4.raw"},
	{NULL, 64, 64, 1, 1, "images/star64x64x4.raw"},
	{NULL, 64, 64, 1, 1, "images/spikes64x64x4.raw"},
	{NULL, 64, 64, 1, 1, "images/coin64x64x4.raw"},
	{NULL,128, 64, 1, 1, "images/bullet128x64x4.raw"},
};

Sprite::Sprite()
{
	x = y = 0;
	spriteIndex = -1;
}

Sprite::~Sprite()
{
}

void Sprite::set(float x, float y, int spriteIndex)
{
	this->x = x;
	this->y = y;
	this->spriteIndex = spriteIndex;
	if (data[spriteIndex].texture == NULL)
	{
		data[spriteIndex].texture = new Image(data[spriteIndex].filelame, data[spriteIndex].im_width, data[spriteIndex].im_height, 4);
		if (data[spriteIndex].texture->id == 0)
			abortGame( (string("Error loading ") + string(data[spriteIndex].filelame) ).c_str());
	}
}


void Sprite::moveTo(float x, float y)
{
	this->x = x;
	this->y = y;
}

bool Sprite::collide(Player& p)
{
	Box2D me,he;

	getBox(me);
	p.getBox(he);

	Box2D i = me.Inter(he);
	return i.isBox();
}

float Sprite::collide_area(Player& p)
{
	Box2D me, he;

	getBox(me);
	p.getBox(he);

	Box2D i = me.Inter(he);
	return i.area();
}

float Sprite::collide_area(int x, int y)
{
	Box2D me, he;
	he.x0 = x - 0.5f;
	he.y0 = y - 0.5f;
	he.x1 = x + 0.5f;
	he.y1 = y + 0.5f;

	getBox(me);

	Box2D i = me.Inter(he);
	return i.area();
}

void Sprite::render(int screenW, int screenH)
{
	data[spriteIndex].texture->set();

	float cell_size = screenW / (float)MAX_WIDTH;
	float w = cell_size * data[spriteIndex].cell_width;
	float h = cell_size * data[spriteIndex].cell_height;
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

void Sprite::getBox(Box2D& b)
{
	b.x0 = x - data[spriteIndex].cell_width * 0.5f;
	b.x1 = x + data[spriteIndex].cell_width * 0.5f;
	b.y0 = y - data[spriteIndex].cell_height* 0.5f;
	b.y1 = y + data[spriteIndex].cell_height* 0.5f;
}





//////////////////////////   bullet projectile /////////////////////////
Projectile::Projectile(float x, float y, float speed, bool dir, bool proj_type) : Sprite()
{
	this->x = x;
	this->y = y;
	spriteIndex = DATA_PROJECTILE;
	this->speed = speed;
	this->dir = dir;
	this->proj_type = proj_type;

	if (data[spriteIndex].texture == NULL)
	{
		data[spriteIndex].texture = new Image(data[spriteIndex].filelame, data[spriteIndex].im_width, data[spriteIndex].im_height, 4);
		if (data[spriteIndex].texture->id == 0)
			abortGame((string("Error loading ") + string(data[spriteIndex].filelame)).c_str());
	}
}

bool Projectile::update(float deltaTime, Level& level, Player& player)
{
	if (dir)
	{
		x += speed * deltaTime;
	}
	else
	{
		x -= speed * deltaTime;
	}
	Box2D projBox;
	getBox(projBox);

	// if it is an enemy porjectile, check for collision with the player
	if (proj_type == 1 && collide_area(player) > 0.5f)
	{
		player.energy -= BULLET_ENERGY_DAMAGE;
		if (player.energy <= 0)
		{
			player.lives--;
			if (player.lives > 0)
				player.energy = 100;
			else
				player.energy = 0;

		}
		return true;
	}

	int x0 = int(projBox.x0 + 0.5f);
	int x1 = int(projBox.x1 + 0.5f);
	int Y = int(0.5f * (projBox.y0 + projBox.y1));
	if (x0 <= 0 || x1 >= MAX_WIDTH || Y <= 0 || Y >= MAX_HEIGHT)
		return true;

	// check for collision with the map
	for (int X = x0; X <= x1; X++)
	{
		int cell = level(X, Y);

		if (collide_area(X, Y) < 0.15f)
			continue;

		if (cell == CELL_WALL)
		{
			return true; // delete projectile
		}
	
		if (this->proj_type == 0) // player bullet
		{
			if (cell == CELL_COIN)
			{
				level(X, Y) = CELL_EMPTY;
				player.score+=10;
				return true; // delete projectile
			}
			if (cell == CELL_MUSHROOM || cell == CELL_SPIKES || cell == CELL_STAR)
			{
				level(X, Y) = CELL_EMPTY;
				player.score += 100;
				return true; // delete projectile
			}

		}
	}
	return false;
}

void Projectile::render(int screenW, int screenH) const
{
	if (data[DATA_PROJECTILE].texture == NULL) // if not loaded
	{
		data[DATA_PROJECTILE].texture = new Image(
			data[DATA_PROJECTILE].filelame, 
			data[DATA_PROJECTILE].im_width, 
			data[DATA_PROJECTILE].im_height, 
			4);
		if (data[DATA_PROJECTILE].texture->id == 0)
			abortGame((string("Error loading ") + string(data[DATA_PROJECTILE].filelame)).c_str());

	}

	data[spriteIndex].texture->set();

	float cell_size = screenW / (float)MAX_WIDTH;
	float w = cell_size * data[spriteIndex].cell_width;
	float h = cell_size * data[spriteIndex].cell_height;
	float X = x * cell_size;
	float Y = y * cell_size;
	glColor3f(1, 1, 1);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_QUADS);

	if (dir) // right
	{
		if (proj_type == 0) // player bullet
		{
			glTexCoord2f(0.5f, 1); glVertex2f(X, Y);
			glTexCoord2f(1.0f, 1); glVertex2f(X + w, Y);
			glTexCoord2f(1.0f, 0); glVertex2f(X + w, Y + h);
			glTexCoord2f(0.5f, 0); glVertex2f(X, Y + h);
		}
		else // enemy bullet
		{
			glTexCoord2f(0.0f, 1); glVertex2f(X, Y);
			glTexCoord2f(0.5f, 1); glVertex2f(X + w, Y);
			glTexCoord2f(0.5f, 0); glVertex2f(X + w, Y + h);
			glTexCoord2f(0.0f, 0); glVertex2f(X, Y + h);

		}
	}
	else
	{
		if (proj_type == 0) // player bullet
		{
			glTexCoord2f(1.0f, 1); glVertex2f(X, Y);
			glTexCoord2f(0.5f, 1); glVertex2f(X + w, Y);
			glTexCoord2f(0.5f, 0); glVertex2f(X + w, Y + h);
			glTexCoord2f(1.0f, 0); glVertex2f(X, Y + h);
		}
		else // enemy bullet
		{
			glTexCoord2f(0.5f, 1); glVertex2f(X, Y);
			glTexCoord2f(0.0f, 1); glVertex2f(X + w, Y);
			glTexCoord2f(0.0f, 0); glVertex2f(X + w, Y + h);
			glTexCoord2f(0.5f, 0); glVertex2f(X, Y + h);

		}
	}
	glEnd();
	glDisable(GL_BLEND);
}