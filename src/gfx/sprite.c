#include "sprite.h"
#include "components/components.h"
#include "resm/resm.h"
#include "core/gll/gll.h"
#include "cglm/cglm.h"
#include "core/globals.h"

#include "core/logger/logger.h"

#define MAX_SPRITES 1000000
#define MAX_Z  10000.0f
#define MIN_Z -10000.0f

/*
 * SPRITE
 *
 * Make sure all sprites are on one spritesheet
 *
 * VBO Layout: (float)
 *
 * x, y, z, tx, ty
 * ---------------
 * x, y   | Positions
 * z      | Z axis
 * tx, ty | Texture position
 */
#define VERTEX_ATTRIBS 5

unsigned int sprite_vao;
unsigned int sprite_vbo;
unsigned int sprite_ebo;
unsigned int sprite_indices[MAX_SPRITES * 6];

int JIN_gfx_sprite_init(void)
{
  /* Set up indices */
  for (unsigned int i = 0; i < MAX_SPRITES; ++i) {
    sprite_indices[i * 6 + 0] = i * 4;
    sprite_indices[i * 6 + 1] = i * 4 + 1;
    sprite_indices[i * 6 + 2] = i * 4 + 3;
    sprite_indices[i * 6 + 3] = i * 4 + 2;
    sprite_indices[i * 6 + 4] = i * 4 + 1;
    sprite_indices[i * 6 + 5] = i * 4 + 3;
  }

  /* Resources */
  JIN_resm_add("sprite_shader", "res/shaders/sprite.shdr", RESM_SHADER);
  JIN_resm_add("spritesheet", "res/images/spritesheet.png", RESM_PNG);

  unsigned int *shader = JIN_resm_get("sprite_shader");

  glUseProgram(*shader);

  mat4 projection;
  glm_ortho(0.0f, (float) WINDOW_WIDTH, (float) WINDOW_HEIGHT, 0.0f, MIN_Z, MAX_Z, projection);
  glUniformMatrix4fv(glGetUniformLocation(*shader, "projection"), 1, GL_FALSE, (float *) projection);
  
  /* GL objects */
  glGenVertexArrays(1, &sprite_vao);
  glGenBuffers(1, &sprite_vbo);
  glGenBuffers(1, &sprite_ebo);

  /* Orphaning/Buffer re-specification used */
  glBindVertexArray(sprite_vao);
  glBindBuffer(GL_ARRAY_BUFFER, sprite_vbo);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, MAX_SPRITES * 6 * sizeof(unsigned int), sprite_indices, GL_STATIC_DRAW);

  /* Position, Texture */
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_ATTRIBS * sizeof(float), (void *) 0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, VERTEX_ATTRIBS * sizeof(float), (void *) (3 * sizeof(float)));

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
 
  return 0;
}

int JIN_gfx_sprite_quit(void)
{
  /* GL objects */
  glBindVertexArray(0);
  
  glDeleteBuffers(1, &sprite_vbo);
  glDeleteBuffers(1, &sprite_ebo);
  glDeleteVertexArrays(1, &sprite_vao);

  return 0;
}

/*
 * JIN_gfx_sprite_draw
 *
 * @desc
 *   Batch draw all entities with a sprite
 *   and position component
 *   Uses a default shader and a default
 *   texture
 * @return
 *   Success
 */
static int prepare_buffer(void *buffer, int x, int y)
{
  struct JEL_Query q;
  JEL_QUERY(q, Position, Sprite);
  
  unsigned int sprite_num = 0;

  /* Prepare the vbo */
  for (unsigned int t = 0; t < q.count; ++t) {
    struct PositionIt pos;
    struct SpriteIt sprite;
    JEL_IT(pos, q.tables[t], Position);
    JEL_IT(sprite, q.tables[t], Sprite);

    // TODO: Loop is not parallelized
    if (sprite_num > MAX_SPRITES)
      break;

    // TODO: Skip sprites outside of the screen
    for (JEL_EntityInt i = 1; i < q.tables[t]->count; ++i) {
      if (pos.x[i] + sprite.w[i] < x || pos.x[i] > x + WINDOW_WIDTH ||
          pos.y[i] + sprite.h[i] < y || pos.y[i] > y + WINDOW_HEIGHT)
        continue;

      #define SPRITE_BUFFER_IDX(index) ((float *) buffer)[(sprite_num * VERTEX_ATTRIBS * 4) + index]
      /* Top right */
      SPRITE_BUFFER_IDX( 0) = pos.x[i] + sprite.w[i] - x;
      SPRITE_BUFFER_IDX( 1) = pos.y[i]               - y;
      SPRITE_BUFFER_IDX( 2) = sprite.z[i];
      SPRITE_BUFFER_IDX( 3) = sprite.tx[i] + sprite.tw[i] - sprite.tw[i] * sprite.dir[i];
      SPRITE_BUFFER_IDX( 4) = sprite.ty[i];

      /* Bottom right */
      SPRITE_BUFFER_IDX( 5) = pos.x[i] + sprite.w[i] - x;
      SPRITE_BUFFER_IDX( 6) = pos.y[i] + sprite.h[i] - y;
      SPRITE_BUFFER_IDX( 7) = sprite.z[i];
      SPRITE_BUFFER_IDX( 8) = sprite.tx[i] + sprite.tw[i] - sprite.tw[i] * sprite.dir[i];
      SPRITE_BUFFER_IDX( 9) = sprite.ty[i] + sprite.th[i];

      /* Bottom left */
      SPRITE_BUFFER_IDX(10) = pos.x[i]               - x;
      SPRITE_BUFFER_IDX(11) = pos.y[i] + sprite.h[i] - y;
      SPRITE_BUFFER_IDX(12) = sprite.z[i];
      SPRITE_BUFFER_IDX(13) = sprite.tx[i] + sprite.tw[i] * sprite.dir[i];
      SPRITE_BUFFER_IDX(14) = sprite.ty[i] + sprite.th[i];

      /* Top left */
      SPRITE_BUFFER_IDX(15) = pos.x[i] - x;
      SPRITE_BUFFER_IDX(16) = pos.y[i] - y;
      SPRITE_BUFFER_IDX(17) = sprite.z[i];
      SPRITE_BUFFER_IDX(18) = sprite.tx[i] + sprite.tw[i] * sprite.dir[i];
      SPRITE_BUFFER_IDX(19) = sprite.ty[i];

      // TODO: Loop is not parallelized
      if (++sprite_num > MAX_SPRITES)
        break;
    }
  }

  JEL_query_destroy(&q);

  return sprite_num;
}

int JIN_gfx_sprite_draw(int x, int y)
{
  unsigned int *shader = JIN_resm_get("sprite_shader");
  unsigned int *texture = JIN_resm_get("spritesheet");

  /* OpenGL Drawing stuff */
  glUseProgram(*shader);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, *texture);

  glBindVertexArray(sprite_vao);
  glBindBuffer(GL_ARRAY_BUFFER, sprite_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * VERTEX_ATTRIBS * MAX_SPRITES * 4, NULL, GL_DYNAMIC_DRAW);
  GLfloat *buffer = (GLfloat *) glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(float) * VERTEX_ATTRIBS * MAX_SPRITES * 4, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

  int sprite_num = prepare_buffer(buffer, x, y);

  glUnmapBuffer(GL_ARRAY_BUFFER);

  glDrawElements(GL_TRIANGLES, sprite_num * 6, GL_UNSIGNED_INT, 0);

  glBindVertexArray(0);

  return 0;  
}
