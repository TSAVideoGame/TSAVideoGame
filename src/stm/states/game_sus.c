#include "stm/stm.h"
#include <JEL/jel.h>
#include "core/core.h"
#include "components/components.h"
#include "gfx/sprite.h"
#include "gfx/gfx.h"
#include "resm/resm.h"
#include "core/gll/gll.h"

struct {
  JEL_Entity img;
} game_sus_vars;

#include "core/logger/logger.h"
static int game_sus_fn_create(struct STM_S *state)
{
  unsigned int *shader = JIN_resm_get("sprite_shader");
  glUseProgram(*shader);
  glUniform1f(glGetUniformLocation(*shader, "lighting"), 0.0f);
  
  game_sus_vars.img = JEL_entity_create();
  JEL_SET(game_sus_vars.img, Position, 352, 32);
  JEL_SET(game_sus_vars.img, Sprite, 0, 256, 256, 256, 144, 128, 128, 0);

  return 0;
}

static int game_sus_fn_destroy(struct STM_S *state)
{
  JEL_entity_destroy(game_sus_vars.img);
  
  return 0;
}

static int game_sus_fn_update(struct STM_S *state)
{
  if (JIN_input.keys.o == 1) {
    JIN_stm_queue("LVL_SEL", 0);
  }

  return 0;
}

static int game_sus_fn_draw(struct STM_S *state)
{
  JIN_gfx_sprite_draw(0, 0);
  return 0;
}

int JIN_states_create_game_sus(struct STM_S *state)
{
  if (STM_s_create(state, 0, game_sus_fn_create, game_sus_fn_destroy, game_sus_fn_update, game_sus_fn_draw)) return -1;
  return 0;
}
