#include <JEL/jel.h>
#include "components/components.h"
#include "gfx/gfx.h"
#include "core/core.h"

extern char *map_collisions;
extern int map_x;
extern JEL_Entity player;

static void update_guard(void)
{
  struct JEL_Query q;
  JEL_QUERY(q, Guard);
  for (unsigned int t = 0; t < q.count; ++t) {
    struct GuardIt guard;
    struct JEL_EntityCIt e;
    JEL_IT(e,     q.tables[t], JEL_EntityC);
    JEL_IT(guard, q.tables[t], Guard);

    for (unsigned int i = 1; i < q.tables[t]->count; ++i) {
      ++guard.ticks[i];
      switch (guard.state[i]) {
        case 0:
          guard.patrol[i](e.entity[i], player);
          break;
        case 1:
          break;
        case 2:
          break;
        default: break;
      }
    }
  }
  JEL_query_destroy(&q);
}

static void guard_patrol_vertical(JEL_Entity guard, JEL_Entity player)
{
  struct Position guard_pos; JEL_GET(guard, Position, &guard_pos);
  struct Physics guard_phys; JEL_GET(guard, Physics, &guard_phys);

  int tile_x = guard_pos.x / TILE_SIZE;
  int tile_y = guard_pos.y / TILE_SIZE;

  /* Check 4 tiles, tile_x/y is top left one */
  /* NOTE: NO BOUNDS CHECKING, make sure player never ends on edge */
  int indices[4];
  indices[0] = tile_y * map_x + tile_x;
  indices[1] = tile_y * map_x + tile_x + 1;
  indices[2] = (tile_y + 1) * map_x + tile_x;
  indices[3] = (tile_y + 1) * map_x + tile_x + 1;

  if (map_collisions[indices[2]]) {
    guard_phys.y_vel = -5;
    guard_pos.y = tile_y * TILE_SIZE;
  }
  if (map_collisions[indices[0]]) {
    guard_phys.y_vel = 5; 
    guard_pos.y = (tile_y + 1) * TILE_SIZE;
  }

  JEL_SET_STRUCT(guard, Physics, guard_phys);
  JEL_SET_STRUCT(guard, Position, guard_pos);
}
static void guard_patrol_horizontal(JEL_Entity guard, JEL_Entity player)
{
  struct Position guard_pos; JEL_GET(guard, Position, &guard_pos);
  struct Physics guard_phys; JEL_GET(guard, Physics, &guard_phys);
  struct Sprite guard_sprite; JEL_GET(guard, Sprite, &guard_sprite);

  int tile_x = guard_pos.x / TILE_SIZE;
  int tile_y = guard_pos.y / TILE_SIZE;

  /* Check 4 tiles, tile_x/y is top left one */
  /* NOTE: NO BOUNDS CHECKING, make sure player never ends on edge */
  int indices[4];
  indices[0] = tile_y * map_x + tile_x;
  indices[1] = tile_y * map_x + tile_x + 1;
  indices[2] = (tile_y + 1) * map_x + tile_x;
  indices[3] = (tile_y + 1) * map_x + tile_x + 1;

  if (map_collisions[indices[1]]) {
    guard_phys.x_vel = -5;
    guard_pos.x = tile_x * TILE_SIZE;
    if (guard_sprite.dir != 1) {
      JEL_SET_PROP(guard, Sprite, dir, 1);
    }
  }
  if (map_collisions[indices[0]]) {
    guard_phys.x_vel = 5; 
    guard_pos.x = (tile_x + 1) * TILE_SIZE;
    if (guard_sprite.dir != 0) {
      JEL_SET_PROP(guard, Sprite, dir, 0);
    }
  }

  JEL_SET_STRUCT(guard, Physics, guard_phys);
  JEL_SET_STRUCT(guard, Position, guard_pos);
}

