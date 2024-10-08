// jump.h                                                                                                            
#ifndef JUMP_H
#define JUMP_H

#include "types.h"

//	Gravity
extern const float gravity; 

void handleJump(Vec &pos, Vec &vel, bool &isJumping);
void initiateJump(Vec &vel, bool &isJumping);

#endif
