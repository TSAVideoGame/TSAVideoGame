#include "stm/stm.h"
#include <JEL/jel.h>
#include "core/core.h"
#include "components/components.h"
#include "gfx/sprite.h"
#include "gfx/gfx.h"
#include "resm/resm.h"
#include "core/gll/gll.h"

/* Max of three digits */
#define DIGITS 3

struct {
  JEL_Entity img;
  JEL_Entity collected;
  JEL_Entity total;
} game_win_vars;

int artifacts_total;
int artifacts_collected;

static int game_win_fn_create(struct STM_S *state)
{
  unsigned int *shader = JIN_resm_get("sprite_shader");
  glUseProgram(*shader);
  glUniform1f(glGetUniformLocation(*shader, "lighting"), 0.0f);
  
  game_win_vars.img = JEL_entity_create();
  JEL_SET(game_win_vars.img, Position, 352, 32);
  JEL_SET(game_win_vars.img, Sprite, 0, 256, 256, 128, 144, 128, 128, 0);

  game_win_vars.collected = JEL_entity_create();
  JEL_SET(game_win_vars.collected, Position, 352, 320);
  JEL_SET(game_win_vars.collected, Sprite, 0, 128, 64, 320, 48, 64, 32, 0);

  for (int i = 0; i < DIGITS; ++i) {
    int digit = artifacts_collected % 10;
    artifacts_collected /= 10;

    JEL_Entity d = JEL_entity_create();
    JEL_SET(d, Position, 496 + 64 * (DIGITS - 1) - 64 * i, 320);
    JEL_SET(d, Sprite, 0, 64, 64, 80 + digit * 16, 0, 16, 16, 0);
  }

  game_win_vars.total = JEL_entity_create();
  JEL_SET(game_win_vars.total, Position, 352, 384);
  JEL_SET(game_win_vars.total, Sprite, 0, 128, 64, 320, 80, 64, 32, 0);
  
  for (int i = 0; i < DIGITS; ++i) {
    int digit = artifacts_total % 10;
    artifacts_total /= 10;

    JEL_Entity d = JEL_entity_create();
    JEL_SET(d, Position, 496 + 64 * (DIGITS - 1) - 64 * i, 384);
    JEL_SET(d, Sprite, 0, 64, 64, 80 + digit * 16, 0, 16, 16, 0);
  }

  return 0;
}

static int game_win_fn_destroy(struct STM_S *state)
{
  struct JEL_Query q;
  JEL_QUERY(q, Sprite);
  for (unsigned int t = 0; t < q.count; ++t) {
    struct JEL_EntityCIt e;
    JEL_IT(e, q.tables[t], JEL_EntityC);

    for (unsigned int i = 1; q.tables[t]->count > 1;) {
      JEL_entity_destroy(e.entity[i]);
    }
  }
  JEL_query_destroy(&q);
  
  return 0;
}

static int game_win_fn_update(struct STM_S *state)
{
  if (JIN_input.keys.o == 1) {
    JIN_stm_queue("LVL_SEL", 0);
  }

  return 0;
}

static int game_win_fn_draw(struct STM_S *state)
{
  JIN_gfx_sprite_draw(0, 0);
  return 0;
}

int JIN_states_create_game_win(struct STM_S *state)
{
  if (STM_s_create(state, 0, game_win_fn_create, game_win_fn_destroy, game_win_fn_update, game_win_fn_draw)) return -1;
  return 0;
}

