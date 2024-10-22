/*
 * AUTHOR: SKYE BEEBE
 *
 * DATE: October 2024
 */
#include "fonts.h"
#include<chrono>
#include <ctime>
#include <unistd.h>
#include <cstdio>
#include <GL/glx.h>
#include "sbeebe.h"

using namespace std;


Stopwatch stopwatch (int sec, int min) {

    while(true) {
        sleep(1);
        sec++;

        if(sec > 59) {
            min++;
            sec = 0;
        }

    }

    //ggprint8b(&r, 16, c, "Min: %i :: Sec: %i :: Millisec: %i ", s.min, s.sec, s.milsec);

}
