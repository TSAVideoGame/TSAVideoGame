#include <JEL/jel.h>
#include "components/components.h"
#include "core/core.h"
#include "anim/anim.h"
#include "gfx/gfx.h"

extern JEL_Entity player;
extern JEL_Entity *tiles;
extern int        map_x;
extern void tile_collision_fn(JEL_Entity, JEL_Entity);
extern int  check_collision(int, int, int, int, int, int, int, int);

enum tflags {TL = 1, TR = 2, BL = 4, BR = 8};
static int player_collisions(void)
{
  struct Position player_pos;
  JEL_GET(player, Position, &player_pos);

  int tile_x = player_pos.x / TILE_SIZE;
  int tile_y = player_pos.y / TILE_SIZE;

  /* Check 4 tiles, tile_x/y is top left one */
  /* NOTE: NO BOUNDS CHECKING, make sure player never ends on edge */
  int indices[4];
  indices[0] = tile_y * map_x + tile_x;
  indices[1] = tile_y * map_x + tile_x + 1;
  indices[2] = (tile_y + 1) * map_x + tile_x;
  indices[3] = (tile_y + 1) * map_x + tile_x + 1;

  char flag = 0;

  struct Position pos_p, pos_t;
  struct AABB     col_p, col_t;
  JEL_GET(player, Position, &pos_p);
  JEL_GET(player, AABB, &col_p);
  for (int i = 0; i < 4; ++i) {
    if (JEL_CHECK(tiles[indices[i]], AABB)) {
      JEL_GET(tiles[indices[i]], Position, &pos_t);
      JEL_GET(tiles[indices[i]], AABB, &col_t);
      int collision = check_collision(pos_p.x, pos_p.y, col_p.w, col_p.h, pos_t.x, pos_t.y, col_t.w, col_t.h);
      if (col_t.on_collision == tile_collision_fn) {
        flag |= (1 << i);
      }
      else {
        if (collision) col_t.on_collision(tiles[indices[i]], player);
      }
    }
  }

  if (flag) {
    struct Physics phys_p;
    JEL_GET(player, Physics, &phys_p);
    struct Position one_t;

    switch (flag) {
      /* One tile cases */
      case TL:
        JEL_GET(tiles[indices[0]], Position, &one_t);
        if ((one_t.x + TILE_SIZE) - pos_p.x > (one_t.y + TILE_SIZE) - pos_p.y) {
          /* Y push */
          pos_p.y = (tile_y + 1) * TILE_SIZE;
          phys_p.y_vel = 0;
        }
        else {
          /* X push */
          pos_p.x = (tile_x + 1) * TILE_SIZE;
          phys_p.x_vel = 0;
        }
        break;
      case TR:
        JEL_GET(tiles[indices[1]], Position, &one_t);
        if ((pos_p.x + TILE_SIZE) - one_t.x > (one_t.y + TILE_SIZE) - pos_p.y) {
          /* Y push */
          pos_p.y = (tile_y + 1) * TILE_SIZE;
          phys_p.y_vel = 0;
        }
        else {
          /* X push */
          pos_p.x = tile_x * TILE_SIZE;
          phys_p.x_vel = 0;
        }
        break;
      case BL:
        JEL_GET(tiles[indices[2]], Position, &one_t);
        if ((one_t.x + TILE_SIZE) - pos_p.x > (pos_p.y + TILE_SIZE) - one_t.y) {
          /* Y push */
          pos_p.y = tile_y * TILE_SIZE;
          phys_p.y_vel = 0;
        }
        else {
          /* X push */
          pos_p.x = (tile_x + 1) * TILE_SIZE;
          phys_p.x_vel = 0;
        }
        break;
      case BR:
        JEL_GET(tiles[indices[3]], Position, &one_t);
        if ((pos_p.x + TILE_SIZE) - one_t.x > (pos_p.y + TILE_SIZE) - one_t.y) {
          /* Y push */
          pos_p.y = tile_y* TILE_SIZE;
          phys_p.y_vel = 0;
        }
        else {
          /* X push */
          pos_p.x = tile_x * TILE_SIZE;
          phys_p.x_vel = 0;
        }
        break;
      /* Two tile cases */
      case TL + TR:
        pos_p.y = (tile_y + 1) * TILE_SIZE;
        phys_p.y_vel = 0;
        break;
      case TL + BL:
        pos_p.x = (tile_x + 1) * TILE_SIZE;
        phys_p.x_vel = 0;
        break;
      case BL + BR:
        pos_p.y = tile_y * TILE_SIZE;
        phys_p.y_vel = 0;
        break;
      case TR + BR:
        pos_p.x = tile_x * TILE_SIZE;
        phys_p.x_vel = 0;
        break;
      /* Three tile cases */
      case TL + TR + BL:
        pos_p.x = (tile_x + 1) * TILE_SIZE;
        pos_p.y = (tile_y + 1) * TILE_SIZE;
        phys_p.x_vel = 0;
        phys_p.y_vel = 0;
        break;
      case TL + TR + BR:
        pos_p.x = tile_x * TILE_SIZE;
        pos_p.y = (tile_y + 1) * TILE_SIZE;
        phys_p.x_vel = 0;
        phys_p.y_vel = 0;
        break;
      case TL + BL + BR:
        pos_p.x = (tile_x + 1) * TILE_SIZE;
        pos_p.y = tile_y * TILE_SIZE;
        phys_p.x_vel = 0;
        phys_p.y_vel = 0;
        break;
      case TR + BL + BR:
        pos_p.x = tile_x * TILE_SIZE;
        pos_p.y = tile_y * TILE_SIZE;
        phys_p.x_vel = 0;
        phys_p.y_vel = 0;
        break;
    }

    JEL_SET_STRUCT(player, Position, pos_p);
    JEL_SET_STRUCT(player, Physics, phys_p);
  }

  /* Non tile collisions (like items and guards) */
  struct JEL_Query q;
  JEL_QUERY(q, Position, AABB);
  for (unsigned int t = 0; t < q.count; ++t) {
    struct JEL_EntityCIt e;
    struct PositionIt pos;
    struct AABBIt col;
    JEL_IT(e,   q.tables[t], JEL_EntityC);
    JEL_IT(pos, q.tables[t], Position);
    JEL_IT(col, q.tables[t], AABB);

    unsigned int count = q.tables[t]->count; /* Count will change */
    for (unsigned int i = 1; i < q.tables[t]->count; ++i) {
      if (e.entity[i] == player) continue;
      if (check_collision(pos_p.x, pos_p.y, col_p.w, col_p.h, pos.x[i], pos.y[i], col.w[i], col.h[i])) {
        col.on_collision[i](e.entity[i], player);
        if (q.tables[t]->count < count) {
          --i; /* Stay on i because last record swaps with the just deleted record */
          count = q.tables[t]->count;
        }
      }
    }
  }
  JEL_query_destroy(&q);

  return 0;
}

/*
 * Figure out a 'friction' for the player
 *
 * Equation is a - a(v^2 / vT^2)
 */
#define PLAYER_SLOW(dir) phys.dir##_accel = phys.dir##_vel / -2; JEL_SET_PROP(player, Physics, dir##_accel, phys.dir##_accel);
#define PLAYER_ACCEL(dir, sign) \
  phys.dir##_accel = sign * (accel - accel * ((phys.dir##_vel * phys.dir##_vel) / (max_vel * max_vel))); \
  JEL_SET_PROP(player, Physics, dir##_accel, phys.dir##_accel);
static int player_movement(void)
{
  float accel = .5;
  float max_vel = 5;

  struct Physics phys;
  JEL_GET(player, Physics, &phys);
  
  if (JIN_input.keys.a || JIN_input.keys.d) {
    if (JIN_input.keys.a) {
      JEL_SET_PROP(player, Sprite, dir, 1);
      if (phys.x_vel > 1) { PLAYER_SLOW(x); } else { PLAYER_ACCEL(x, -1); }
    }
    if (JIN_input.keys.d) {
      JEL_SET_PROP(player, Sprite, dir, 0);
      if (phys.x_vel < -1) { PLAYER_SLOW(x); } else { PLAYER_ACCEL(x, 1); }
    }
  }
  else {
    PLAYER_SLOW(x);
  }

  if (JIN_input.keys.w || JIN_input.keys.s) {
    if (JIN_input.keys.w) {
      if (phys.y_vel > 1) { PLAYER_SLOW(y); } else { PLAYER_ACCEL(y, -1); }
    }
    if (JIN_input.keys.s) {
      if (phys.y_vel < -1) { PLAYER_SLOW(y); } else { PLAYER_ACCEL(y, 1); }
    }
  }
  else {
    PLAYER_SLOW(y);
  }

  /* Animation */
  if (JIN_input.keys.w || JIN_input.keys.a || JIN_input.keys.s || JIN_input.keys.d) {
    JIN_anim_set(player, "RUN");
  }
  else {
    JIN_anim_set(player, "IDLE");
  }

  return 0;
}

