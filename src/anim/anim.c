#include "anim.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "../core/logger.h"
#include <string.h>

#include "../gfx/gfx.h"

JEL_COMPONENT_CREATE(Sprite, struct JIN_Animd *, animd, int32_t, anim, int32_t, ticks, int32_t, frame)

/* ANIMATION FUNCTIONS */

/*
 * JIN_animd_create
 *
 * @desc
 * @param animd
 * @param fpath
 * @return
 */
#define READ(var, count) \
    if (fread(var, sizeof(*var), count, file) != count) ERR_EXIT(-1, "Could not read from file");
int JIN_animd_create(struct JIN_Animd *animd, const char *fpath)
{
  FILE   *file;

  if (!(file = fopen(fpath, "rb"))) ERR_EXIT(-1, "Could not open file");
  READ(&animd->cols, 1);
  READ(&animd->rows, 1);
  READ(&animd->anim_nums, 1);

  if (!(animd->names       = malloc(animd->anim_nums * (8 * sizeof(char     ))))) ERR_EXIT(-1, "Out of memory");
  if (!(animd->frame_nums  = malloc(animd->anim_nums * (    sizeof(int32_t  ))))) ERR_EXIT(-1, "Out of memory");
  if (!(animd->ids         = malloc(animd->anim_nums * (    sizeof(int32_t *))))) ERR_EXIT(-1, "Out of memory");
  if (!(animd->frame_ticks = malloc(animd->anim_nums * (    sizeof(int32_t *))))) ERR_EXIT(-1, "Out of memory");

  /* Loop through every animation */
  for (int32_t i = 0; i < animd->anim_nums; ++i) {
    READ(&animd->names[i * 8], 8);
    READ(&animd->frame_nums[i], 1);
    if (!(animd->ids[i]         = malloc(animd->rows * animd->cols * animd->frame_nums[i] * sizeof(int32_t)))) ERR_EXIT(-1, "Out of memory");
    if (!(animd->frame_ticks[i] = malloc(                            animd->frame_nums[i] * sizeof(int32_t)))) ERR_EXIT(-1, "Out of memory");
    /* Loop through each frame */
    for (int32_t j = 0; j < animd->frame_nums[i]; ++j) {
      READ(&animd->ids[i][j * animd->rows * animd->cols], animd->rows * animd->cols);
      READ(&animd->frame_ticks[i][j], 1);
    }
  }

  fclose(file);

  return 0;
}

/*
 * JIN_animd_destroy
 *
 * @desc
 * @param animd
 * @return
 */
int JIN_animd_destroy(struct JIN_Animd *animd)
{
  free(animd->names);
  for (int32_t i = 0; i < animd->anim_nums; ++i) {
    free(animd->ids[i]);
    free(animd->frame_ticks[i]);
  }
  free(animd->frame_nums);
  free(animd->ids);
  free(animd->frame_ticks);

  return 0;
}

/* ECS FUNCTIONS */

/*
 * JIN_anim_update
 *
 * @desc
 * @return
 */
int JIN_anim_update(void)
{
  struct JEL_Query *q;
  JEL_QUERY(q, Sprite);

  for (JEL_ComponentInt i = 0; i < q->tables_num; ++i) {
    struct SpriteFragment *sprite;
    JEL_FRAGMENT_GET(sprite, q->tables[i], Sprite);

    for (JEL_EntityInt j = 0; j < q->tables[i]->num; ++j) {
      ++sprite->ticks[j];
      /* Check if done with this frame */
      if (sprite->ticks[j] >= sprite->animd[j]->frame_ticks[sprite->anim[j]][sprite->frame[j]]) {
        sprite->ticks[j] = 0;
        ++sprite->frame[j];
        /* Check if done with all the frames in the animation */
        if (sprite->frame[j] >= sprite->animd[j]->frame_nums[sprite->anim[j]]) {
          sprite->frame[j] = 0;
        }
      }
    }
  }

  JEL_query_destroy(q);

  return 0;
}

/*
 * JIN_anim_draw
 *
 * @desc
 * @return
 */
#include "../resm/resm.h"
#include "../core/globals.h"
int JIN_anim_draw(void)
{
  struct JEL_Query *q;
  JEL_QUERY(q, Sprite);

  unsigned int *shader;
  unsigned int *texture;

  shader = JIN_resm_get(&JIN_resm, "sprite_shader");
  texture = JIN_resm_get(&JIN_resm, "player_img");
  
  for (JEL_ComponentInt i = 0; i < q->tables_num; ++i) {
    struct SpriteFragment *sprite;
    JEL_FRAGMENT_GET(sprite, q->tables[i], Sprite);

    for (JEL_EntityInt j = 0; j < q->tables[i]->num; ++j) {
      /* Currently just uses 0x0 as the drawing coordinate */
      for (int rows = 0; rows < sprite->animd[j]->rows; ++rows) {
        for (int cols = 0; cols < sprite->animd[j]->cols; ++cols) {
          int32_t x = sprite->animd[j]->rows;
          int32_t y = sprite->animd[j]->cols;
          JIN_gfx_draw_sprite(shader, texture, (256 + 16) + rows * 16, 0 + cols * 16, 256, 256, 
              16 * sprite->animd[j]->ids[sprite->anim[j]][sprite->frame[j] * x * y + (rows * x) + cols], 0, 16, 16);
        }
      }
    }
  }

  JEL_query_destroy(q);

  return 0;
}

/*
 * JIN_anim_set
 *
 * @desc
 * @return
 */
int JIN_anim_set(JEL_Entity entity, const char *animation)
{
  struct JIN_Animd *data;
  JEL_ENTITY_GET(entity, Sprite, animd, data);

  for (int i = 0; i < data->anim_nums; ++i) {
    if (!strcmp(animation, &data->names[i * 8])) {
      int32_t anim_cur;
      JEL_ENTITY_GET(entity, Sprite, anim, anim_cur);
      if (anim_cur == i) return 0; /* Already using this animation */
      JEL_ENTITY_SET(entity, Sprite, anim, i);
      JEL_ENTITY_SET(entity, Sprite, ticks, 0);
      JEL_ENTITY_SET(entity, Sprite, frame, 0);
    }
  }

  /* Do nothing if no match */

  return 0;
}
