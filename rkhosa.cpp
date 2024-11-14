//  Rajveer Khosa
#include "rkhosa.h"

Level::Level() {
    // Constructor implementation
    tilesize[0] = 32;
    tilesize[1] = 32;
    ftsz[0] = (Flt)tilesize[0];
    ftsz[1] = (Flt)tilesize[1];
    tile_base = 220.0;
    //read level
    FILE *fpi = fopen("level1.txt","r");
    if (fpi) {
        nrows=0;
        char line[100];
        while (fgets(line, 100, fpi) != NULL) {
            removeCrLf(line);
            int slen = strlen(line);
            ncols = slen;
            //Log("line: %s\n", line);
            for (int j=0; j<slen; j++) {
                arr[nrows][j] = line[j];
            }
            ++nrows;
        }
        fclose(fpi);
        //printf("nrows of background data: %i\n", nrows);
    }
    for (int i=0; i<nrows; i++) {
        for (int j=0; j<ncols; j++) {
            printf("%c", arr[i][j]);
        }   
        printf("\n");
    }
} lev; 

void Level::removeCrLf(char *line) {
    char *p = str;
        while (*p) {
            if (*p == 10 || *p == 13) {
                *p = '\0';
                break;
            }
            ++p;
        }
    }
}

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

