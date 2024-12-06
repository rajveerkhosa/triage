/*#ifndef CSCHMIEDEL_H
#define CSCHMIEDEL_H

#include <ctime>   // Required for timespec
#include "fonts.h" // Required for ggprint8b and Rect

class Timer {
private:
    struct timespec startTime; // Stores when the timer starts
    struct timespec pausedTime;
    int duration;              // Timer duration in seconds
    bool paused;
public:
    Timer() : duration(0), paused(false)
    {
        startTime.tv_sec = 0;
        startTime.tv_nsec = 0;
        pausedTime.tv_sec = 0;
        pausedTime.tv_nsec = 0;
    }

    void start(int seconds)
    {
        clock_gettime(CLOCK_REALTIME, &startTime);
        duration = seconds;
        paused = false;
    }
   
    void pause()
    {
        if (!paused)
        {
            clock_gettime(CLOCK_REALTIME, &pausedTime);
            paused = true;
        }
    }
   
    void resume()
    {
        if (paused)
        {
            struct timespec now;
            clock_gettime(CLOCK_REALTIME, &now);
            // Adjust startTime to account for the paused duration
            startTime.tv_sec += (now.tv_sec - pausedTime.tv_sec);
            startTime.tv_nsec += (now.tv_nsec - pausedTime.tv_nsec);
            if (startTime.tv_nsec >= 1000000000) 
            {
                startTime.tv_sec += 1;
                startTime.tv_nsec -= 1000000000;
            }
            paused = false;
        }
    }

    int getRemainingTime() const 
    {
        if (paused) {
            long elapsed = pausedTime.tv_sec - startTime.tv_sec;
            int remaining = duration - static_cast<int>(elapsed);
            return (remaining > 0) ? remaining : 0;
        } else 
        {
        struct timespec now;
        clock_gettime(CLOCK_REALTIME, &now);

        long elapsed = now.tv_sec - startTime.tv_sec;
        int remaining = duration - static_cast<int>(elapsed);

        return (remaining > 0) ? remaining : 0;
    }
    }
    void render(int x, int y); // Render the time
 
};

#endif // CSCHMIEDEL_H
*/
#ifndef CSCHMIEDEL_H
#define CSCHMIEDEL_H
#include <cstdio>
#include <GL/gl.h>
#include <ctime>   // Required for timespec
#include "fonts.h" // Required for ggprint8b and Rect

class Timer {
private:
    struct timespec startTime; // Stores when the timer starts
    struct timespec pausedTime;
    int duration;              // Timer duration in seconds
    bool paused;
public:
    Timer() : duration(0), paused(false)
    {   
        startTime.tv_sec = 0;
        startTime.tv_nsec = 0;
        pausedTime.tv_sec = 0;
        pausedTime.tv_nsec = 0;
    }   

    void start(int seconds)
    {   
        clock_gettime(CLOCK_REALTIME, &startTime);
        duration = seconds;
        paused = false;
    }   

    void pause()
    {   
        if (!paused)
        {   
            clock_gettime(CLOCK_REALTIME, &pausedTime);
            paused = true;
        }   
    }   

    void resume()
    {   
        if (paused)
        {   
            struct timespec now;
            clock_gettime(CLOCK_REALTIME, &now);
            // Adjust startTime to account for the paused duration
            startTime.tv_sec += (now.tv_sec - pausedTime.tv_sec);
            startTime.tv_nsec += (now.tv_nsec - pausedTime.tv_nsec);
            if (startTime.tv_nsec >= 1000000000)
            {   
                startTime.tv_sec += 1;
                startTime.tv_nsec -= 1000000000;
            }   
            paused = false;
        }   
    }   

    int getRemainingTime() const
    {   
        if (paused) {
            long elapsed = pausedTime.tv_sec - startTime.tv_sec;
            int remaining = duration - static_cast<int>(elapsed);
            return (remaining > 0) ? remaining : 0;
        } else
        {   
            struct timespec now;
            clock_gettime(CLOCK_REALTIME, &now);

            long elapsed = now.tv_sec - startTime.tv_sec;
            int remaining = duration - static_cast<int>(elapsed);

            return (remaining > 0) ? remaining : 0;
        }   
    }   
    void render(int x, int y); // Render the timer
};

class FPSCounter {
private:
    int frameCount;
    double elapsedTime;
    double fps;
    struct timespec lastTime;

public:
    FPSCounter() : frameCount(0), elapsedTime(0.0), fps(0.0)
    {   
        clock_gettime(CLOCK_REALTIME, &lastTime);
    }   

    void update()
    {   
        struct timespec currentTime;
        clock_gettime(CLOCK_REALTIME, &currentTime);

        double timeDiff = (currentTime.tv_sec - lastTime.tv_sec) +
                          (currentTime.tv_nsec - lastTime.tv_nsec) / 1e9;

        elapsedTime += timeDiff;
        frameCount++;

        if (elapsedTime >= 1.0)
        {   
            fps = frameCount / elapsedTime;
            elapsedTime = 0.0;
            frameCount = 0;
        }   

        lastTime = currentTime;
    }   

    void render(int x, int y) const
    {   
        char fps_text[16];
        snprintf(fps_text, sizeof(fps_text), "FPS: %.2f", fps);

        Rect fps_position;
        fps_position.bot = y;
        fps_position.left = x;
        fps_position.center = 0;

        glColor3f(1.0, 1.0, 1.0); // Set text color to white
        ggprint8b(&fps_position, 0, 0x00ffffff, fps_text); // Render the FPS counter
    }   
};

#endif // CSCHMIEDEL_H

