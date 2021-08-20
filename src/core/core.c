#include "core.h"
#include "../snd/snd.h"
#include "globals.h"
#include "logger.h"
#include <JEL/jel.h>

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

  if (!(window->window = glfwCreateWindow(window_width, window_height, "Window", NULL, NULL))) ERR_EXIT(-1, "Could not create a window");
  window->active = 1;

  glfwMakeContextCurrent(window->window);
  glEnable(GL_DEPTH_TEST);

  WINDOW_WIDTH = window_width;
  WINDOW_HEIGHT = window_height;

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
  /* 'Libraries' */
  if (JIN_logger_init(JIN_LOGGER_LOG | JIN_LOGGER_ERR)) return -1;
  LOG(LOG, "Initializing libraries");
  if (!glfwInit())                    ERR_EXIT(-1, "Could not initialize GLFW");
  if (JIN_window_create(&JIN_window)) ERR_EXIT(-1, "Could not create a window");
  if (glewInit() != GLEW_OK)          ERR_EXIT(-1, "Could not initialize GLEW");
  if (JIN_snd_init())                 ERR_EXIT(-1, "Could not initialize Sound");
  if (JEL_init())                     ERR_EXIT(-1, "Could not initialize JEL");

  /* Singletons */
  LOG(LOG, "Creating singletons");
  if (JIN_resm_create(&JIN_resm))                         ERR_EXIT(-1, "Could not create a resource manager");
  if (STM_stack_create(&JIN_states))                      ERR_EXIT(-1, "Could not create a state stack");
  if (JIN_sndbgm_create(&JIN_sndbgm, "res/sounds/L.wav")) ERR_EXIT(-1, "Could not create background music");

  /* Core resources */
  if (JIN_resm_add(&JIN_resm, "JIN_CORE_MODEL_SPRITE", "res/models/square.mdld", JIN_RES_MODEL)) ERR_EXIT(-1, "Can't create the sprite model");

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
  LOG(LOG, "Quitting core (closing libraries and singletons)");
  JIN_sndbgm_destroy(&JIN_sndbgm);
  JIN_window_destroy(&JIN_window);
  STM_stack_destroy(&JIN_states);
  JIN_resm_destroy(&JIN_resm);

  JIN_snd_quit();
  glfwTerminate();
  JIN_logger_quit();
  
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
  JIN_sndbgm_update(&JIN_sndbgm);
  STM_stack_update(&JIN_states);

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
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  STM_stack_draw(&JIN_states);

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
