#include "stm/stm.h"

static int game_win_fn_create(struct STM_S *state)
{
  return 0;
}

static int game_win_fn_destroy(struct STM_S *state)
{
  return 0;
}

static int game_win_fn_update(struct STM_S *state)
{
  return 0;
}

static int game_win_fn_draw(struct STM_S *state)
{
  return 0;
}

int JIN_states_create_game_win(struct STM_S *state)
{
  if (STM_s_create(state, 0, game_win_fn_create, game_win_fn_destroy, game_win_fn_update, game_win_fn_draw)) return -1;
  return 0;
}

