#ifndef __EMSCRIPTEN__
  #error Use Emscripten for building for web, if you are, something is mad sus
#endif

#include "core/core.h"
#include "core/window/window.h"
#include "core/gll/gll.h"
#include "stm/states.h"
#include "snd/snd.h"
#include "resm/resm.h"
#include <emscripten/emscripten.h>

extern struct JIN_Window *root;

int web_loop(void)
{
  JIN_window_gl_set(root);
  if (JIN_gll()) {
    LOG(ERR, "JIN_gll() failed");
    JIN_input.quit = 1;
  }

  /* INITIALIZE */
  glEnable(GL_DEPTH_TEST);
  /* Core resources */
  if (JIN_resm_add(&JIN_resm, "JIN_MODEL_SPRITE", "res/models/square.mdld", JIN_RES_MODEL)) ERR_EXIT(0, "Can't create the sprite model");

  struct STM_State test;
  JIN_states_test_create(&test);
  JIN_state_push(&test);

  JIN_sndbgm_play();
  /* GAME LOOP */
  emscripten_set_main_loop(JIN_tick, 30, EM_TRUE);
  while (1) {
    if (JIN_input.quit) break;
    JIN_tick();
  }

  JIN_window_gl_unset(root);

  return 0;
}

int main(int argc, char *args[])
{
  JIN_init();

  web_loop();

  JIN_input_loop();

  JIN_quit();

  return 0;
}
