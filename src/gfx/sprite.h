#ifndef JIN_SPRITE_H
#define JIN_SPRITE_H

#include <JEL/jel.h>

struct Sprite {
  int z;
  int w;
  int h;
  int tx;
  int ty;
  int tw;
  int th;
};
JEL_COMPONENT(Sprite, z, w, h, tx, ty, tw, th);

int JIN_gfx_sprite_init(void);
int JIN_gfx_sprite_quit(void);

int JIN_gfx_sprite_draw(void);

#endif
