#include <JEL/jel.h>
#include "stm/stm.h"
#include "core/gll/gll.h"
#include "cglm/cglm.h"
#include "anim/anim.h"
#include "core/globals.h"
#include "resm/resm.h"
#include "core/core.h"

static JEL_Entity player;
/* ANIMATION STATE */
static int animation_fn_create(struct STM_S *state)
{
  unsigned int *shader = JIN_resm_get("sprite_shader");

  glUseProgram(*shader);

  mat4 projection;
  glm_ortho(0.0f, (float) WINDOW_WIDTH, (float) WINDOW_HEIGHT, 0.0f, -1.0f, 1.0f, projection);
  glUniformMatrix4fv(glGetUniformLocation(*shader, "projection"), 1, GL_FALSE, (float *) projection);
  
  player = JEL_entity_create();
  JEL_ENTITY_ADD(player, Sprite);
  JEL_ENTITY_SET(player, Sprite, animd, JIN_resm_get("player_animation"));
  JEL_ENTITY_SET(player, Sprite, anim, 0);
  JEL_ENTITY_SET(player, Sprite, frame, 0);
  JEL_ENTITY_SET(player, Sprite, ticks, 0);

  return 0;
}

static int animation_fn_destroy(struct STM_S *state)
{
  JEL_entity_destroy(player);

  return 0;
}

static int animation_fn_update(struct STM_S *state)
{
  if (JIN_input.keys.d == 1) {
    JIN_stm_queue("3D", 0);
  }
  JIN_anim_update();

  return 0;
}

static int animation_fn_draw(struct STM_S *state)
{
  unsigned int *shader;
  unsigned int *texture;

  shader = JIN_resm_get("sprite_shader");
  texture = JIN_resm_get("test_image");

  JIN_anim_draw();

  return 0;
}

int JIN_states_create_animation(struct STM_S *state)
{
  /* Animation test */
  if (STM_s_create(state, 0, animation_fn_create, animation_fn_destroy, animation_fn_update, animation_fn_draw)) return -1;

  return 0;
}

