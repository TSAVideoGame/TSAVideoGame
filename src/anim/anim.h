#ifndef JIN_ANIM_H
#define JIN_ANIM_H

#include <stdint.h>
#include <JEL/jel.h>

/*
 * ANIMATION
 *
 * 2D animations
 * There is animation data and a JEL
 * component
 *
 * TYPES:
 * struct JIN_Animd | Animation data
 */
struct JIN_Animd {
  /* Information for meta sprites */
  int32_t   rows;
  int32_t   cols;
  /* Acutal info */
  int32_t   anim_nums;
  char     *names;       /* Name of the animation */
  int32_t  *frame_nums;  /* Number of frames in each animation */
  int32_t **ids;         /* Sprite ids */
  int32_t **frame_ticks; /* Frame durations */
};

JEL_COMPONENT_DEFINE(Sprite, struct JIN_Animd *, animd, int32_t, anim, int32_t, ticks, int32_t, frame)
JEL_COMPONENT_EXTERN(Sprite)

/*
 * Animation functions
 *
 * create  |
 * destroy |
 */
int JIN_animd_create (struct JIN_Animd *animd, const char *fpath);
int JIN_animd_destroy(struct JIN_Animd *animd);

/*
 * ECS functions
 *
 * update | Game loop update for animations
 * draw   | Game loop draw for animations
 * set    | Set the animation to play
 */
int JIN_anim_update(void);
int JIN_anim_draw(void);
int JIN_anim_set(JEL_Entity entity, const char *animation);

#endif
