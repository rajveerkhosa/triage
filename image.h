#ifndef _IMAGE_h
#define _IMAGE_h

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

class Image 
{
public:
    int width, height;
    unsigned char *data;
    ~Image() 
	{ 
		delete [] data; 
	}
	
    Image(const char *fname) 
	{
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


#endif

