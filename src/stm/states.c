#include "states.h"
#include "../resm/resm.h"
#include "../core/globals.h"
#include "snd/snd.h"
#include <GL/glew.h>
#include "../gfx/cglm/cglm.h"
#include "../core/logger.h"

static unsigned int vao;

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

  /* OpenGL test */
  unsigned int vbo;
  float vertices[] = {
    /* Pos        Tex pos  */
    0.0f, 1.0f,   0.0f, 1.0f,
    1.0f, 0.0f,   1.0f, 0.0f,
    0.0f, 0.0f,   0.0f, 0.0f,

    0.0f, 1.0f,   0.0f, 1.0f,
    1.0f, 1.0f,   1.0f, 1.0f,
    1.0f, 0.0f,   1.0f, 0.0f
  };

  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  /* Pos */
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) 0);
  glEnableVertexAttribArray(0);
  /* Text pos */
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) (2 * sizeof(float)));
  glEnableVertexAttribArray(1);

  /* Un-bind everything */
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  
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
  int x = 16;
  int y = 16;
  int w = 256;
  int h = 256;
  int t_x = 0;
  int t_y = 0;
  int t_w = 1024;
  int t_h = 1024;
  unsigned int *program = JIN_resm_get(&JIN_resm, "sprite_shader");

  glUseProgram(*program);

  mat4 model = GLM_MAT4_IDENTITY_INIT;

  glm_translate(model, (vec3) {x, y, 0.0f});
  glm_scale(model, (vec3) {w, h, 1.0f});

  glUniformMatrix4fv(glGetUniformLocation(*program, "model"), 1, GL_FALSE, (float *) model);

  glUniform2f(glGetUniformLocation(*program, "tex_scale"), t_w, t_h);
  glUniform2f(glGetUniformLocation(*program, "text_translate"), t_x, t_y);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_RECTANGLE, * (unsigned int *) JIN_resm_get(&JIN_resm, "test_image"));

  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);

  return 0;
}

int JIN_states_test_create(struct STM_State *state)
{
  if (STM_state_create(state, "test", 0, 0, test_fn_create, test_fn_destroy, test_fn_update, test_fn_draw)) return -1;

  return 0;
}

