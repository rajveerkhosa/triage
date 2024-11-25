#ifndef _CONSTANTS_h
#define _CONSTANTS_h

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

//constants
const float timeslice = 1.0f;
const float gravity = -0.2f;
const float friction = 0.9f;
const float jumpStrength = 5.0f;
#define ALPHA 1


#endif

