#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glut.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <iostream>

Display *display;
Window window;
GLXContext glContext;

float currentHealth = 100.0f; // Start with 100% health
const float maxHealth = 100.0f; // Maximum health
bool gameOver = false; // Track if the game is over

void initializeOpenGL() {
    int argc = 1;
    char *argv[1] = {(char *)"something"};
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    display = XOpenDisplay(nullptr);
    if (!display) {
        std::cerr << "Unable to open X display" << std::endl;
        exit(1);
    }

    int screen = DefaultScreen(display);
    Window root = RootWindow(display, screen);

    GLint attribs[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
    XVisualInfo *vi = glXChooseVisual(display, 0, attribs);
    if (!vi) {
        std::cerr << "No appropriate visual found" << std::endl;
        exit(1);
    }

    Colormap cmap = XCreateColormap(display, root, vi->visual, AllocNone);
    XSetWindowAttributes swa;
    swa.colormap = cmap;
    swa.event_mask = ExposureMask | KeyPressMask;

    window = XCreateWindow(display, root, 0, 0, 800, 600, 0, vi->depth, InputOutput,
                           vi->visual, CWColormap | CWEventMask, &swa);
    XMapWindow(display, window);
    
    glContext = glXCreateContext(display, vi, nullptr, GL_TRUE);
    if (!glContext) {
        std::cerr << "Failed to create an OpenGL context" << std::endl;
        exit(1);
    }
    
    if (!glXMakeCurrent(display, window, glContext)) {
        std::cerr << "Failed to make OpenGL context current" << std::endl;
        exit(1);
    }

    glViewport(0, 0, 800, 600);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 800, 0, 600, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

void cleanup() {
    glXMakeCurrent(display, 0, 0);
    glXDestroyContext(display, glContext);
    XDestroyWindow(display, window);
    XCloseDisplay(display);
}

void drawHealthBar(float currentHealth, float maxHealth) {
    float barX = 50;    // X position (top-left)
    float barY = 550;   // Y position (top-left)
    float barWidth = 200;  // Width of the health bar
    float barHeight = 20;   // Height of the health bar

    float healthPercentage = currentHealth / maxHealth;
    float currentBarWidth = barWidth * healthPercentage;

    // Draw the background of the health bar
    glBegin(GL_QUADS);
        glColor3f(0.5f, 0.5f, 0.5f);  // Grey background
        glVertex2f(barX, barY);
        glVertex2f(barX + barWidth, barY);
        glVertex2f(barX + barWidth, barY + barHeight);
        glVertex2f(barX, barY + barHeight);
    glEnd();

    // Draw the current health
    glBegin(GL_QUADS);
        glColor3f(0.0f, 1.0f, 0.0f);  // Green for health
        glVertex2f(barX, barY);
        glVertex2f(barX + currentBarWidth, barY);
        glVertex2f(barX + currentBarWidth, barY + barHeight);
        glVertex2f(barX, barY + barHeight);
    glEnd();
}

void renderText(float x, float y, const char *text, float r, float g, float b) {
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    for (const char *c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    // Draw the health bar
    drawHealthBar(currentHealth, maxHealth);
    
    // Check if health is 0 and render "You Died!" message
    if (currentHealth <= 0) {
        renderText(300, 300, "You Died!", 1.0f, 0.0f, 0.0f);
        renderText(200, 340, "Press R to Restart or Q to Quit", 1.0f, 1.0f, 1.0f);
    } else {
        // Render instructional text at the bottom right
        renderText(500, 580, "Press space bar to deal damage", 1.0f, 1.0f, 1.0f);
        renderText(500, 560, "Press H to heal", 1.0f, 1.0f, 1.0f);
    }
    
    glXSwapBuffers(display, window);
}

void handleKeyPress(XKeyEvent &event) {
    KeySym keysym = XLookupKeysym(&event, 0); // Get keysym for the pressed key

    if (currentHealth <= 0) {
        if (keysym == XK_r) { // 'R' key to restart
            currentHealth = maxHealth; // Reset health to 100%
        }
        if (keysym == XK_q) { // 'Q' key to quit
            exit(0);
        }
        return; // Skip further processing if game is over
    }

    if (keysym == XK_space) { // Space bar for damage
        currentHealth -= 10.0f; // Deal 10% damage
        if (currentHealth < 0) {
            currentHealth = 0; // Ensure health does not go below 0
        }
    }
    if (keysym == XK_h) { // 'H' key for healing
        currentHealth += 10.0f; // Heal 10%
        if (currentHealth > maxHealth) {
            currentHealth = maxHealth; // Ensure health does not exceed max
        }
    }
}

int main() {
    initializeOpenGL();

    XEvent event;
    while (true) {
        while (XPending(display)) {
            XNextEvent(display, &event);
            if (event.type == KeyPress) {
                handleKeyPress(reinterpret_cast<XKeyEvent&>(event)); // Cast to XKeyEvent
            }
        }

        render();
    }

    cleanup();
    return 0;
}

