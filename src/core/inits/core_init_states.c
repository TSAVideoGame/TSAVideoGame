#include "stm/states.h"

/*
 * CORE INIT STATE
 *
 * A list of states to initialize
 */

#define INIT_STATE_LIST \
  X("MAIN_MENU", JIN_states_create_main_menu) \
  X("LVL_SEL",   JIN_states_create_lvlsel) \
  X("MUSEUM",    JIN_states_create_museum) \
  X("PAUSE",     JIN_states_create_pause) \
  X("GAME_OVER", JIN_states_create_game_over) \
  X("GAME_WIN",  JIN_states_create_game_win) \

static int init_states(void)
{
  #define X(name, fn) JIN_stm_add(name, fn);
  INIT_STATE_LIST
  #undef X
  return 0;
}
