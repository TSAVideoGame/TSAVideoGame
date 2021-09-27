#include "states.h"
#include "resm/resm.h"
#include "core/core.h"
#include "snd/snd.h"
#include "gfx/gfx.h"
#include "glew/glew.h"
#include "cglm/cglm.h"
#include "anim/anim.h"
#include <JEL/jel.h>

static JEL_Entity player; /* Don't do this, create a state variable */

/*
 * TODO
 *
 * Renderdoc rasterizer in the pipeline is reporting
 * 0 for width and height, when it should be
 * 640 and 480. This is a big problem, not sure what
 * to do...
 */
static int test_fn_create(struct STM_State *state)
{
  JIN_sndbgm_play();


  /*JIN_resm_add(&JIN_resm, "sprite_shader", "res/shaders/sprite.shdr", JIN_RES_SHADER);
  unsigned int *shader = JIN_resm_get(&JIN_resm, "sprite_shader");

  glUseProgram(*shader);

  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  mat4 projection;
  glm_ortho(0.0f, (float) WINDOW_WIDTH, (float) WINDOW_HEIGHT, 0.0f, -1.0f, 1.0f, projection);
  glUniformMatrix4fv(glGetUniformLocation(*shader, "projection"), 1, GL_FALSE, (float *) projection);

  JIN_resm_add(&JIN_resm, "test_image", "res/images/test_image.png", JIN_RES_PNG);

  JIN_resm_add(&JIN_resm, "test_shader", "res/shaders/test.shdr", JIN_RES_SHADER);
   3d fun */
  /*JIN_resm_add(&JIN_resm, "3d_shader", "res/shaders/3d.shdr", JIN_RES_SHADER);
  JIN_resm_add(&JIN_resm, "3d_spaceship", "res/models/space_ship.mdld", JIN_RES_MODEL);
*/
  /* Animation test */
  /*JIN_resm_add(&JIN_resm, "player_animation", "res/animations/player.animd", JIN_RES_ANIM);
  JIN_resm_add(&JIN_resm, "player_img", "res/images/dodger.png", JIN_RES_PNG);

  JEL_COMPONENT_REGISTER(Sprite);

  player = JEL_entity_create();
  JEL_ENTITY_ADD(player, Sprite);
  JEL_ENTITY_SET(player, Sprite, animd, JIN_resm_get(&JIN_resm, "player_animation"));
  JEL_ENTITY_SET(player, Sprite, anim, 0);
  JEL_ENTITY_SET(player, Sprite, frame, 0);
  JEL_ENTITY_SET(player, Sprite, ticks, 0);*/

  return 0;
}

static int test_fn_destroy(struct STM_State *state)
{
  return 0;
}

static int test_fn_update(struct STM_State *state)
{
  //JIN_anim_update();
  return 0;
}

static int ticks = 0;
static int test_fn_draw(struct STM_State *state)
{
  /* glClear works but nothing else */
  //glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
  //glClear(GL_COLOR_BUFFER_BIT);
  
  unsigned int *shader;
  unsigned int *texture;

  /*
  shader = JIN_resm_get(&JIN_resm, "sprite_shader");
  texture = JIN_resm_get(&JIN_resm, "test_image");
  JIN_gfx_draw_sprite(shader, texture, 16, 16, 256, 256, 0, 0, 256, 256);
  JIN_gfx_draw_sprite(shader, texture, 16, 256, 256, 256, 32, 32, 224, 224);

  JIN_anim_draw();
*/
  /* Try and draw a triangle */
  //shader = JIN_resm_get(&JIN_resm, "test_shader");
  /* 3d fun */
  /*shader = JIN_resm_get(&JIN_resm, "3d_shader");
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
*/
  /*glUniformMatrix4fv(glGetUniformLocation(*shader, "model"), 1, GL_FALSE, (const float *) model);
  glUniformMatrix4fv(glGetUniformLocation(*shader, "view"), 1, GL_FALSE, (const float *) view);
  glUniformMatrix4fv(glGetUniformLocation(*shader, "projection"), 1, GL_FALSE, (const float *) projection);

  glUniform3f(glGetUniformLocation(*shader, "light_color"), 1.0f, 1.0f, 1.0f);
  glUniform3f(glGetUniformLocation(*shader, "light_pos"), 4.0f, 0.0f, -40.0f);

  glBindVertexArray(((struct JIN_Model *) JIN_resm_get(&JIN_resm, "3d_spaceship"))->vao);
  glDrawArrays(GL_TRIANGLES, 0, 78);
*/
  return 0;
}

int JIN_states_test_create(struct STM_State *state)
{
  if (STM_state_create(state, "test", 0, 0, test_fn_create, test_fn_destroy, test_fn_update, test_fn_draw)) return -1;

  return 0;
}

