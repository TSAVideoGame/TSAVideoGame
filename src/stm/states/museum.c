#include "stm/stm.h"
#include <JEL/jel.h>
#include "resm/resm.h"
#include "gfx/gfx.h"
#include "components/components.h"
#include "core/core.h"

#define ASCII_0 48

#define TILE_SIZE 32

/*
 * MUSEUM
 *
 * This is a museum level, whether
 * it's an actual level or procedurally
 * generated shouldn't matter
 */
static int test_map_meta[] = {7, 7};
static char *test_map = 
  "000 000 000 000 000 000 000 "
  "000 100 100 100 100 100 000 "
  "000 100 100 100 100 100 000 "
  "000 200 200 200 200 200 000 "
  "000 200 200 200 200 200 000 "
  "000 200 200 200 200 200 000 "
  "000 000 000 000 000 000 000 "
;
static int map_x, map_y;
static JEL_Entity *tiles;
static JEL_Entity player;

static int museum_fn_create(struct STM_S *state)
{
  map_x = test_map_meta[0];
  map_y = test_map_meta[1];

  tiles = malloc(sizeof(JEL_Entity) * map_x * map_y);

  unsigned int *sprite = JIN_resm_get("tiles");
  
  for (int i = 0; i < map_x * map_y; ++i) {
    tiles[i] = JEL_entity_create();
    JEL_ENTITY_ADD(tiles[i], Position);
    JEL_ENTITY_SET(tiles[i], Position, x, (i % map_y) * TILE_SIZE);
    JEL_ENTITY_SET(tiles[i], Position, y, (i / map_y) * TILE_SIZE);


    JEL_ENTITY_ADD(tiles[i], Sprite);
    JEL_ENTITY_SET(tiles[i], Sprite, z, 0);
    JEL_ENTITY_SET(tiles[i], Sprite, w, TILE_SIZE);
    JEL_ENTITY_SET(tiles[i], Sprite, h, TILE_SIZE);
    JEL_ENTITY_SET(tiles[i], Sprite, tx, (test_map[0] - ASCII_0) * 32);
    JEL_ENTITY_SET(tiles[i], Sprite, ty, 16);
    JEL_ENTITY_SET(tiles[i], Sprite, tw, 32);
    JEL_ENTITY_SET(tiles[i], Sprite, th, 32);

    test_map += 4;
  }
  test_map -= (map_x * map_y - 1) * 4;

  player = JEL_entity_create();
  JEL_ENTITY_ADD(player, Position);
  JEL_ENTITY_SET(player, Position, x, 256);
  JEL_ENTITY_SET(player, Position, y, 256);

  JEL_ENTITY_ADD(player, Physics);
  JEL_ENTITY_SET(player, Physics, x_accel, 0);
  JEL_ENTITY_SET(player, Physics, y_accel, 0);
  JEL_ENTITY_SET(player, Physics, x_vel, 0);
  JEL_ENTITY_SET(player, Physics, y_vel, 0);

  JEL_ENTITY_ADD(player, Sprite);
  sprite = JIN_resm_get("player_img");
  JEL_ENTITY_SET(player, Sprite, z, 1);
  JEL_ENTITY_SET(player, Sprite, w, TILE_SIZE);
  JEL_ENTITY_SET(player, Sprite, h, TILE_SIZE);
  JEL_ENTITY_SET(player, Sprite, tx, 0);
  JEL_ENTITY_SET(player, Sprite, ty, 0);
  JEL_ENTITY_SET(player, Sprite, tw, 16);
  JEL_ENTITY_SET(player, Sprite, th, 16);

  return 0;
}

static int museum_fn_destroy(struct STM_S *state)
{
  for (int i = 0; i < map_x * map_y; ++i) {
    JEL_entity_destroy(tiles[i]);
  }

  free(tiles);

  JEL_entity_destroy(player);
  
  return 0;
}

/*
 * Probably put player movement into a different function
 * Figure out a 'friction' for the player
 *
 * Equation is a - a(v^2 / vT^2)
 */
static int player_movement(void)
{
  float accel = .5;
  float max_vel = 5;

  float x_vel;
  JEL_ENTITY_GET(player, Physics, x_vel, x_vel);
  if (JIN_input.keys.a || JIN_input.keys.d) {
    if (JIN_input.keys.a) {
      JEL_ENTITY_SET(player, Physics, x_accel, -1 * accel + accel * ((x_vel * x_vel) / (max_vel * max_vel)));
    }
    else {
      JEL_ENTITY_SET(player, Physics, x_accel, accel - accel * ((x_vel * x_vel) / (max_vel * max_vel)));
    }
  }
  else {
    JEL_ENTITY_SET(player, Physics, x_accel, x_vel / -2);
  }

  float y_vel;
  JEL_ENTITY_GET(player, Physics, y_vel, y_vel);
  if (JIN_input.keys.w || JIN_input.keys.s) {
    if (JIN_input.keys.w) {
      JEL_ENTITY_SET(player, Physics, y_accel, -1 * accel + accel * ((y_vel * y_vel) / (max_vel * max_vel)));
    }
    else {
      JEL_ENTITY_SET(player, Physics, y_accel, accel - accel * ((y_vel * y_vel) / (max_vel * max_vel)));
    }
  }
  else {
    JEL_ENTITY_SET(player, Physics, y_accel, y_vel / -2);
  }

  return 0;
}

static int museum_fn_update(struct STM_S *state)
{
  player_movement();

  struct JEL_Query *q;
  JEL_QUERY(q, Position, Physics);

  for (JEL_ComponentInt i = 0; i < q->tables_num; ++i) {
    struct PositionFragment *pos;
    struct PhysicsFragment *phys;
    JEL_FRAGMENT_GET(pos, q->tables[i], Position);
    JEL_FRAGMENT_GET(phys, q->tables[i], Physics);

    for (JEL_EntityInt j = 0; j < q->tables[i]->num; ++j) {
      /* Explicit Euler Integration ;) */
      phys->x_vel[j] += phys->x_accel[j];
      pos->x[j] += phys->x_vel[j];
      
      phys->y_vel[j] += phys->y_accel[j];
      pos->y[j] += phys->y_vel[j];
    }
  }

  JEL_query_destroy(q);
  
  return 0;
}

static int museum_fn_draw(struct STM_S *state)
{
  JIN_gfx_sprite_draw();

  return 0;
}

int JIN_states_create_museum(struct STM_S *state)
{
  if (STM_s_create(state, 0, museum_fn_create, museum_fn_destroy, museum_fn_update, museum_fn_draw)) return -1;
  return 0;
}

