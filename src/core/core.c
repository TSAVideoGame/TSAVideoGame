#include "core.h"
#include "gll/gll.h"
#include "time.h"
#include "thread/thread.h"
#include "logger/logger.h"


#include <JEL/jel.h>
#include "../resm/resm.h"
#include "../snd/snd.h"
#include "../stm/stm.h"
#include "stm/states.h"


#include "window/window.h"
#include "env/env.h"

struct JIN_Window *root; /* Root window */
struct JIN_Env     env; /* Environment variables */

struct JIN_Input JIN_inputv;
struct JIN_Input JIN_input;

/* CORE FUNCTIONS */

/*
 * JIN_init
 *
 * @desc
 *   Initialize JIN
 * @return
 *    0 on success
 *   !0 on failure
 */
int JIN_init(void)
{
  if (JIN_logger_init(JIN_LOGGER_CONSOLE, JIN_LOGGER_ERR)) return 0;

  if (JIN_env_init(&JIN_env)) ERR_EXIT(-1, "Could not initialize the environment");
  if (!(root = JIN_window_create())) ERR_EXIT(-1, "Could not create the root window");

  JIN_INPUT_INIT(JIN_inputv);
  JIN_INPUT_INIT(JIN_input);

  /* Libraries */
  LOG(LOG, "Initializing libraries");
  if (JIN_snd_init())                 ERR_EXIT(0, "Could not initialize Sound");
  if (JEL_init())                     ERR_EXIT(0, "Could not initialize JEL");

  /* Singletons */
  LOG(LOG, "Creating singletons");
  if (RESM_create(&JIN_resm))                             ERR_EXIT(0, "Could not create a resource manager");
  if (STM_stack_create(&JIN_states))                      ERR_EXIT(0, "Could not create a state stack");
  if (JIN_sndbgm_create(&JIN_sndbgm, "res/sounds/L.wav")) ERR_EXIT(0, "Could not create background music");

  return 0;
}

/*
 * JIN_quit
 *
 * @desc
 *   Quit JIN
 * @return
 *   0 on success
 */
int JIN_quit(void)
{
  /* QUIT */
  LOG(LOG, "Quitting core (closing libraries and singletons)");
  JIN_sndbgm_destroy(&JIN_sndbgm);
  STM_stack_destroy(&JIN_states);
  RESM_destroy(&JIN_resm);
 
  JEL_quit();
  JIN_snd_quit();
  JIN_logger_quit();

  JIN_window_destroy(root);
  JIN_env_quit(&JIN_env);

  return 0;
}

#define FPS         30
#define FRAME_DELAY (1000 / FPS)
void JIN_tick(void)
{
  clock_t frame_start, frame_end;
  double  frame_time;

  frame_start = clock();

  JIN_input = JIN_inputv;
  JIN_update();
  JIN_draw();

  frame_end = clock();
  frame_time = (frame_end - frame_start) / CLOCKS_PER_SEC / 1000;

  if (FRAME_DELAY > frame_time) {
    JIN_sleep(FRAME_DELAY - frame_time);
  }

  /* return 0; */
}

/*
 * JIN_update
 * 
 * @desc
 *   Update stuff
 * @return
 *   0 on success
 */
int JIN_update(void)
{
  JIN_sndbgm_update(&JIN_sndbgm);
  STM_stack_update(&JIN_states);
  
  return 0;
}

/*
 * JIN_draw
 *
 * @desc
 *   Draw stuff
 * @return
 *   0 on success
 */
int JIN_draw(void)
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  STM_stack_draw(&JIN_states);
  
  JIN_window_buffer_swap(root);

  return 0;
}

/*
 * JIN_dialog
 *
 * @desc
 *   Create a dialog (pop-up box)
 *   to display a message to the user
 * @param msg
 *   String to display
 * @return
 *    0 on success
 *   !0 on failure
 */
int JIN_dialog(const char *msg)
{
  return 0;
}

/*
 *
 */
  /* Create states */
struct STM_State test;

/* This shouldn't be needed but is for some reason, GLAPIENTRY is not defined */
#ifdef _WIN32
#define GLAPIENTRY __stdcall
#endif
#include <stdio.h>
void GLAPIENTRY gl_err_callback(GLenum src, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *msg, const void *usr_param)
{
  if (severity != GL_DEBUG_SEVERITY_NOTIFICATION)
    fprintf(stderr, "GL CALLBACK: type = 0x%x, severity = 0x%x, message = %s\n", type, severity, msg);
}
JIN_THREAD_FN JIN_game_thread(void *data)
{
  JIN_window_gl_set(root);
  if (JIN_gll()) {
    LOG(ERR, "JIN_gll() failed");
    JIN_input.quit = 1;
  }

  /* INITIALIZE */
  /*glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(gl_err_callback, 0);*/
  glEnable(GL_DEPTH_TEST);
  /* Core resources */
  if (JIN_resm_add("JIN_MODEL_SPRITE", "res/models/square.mdld", RESM_MODEL)) ERR_EXIT(0, "Can't create the sprite model");
  
  JIN_states_test_create(&test);
  JIN_state_push(&test);

  JIN_sndbgm_play();
  /* GAME LOOP */
  while (1) {
    if (JIN_input.quit) break;
    JIN_tick();
  }

  JIN_window_gl_unset(root);
  
  return 0;
}

