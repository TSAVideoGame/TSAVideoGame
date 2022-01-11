#include "stm/stm.h"
#include <JEL/jel.h>
#include "resm/resm.h"
#include "gfx/gfx.h"
#include "components/components.h"
#include "core/core.h"
#include "anim/anim.h"

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
  "001 001 001 001 001 001 001 "
  "001 101 101 101 101 101 001 "
  "001 100 100 100 100 100 001 "
  "001 200 200 200 200 200 001 "
  "001 200 210 200 200 200 001 "
  "001 200 200 200 200 200 001 "
  "001 001 001 001 001 001 001 "
;
static int map_x, map_y;
static JEL_Entity *tiles;
static JEL_Entity player;

/* Collision functions */
static void dummy_collision_fn(JEL_Entity self, JEL_Entity other) { return; }
static void tile_collision_fn(JEL_Entity tile, JEL_Entity other)
{

}

static int museum_fn_create(struct STM_S *state)
{
  map_x = test_map_meta[0];
  map_y = test_map_meta[1];

  tiles = malloc(sizeof(JEL_Entity) * map_x * map_y);
 
  int spawn_x, spawn_y;

  for (int i = 0; i < map_x * map_y; ++i) {
    tiles[i] = JEL_entity_create();
    JEL_SET(tiles[i], Position, (i % map_y) * TILE_SIZE, (i / map_y) * TILE_SIZE);
    JEL_SET(tiles[i], Sprite, 0, TILE_SIZE, TILE_SIZE, (test_map[0] - ASCII_0) * 32, 16, 32, 32, 0);
    /* Collision */
    int coltype = test_map[2] - ASCII_0;
    if (coltype) {
      void (*col_fn)(JEL_Entity, JEL_Entity);
      switch (coltype) {
        case 1:
          col_fn = tile_collision_fn;
          break;
        default:
          col_fn = dummy_collision_fn;
          break;
      }
      JEL_SET(tiles[i], AABB, TILE_SIZE, TILE_SIZE, col_fn);
    }
    /* Item */
    switch (test_map[1] - ASCII_0) {
      case 1:
        spawn_x = i % map_y * TILE_SIZE;
        spawn_y = i / map_y * TILE_SIZE;
        break;
      default: break;
    }

    test_map += 4;
  }
  test_map -= (map_x * map_y - 1) * 4;

  player = JEL_entity_create();
  JEL_SET(player, Position, spawn_x, spawn_y);
  JEL_SET(player, Physics, 0, 0, 0, 0);
  JEL_SET(player, Sprite, 1, TILE_SIZE, TILE_SIZE, 0, 0, 16, 16, 0);
  JEL_SET(player, Animation, (struct JIN_Animd *) JIN_resm_get("player_animation"), 1, 0, 0,);
  JEL_SET(player, AABB, TILE_SIZE, TILE_SIZE, dummy_collision_fn);

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

  struct Physics phys;
  JEL_GET(player, Physics, &phys);
  if (JIN_input.keys.a || JIN_input.keys.d) {
    if (JIN_input.keys.a) {
      JEL_SET_PROP(player, Sprite, dir, 1);
      phys.x_accel = -1 * accel + accel * ((phys.x_vel * phys.x_vel) / (max_vel * max_vel));
      JEL_SET_PROP(player, Physics, x_accel, phys.x_accel);
    }
    else {
      JEL_SET_PROP(player, Sprite, dir, 0);
      phys.x_accel = accel - accel * ((phys.x_vel * phys.x_vel) / (max_vel * max_vel));
      JEL_SET_PROP(player, Physics, x_accel, phys.x_accel);
    }
  }
  else {
    phys.x_accel = phys.x_vel / -2;
    JEL_SET_PROP(player, Physics, x_accel, phys.x_accel);
  }

  if (JIN_input.keys.w || JIN_input.keys.s) {
    if (JIN_input.keys.w) {
      phys.y_accel = -1 * accel + accel * ((phys.y_vel * phys.y_vel) / (max_vel * max_vel));
      JEL_SET_PROP(player, Physics, y_accel, phys.y_accel);
    }
    else {
      phys.y_accel = accel - accel * ((phys.y_vel * phys.y_vel) / (max_vel * max_vel));
      JEL_SET_PROP(player, Physics, y_accel, phys.y_accel);
    }
  }
  else {
    phys.y_accel = phys.y_vel / -2;
    JEL_SET_PROP(player, Physics, y_accel, phys.y_accel);
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

  for (int i = 0; i < 4; ++i) {
    if (JEL_CHECK(tiles[indices[i]], AABB)) {
      struct AABB col;
      JEL_GET(tiles[indices[i]], AABB, &col);
      col.on_collision(tiles[indices[i]], player);
    }
  }

  return 0;
}

static int museum_fn_update(struct STM_S *state)
{
  JIN_anim_update();
  
  player_movement();

  struct JEL_Query q;
  JEL_QUERY(q, Position, Physics);

  for (unsigned int i = 0; i < q.count; ++i) {
    struct PositionIt pos;
    struct PhysicsIt phys;
    JEL_IT(pos, q.tables[i], Position);
    JEL_IT(phys, q.tables[i], Physics);

    for (JEL_EntityInt j = 1; j < q.tables[i]->count; ++j) {
      /* Explicit Euler Integration ;) */
      phys.x_vel[j] += phys.x_accel[j];
      pos.x[j] += phys.x_vel[j];
      
      phys.y_vel[j] += phys.y_accel[j];
      pos.y[j] += phys.y_vel[j];
    }
  }

  JEL_query_destroy(&q);
 
  player_collisions();

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

