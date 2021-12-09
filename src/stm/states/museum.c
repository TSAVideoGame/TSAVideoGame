#include "stm/stm.h"

/*
 * MUSEUM
 *
 * This is a museum level, whether
 * it's an actual level or procedurally
 * generated shouldn't matter
 */

static int museum_fn_create(struct STM_S *state)
{
  return 0;
}

static int museum_fn_destroy(struct STM_S *state)
{
  return 0;
}

static int museum_fn_update(struct STM_S *state)
{
  return 0;
}

static int museum_fn_draw(struct STM_S *state)
{
  return 0;
}

int JIN_states_create_museum(struct STM_S *state)
{
  if (STM_s_create(state, 0, museum_fn_create, museum_fn_destroy, museum_fn_update, museum_fn_draw)) return -1;
  return 0;
}

