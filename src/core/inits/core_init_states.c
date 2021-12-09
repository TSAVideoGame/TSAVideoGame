#include "stm/states.h"

/*
 * CORE INIT STATE
 *
 * A list of states to initialize
 */

#define INIT_STATE_LIST \
  X("IMG", JIN_states_create_img) \
  X("ANIMATION", JIN_states_create_animation) \
  X("3D", JIN_states_create_3d) \
  X("MAIN_MENU", JIN_states_create_main_menu) \
  X("MUSEUM", JIN_states_create_museum) \

static int init_states(void)
{
  #define X(name, fn) JIN_stm_add(name, fn);
  INIT_STATE_LIST
  #undef X
  return 0;
}
