#ifndef __EMSCRIPTEN__
  #error Use Emscripten for building for web, if you are, something is mad sus
#endif

#include "core/core.h"
#include <emscripten/emscripten.h>

int main(int argc, char *args[])
{
  EM_ASM(alert("Confirm to begin"));

  JIN_init();

  JIN_input_loop(); /* No loop, just sets up callbacks */

  JIN_web_loop();

  JIN_quit();

  return 0;
}
