#include "stm/stm.h"
#include "core/core.h"
#include "resm/resm.h"
#include "core/gll/gll.h"
#include "cglm/cglm.h"
#include "gfx/gfx.h"

/* 3D STATE */
static int ticks = 0;
static int td_fn_create(struct STM_S *state)
{
  return 0;
}

static int td_fn_destroy(struct STM_S *state)
{
  return 0;
}

static int td_fn_update(struct STM_S *state)
{
  if (JIN_input.keys.d == 1) {
    JIN_stm_queue("IMG", 0);
  }
  ++ticks;

  return 0;
}

static int td_fn_draw(struct STM_S *state)
{
  unsigned int *shader;

  /* 3d fun */
  shader = JIN_resm_get("3d_shader");
  glUseProgram(*shader);
  vec3 vec = {0.0f, 0.0f, 0.0f};
  mat4 model = GLM_MAT4_IDENTITY_INIT;
  vec[1] = 1.0f;
  glm_rotate(model, glm_rad((ticks += 30)) / 30, vec); 

  mat4 view = GLM_MAT4_IDENTITY_INIT;
  vec[0] = 4.0f;
  vec[1] = -4.0f;
  vec[2] = -20.0f;
  glm_translate(view, vec);

  mat4 projection = GLM_MAT4_IDENTITY_INIT;

  glm_perspective(glm_rad(45.0f), (float) WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f, projection);

  glUniformMatrix4fv(glGetUniformLocation(*shader, "model"), 1, GL_FALSE, (const float *) model);
  glUniformMatrix4fv(glGetUniformLocation(*shader, "view"), 1, GL_FALSE, (const float *) view);
  glUniformMatrix4fv(glGetUniformLocation(*shader, "projection"), 1, GL_FALSE, (const float *) projection);

  glUniform3f(glGetUniformLocation(*shader, "light_color"), 1.0f, 1.0f, 1.0f);
  glUniform3f(glGetUniformLocation(*shader, "light_pos"), 4.0f, 0.0f, -40.0f);

  glBindVertexArray(((struct JIN_Model *) JIN_resm_get("3d_spaceship"))->vao);
  glDrawArrays(GL_TRIANGLES, 0, 78);

  return 0;
}

int JIN_states_create_3d(struct STM_S *state)
{
  unsigned int *shader = JIN_resm_get("3d_shader");

  glUseProgram(*shader);

  mat4 projection;
  glm_ortho(0.0f, (float) WINDOW_WIDTH, (float) WINDOW_HEIGHT, 0.0f, -1.0f, 1.0f, projection);
  glUniformMatrix4fv(glGetUniformLocation(*shader, "projection"), 1, GL_FALSE, (float *) projection);

  if (STM_s_create(state, 0, td_fn_create, td_fn_destroy, td_fn_update, td_fn_draw)) return -1;

  return 0;
}
