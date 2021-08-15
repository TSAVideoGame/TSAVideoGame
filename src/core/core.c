#include "core.h"
#include "../snd/snd.h"
#include "globals.h"

/* WINDOW FUNCTIONS */

/*
 * JIN_window_create
 *
 * @desc
 * @param window
 * @return
 */
int JIN_window_create(struct JIN_Window *window)
{
  int window_width  = 480;
  int window_height = 320;

  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  if (!(window->window = glfwCreateWindow(window_width, window_height, "Window", NULL, NULL))) return -1;
  window->active = 1;

  glfwMakeContextCurrent(window->window);
  
  return 0;
}

/*
 * JIN_window_destroy
 *
 * @desc
 * @param window
 * @return
 */
int JIN_window_destroy(struct JIN_Window *window)
{
  glfwDestroyWindow(window->window);

  return 0;
}

/* CORE FUNCTIONS */

/*
 * JIN_core_init
 *
 * @desc
 * @return
 */
int JIN_core_init(void)
{
  if (!glfwInit()) return -1;
  JIN_window_create(&JIN_window);
  if (glewInit() != GLEW_OK) return -1;

  if (JIN_snd_init()) return -1;

  JIN_resm_create(&JIN_resm);
  STM_stack_create(&JIN_states);

  return 0;
}

/*
 * JIN_core_quit
 *
 * @desc
 * @return
 */
int JIN_core_quit(void)
{
  JIN_window_destroy(&JIN_window);
  STM_stack_destroy(&JIN_states);
  JIN_resm_destroy(&JIN_resm);

  JIN_snd_quit();

  glfwTerminate();

  return 0;
}

/*
 * JIN_core_input
 *
 * @desc
 * @return
 */
int JIN_core_input(void)
{
  glfwPollEvents();

  if (glfwWindowShouldClose(JIN_window.window)) {
    JIN_window.active = 0;
  }
  return 0;
}

/*
 * JIN_core_update
 *
 * @desc
 * @return
 */
int JIN_core_update(void)
{
  return 0;
}

/*
 * JIN_core_draw
 *
 * @desc
 * @return
 */
int JIN_core_draw(void)
{
  glfwSwapBuffers(JIN_window.window);

  return 0;
}

/*
 * JIN_core_active
 *
 * @desc
 * @return
 */
int JIN_core_active(void)
{
  return JIN_window.active;
}

/* MISC FUNCTIONS */

/*
 * JIN_sleep
 *
 * @desc
 *   Sleep function so game doesn't
 *   use 100% of the cpu
 * @param time
 *   Time in ms
 * @return
 */
#ifdef __linux__
  #include <unistd.h>
#elif _WIN32
  #include <windows.h>
#else
  #error Platform not supported (JIN_sleep)
#endif
int JIN_sleep(double time)
{
  #ifdef __linux__
    usleep(time * 1000);
  #elif _WIN32
    Sleep(time);
  #else
    #error Platform not supported (JIN_sleep)
  #endif
  return 0;
}
