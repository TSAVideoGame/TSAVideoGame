#include "stm/stm.h"
#include "resm/resm.h"
#include "core/core.h"
#include "core/gll/gll.h"
#include "gfx/gfx.h"
#include "cglm/cglm.h"

/* IMAGE STATE */
static int img_fn_create(struct STM_S *state)
{
  return 0;
}

static int img_fn_destroy(struct STM_S *state)
{
  return 0;
}

static int img_fn_update(struct STM_S *state)
{
  if (JIN_input.keys.d == 1) {
    JIN_stm_queue("ANIMATION", 0);
  }
  return 0;
}

static int img_fn_draw(struct STM_S *state)
{
  unsigned int *shader;
  unsigned int *texture;

  shader = JIN_resm_get("sprite_shader");
  texture = JIN_resm_get("test_image");
  JIN_gfx_draw_sprite(shader, texture, 16, 16, 256, 256, 0, 0, 256, 256);
  JIN_gfx_draw_sprite(shader, texture, 16, 256, 256, 256, 32, 32, 224, 224);

  return 0;
}

int JIN_states_create_img(struct STM_S *state)
{
  if (STM_s_create(state, 0, img_fn_create, img_fn_destroy, img_fn_update, img_fn_draw)) return -1;

  return 0;
}

