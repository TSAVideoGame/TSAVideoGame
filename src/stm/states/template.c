#include "stm/stm.h"

static int state_fn_create(struct STM_S *state)
{
  return 0;
}

static int state_fn_destroy(struct STM_S *state)
{
  return 0;
}

static int state_fn_update(struct STM_S *state)
{
  return 0;
}

static int state_fn_draw(struct STM_S *state)
{
  return 0;
}

int JIN_states_create_state(struct STM_S *state)
{
  if (STM_s_create(state, 0, state_fn_create, state_fn_destroy, state_fn_update, state_fn_draw)) return -1;
  return 0;
}

