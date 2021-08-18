#include "states.h"
#include "../resm/resm.h"
#include "../core/globals.h"
#include "snd/snd.h"
#include "../gfx/gfx.h"
#include <GL/glew.h>
#include "../gfx/cglm/cglm.h"
#include "../core/logger.h"

static int test_fn_create(struct STM_State *state)
{
  JIN_sndbgm_play();

  JIN_resm_add(&JIN_resm, "sprite_shader", "res/shaders/sprite.shdr", JIN_RES_SHADER);

  unsigned int *shader = JIN_resm_get(&JIN_resm, "sprite_shader");

  glUseProgram(*shader);

  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  
  mat4 projection;
  glm_ortho(0.0f, 640.0f, 480.0f, 0.0f, -1.0f, 1.0f, projection);
  glUniformMatrix4fv(glGetUniformLocation(*shader, "projection"), 1, GL_FALSE, (float *) projection);

  JIN_resm_add(&JIN_resm, "test_image", "res/images/test_img.png", JIN_RES_PNG);

  return 0;
}

static int test_fn_destroy(struct STM_State *state)
{
  return 0;
}

static int test_fn_update(struct STM_State *state)
{
  return 0;
}

static int test_fn_draw(struct STM_State *state)
{
  unsigned int *shader;
  unsigned int *texture;

  shader = JIN_resm_get(&JIN_resm, "sprite_shader");
  texture = JIN_resm_get(&JIN_resm, "test_image");
  JIN_gfx_draw_sprite(shader, texture, 16, 16, 256, 256, 0, 0, 1024, 1024);
  return 0;
}

int JIN_states_test_create(struct STM_State *state)
{
  if (STM_state_create(state, "test", 0, 0, test_fn_create, test_fn_destroy, test_fn_update, test_fn_draw)) return -1;

  return 0;
}

