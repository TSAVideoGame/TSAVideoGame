#ifndef JIN_SPRITE_H
#define JIN_SPRITE_H

#include <JEL/jel.h>

JEL_COMPONENT_DEFINE(Sprite, int, z, int, w, int, h, int, tx, int, ty, int, tw, int, th);
JEL_COMPONENT_EXTERN(Sprite);

int JIN_gfx_sprite_init(void);
int JIN_gfx_sprite_quit(void);

int JIN_gfx_sprite_draw(void);

#endif
