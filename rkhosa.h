// Level.h
#ifndef LEVEL_H
#define LEVEL_H

#include <cstdio>
#include <cstring>

typedef float Flt;



class Level {
public:
    unsigned char arr[16][80];
    int nrows, ncols;
    int tilesize[2];
    Flt ftsz[2];
    Flt tile_base;
    Level();
    void removeCrLf(char *line);
};

#endif // LEVEL_H


