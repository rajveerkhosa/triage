
/*#include "cschmiedel.h"
#include <cstdio>  // For snprintf
#include <GL/gl.h> // For OpenGL rendering functions

void Timer::render(int x, int y)  {
    int remaining_time = getRemainingTime();
    int minutes = remaining_time / 60;
    int seconds = remaining_time % 60;

    char time_text[16];
    snprintf(time_text, sizeof(time_text), "%d:%02d", minutes, seconds);

    Rect timer_position; // Define a Rect for positioning the text
    timer_position.bot = y;
    timer_position.left = x;
    timer_position.center = 0;

    glColor3f(1.0, 1.0, 1.0); // Set text color to white
    ggprint8b(&timer_position, 0, 0x00ffffff, time_text); // Render the timer
}
*/

#include "cschmiedel.h"
#include <cstdio>  // For snprintf
#include <GL/gl.h> // For OpenGL rendering functions
#include <fstream> // For logging
#include <string>  // For handling log messages
#include <ctime>   // For timestamps
#include <sstream> // For formatting time
#include <chrono>  // For high-resolution timing

// Logging levels
enum LogLevel {
    INFO,
    WARNING,
    ERROR,
    DEBUG
};

// Logger class definition
class Logger {
private:
    std::ofstream logFile;
    LogLevel logLevel;

    // Get current timestamp as a string
    std::string getCurrentTime() {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        std::ostringstream oss;
        oss << 1900 + ltm->tm_year << "-" << 1 + ltm->tm_mon << "-" << ltm->tm_mday
            << " " << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec;
        return oss.str(); 
    }

    // Convert log level to string
    std::string logLevelToString(LogLevel level) {
        switch (level) {
            case INFO: return "INFO";
            case WARNING: return "WARNING";
            case ERROR: return "ERROR";
            case DEBUG: return "DEBUG";
            default: return "UNKNOWN";
        }       
    }           

public: 
    // Constructor initializes the log file
    Logger(const std::string& filename, LogLevel level = INFO)
        : logLevel(level) {
        logFile.open(filename, std::ios::out | std::ios::app);
        if (!logFile.is_open()) {
            throw std::ios_base::failure("Failed to open log file.");
        }   
    }           

    // Destructor closes the log file
    ~Logger() {
        if (logFile.is_open()) {
            logFile.close();
        }   
    }           

    // Log a message with a specific log level
    void log(const std::string& message, LogLevel level = INFO) {
        if (level >= logLevel) {
            logFile << "[" << getCurrentTime() << "] "
                    << "[" << logLevelToString(level) << "] "
                    << message << std::endl;
        }               
    }                   

    // Set the minimum log level
    void setLogLevel(LogLevel level) {
        logLevel = level;
    }   
};

// Create a global logger instance
Logger gameLogger("game_log.txt", INFO);

// Timer rendering function
void Timer::render(int x, int y) {
    int remaining_time = getRemainingTime();
    int minutes = remaining_time / 60;
    int seconds = remaining_time % 60;

    char time_text[16];
    snprintf(time_text, sizeof(time_text), "%d:%02d", minutes, seconds);

    Rect timer_position; // Define a Rect for positioning the text 
    timer_position.bot = y;
    timer_position.left = x; 
    timer_position.center = 0;

    glColor3f(1.0, 1.0, 1.0); // Set text color to white
    ggprint8b(&timer_position, 0, 0x00ffffff, time_text); // Render the timer

    // Log rendering of the timer
    gameLogger.log("Timer rendered: " + std::string(time_text), DEBUG);
}

// FPS Counter Class Implementation
/*void FPSCounter::update() {
    frameCount++;
    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsed = currentTime - startTime;

    if (elapsed.count() >= 1.0f) {
        fps = frameCount / elapsed.count();
        frameCount = 0;
        startTime = currentTime;

        // Log FPS update
        gameLogger.log("FPS updated: " + std::to_string(fps), DEBUG);
    }
}

void FPSCounter::render(int x, int y) const {
    char fps_text[16];
    snprintf(fps_text, sizeof(fps_text), "FPS: %.1f", fps);

    Rect fps_position; // Define a Rect for positioning the text
    fps_position.bot = y;
    fps_position.left = x;
    fps_position.center = 0;

    glColor3f(1.0, 1.0, 1.0); // Set text color to white
    ggprint8b(&fps_position, 0, 0x00ffffff, fps_text);
}
*/
// Example usage: Logging during initialization
void initializeGame() {
    gameLogger.log("Game initialized successfully.", INFO);
}

// Example usage: Logging an error
void reportError(const std::string& errorMessage) {
    gameLogger.log("Error occurred: " + errorMessage, ERROR);
}

// Example usage: Logging debug information 
void debugInfo(const std::string& debugMessage) {
    gameLogger.log("Debug info: " + debugMessage, DEBUG);
}
//  Christian Schmiedel
