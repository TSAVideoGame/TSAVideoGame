#ifndef JIN_CORE_H
#define JIN_CORE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

/*
 * CORE
 *
 * Window, input, etc
 * Window functions are private because
 * the game only has one window, so one
 * OpenGL context and things are simple
 *
 * TYPES:
 * struct JIN_Window | The game window
 */

struct JIN_Window {
  GLFWwindow *window;
  int         active; /* Should window be closed? */
};

/*
 * Core functions
 *
 * init   | Initialize JIN
 * quit   | Quit JIN
 * input  | Recieve user input
 * update | Perform game logic
 * draw   | Draw game to the screen
 * active | Is the game still running?
 */
int JIN_core_init  (void);
int JIN_core_quit  (void);
int JIN_core_input (void);
int JIN_core_update(void);
int JIN_core_draw  (void);
int JIN_core_active(void);

/*
 * Misc functions
 */
int JIN_sleep(double time);

#endif
