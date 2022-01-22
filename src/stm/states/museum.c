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
 *
 * Make sure you set these variables below before
 * you queue/push this state
 */
int  *map_meta;
char *map_tiles;
char *map_items;
char *map_collisions;

static int map_x, map_y;
static JEL_Entity *tiles;
static JEL_Entity player;
static struct { int x; int y; } camera;

/* Collision functions */
static int  check_collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
  return (x1 < x2 + w2 &&
          x1 + w1 > x2 &&
          y1 < y2 + h2 &&
          y1 + h1 > y2);
}
static void dummy_collision_fn(JEL_Entity self, JEL_Entity other) { return; }
static void tile_collision_fn(JEL_Entity tile, JEL_Entity other)
{

}
static void door_collision_fn(JEL_Entity tile, JEL_Entity other)
{
  if (JIN_input.keys.o) {
    JIN_stm_queue("LVL_SEL", 0);
  }
}
static void artifact_collision_fn(JEL_Entity item, JEL_Entity other)
{
  JEL_entity_destroy(item);
}
static void guard_collision_fn(JEL_Entity guard, JEL_Entity player)
{
  JIN_stm_queue("LVL_SEL", 0);
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
    guard_phys.y_vel -= 5; 
  }
  if (map_collisions[indices[0]]) {
    guard_phys.y_vel += 5; 
  }

  JEL_SET_STRUCT(guard, Physics, guard_phys);
}
static void guard_patrol_horizontal(JEL_Entity guard, JEL_Entity player)
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

  if (map_collisions[indices[1]]) {
    guard_phys.x_vel -= 5; 
  }
  if (map_collisions[indices[0]]) {
    guard_phys.x_vel += 5; 
  }

  JEL_SET_STRUCT(guard, Physics, guard_phys);
}
static int museum_fn_create(struct STM_S *state)
{
  map_x = map_meta[0];
  map_y = map_meta[1];

  tiles = malloc(sizeof(JEL_Entity) * map_x * map_y);
 
  int spawn_x, spawn_y;

  for (int i = 0; i < map_x * map_y; ++i) {
    tiles[i] = JEL_entity_create();
    JEL_SET(tiles[i], Position, (i % map_x) * TILE_SIZE, (i / map_x) * TILE_SIZE);
    JEL_SET(tiles[i], Sprite, 0, TILE_SIZE, TILE_SIZE, map_tiles[i] * 32, 16, 32, 32, 0);
    /* Collision */
    int coltype = map_collisions[i];
    if (coltype) {
      void (*col_fn)(JEL_Entity, JEL_Entity);
      switch (coltype) {
        case 1:
          col_fn = tile_collision_fn;
          break;
        case 2:
          col_fn = door_collision_fn;
          break;
        default:
          col_fn = NULL;
          break;
      }
      if (col_fn) {
        JEL_SET(tiles[i], AABB, TILE_SIZE, TILE_SIZE, col_fn);
      }
    }
    JEL_Entity new_item;
    /* Item */
    switch (map_items[i]) {
      case 1: /* Player spawn */
        spawn_x = i % map_x * TILE_SIZE;
        spawn_y = i / map_x * TILE_SIZE;
        break;
      case 2: /* Artifact */
        new_item = JEL_entity_create();
        JEL_SET(new_item, Position, i % map_x * TILE_SIZE, i / map_x * TILE_SIZE);
        JEL_SET(new_item, Sprite, 1, TILE_SIZE, TILE_SIZE, 160, 16, 32, 32, 0);
        JEL_SET(new_item, AABB, TILE_SIZE, TILE_SIZE, artifact_collision_fn);
        break;
      case 3: /* Guard */
        new_item = JEL_entity_create();
        JEL_SET(new_item, Position, i % map_x * TILE_SIZE, i / map_x * TILE_SIZE);
        JEL_SET(new_item, Sprite, 1, TILE_SIZE, TILE_SIZE, 32, 0, 16, 16, 0);
        JEL_SET(new_item, AABB, TILE_SIZE, TILE_SIZE, guard_collision_fn);
        JEL_SET(new_item, Physics, 0, 5, 0, 0);
        JEL_SET(new_item, Guard, 0, 0, guard_patrol_vertical, dummy_collision_fn, dummy_collision_fn);
        break;
      case 4: /* Guard */
        new_item = JEL_entity_create();
        JEL_SET(new_item, Position, i % map_x * TILE_SIZE, i / map_x * TILE_SIZE);
        JEL_SET(new_item, Sprite, 1, TILE_SIZE, TILE_SIZE, 32, 0, 16, 16, 0);
        JEL_SET(new_item, AABB, TILE_SIZE, TILE_SIZE, guard_collision_fn);
        JEL_SET(new_item, Physics, 5, 0, 0, 0);
        JEL_SET(new_item, Guard, 0, 0, guard_patrol_horizontal, dummy_collision_fn, dummy_collision_fn);
        break;
      default: break;
    }
  }

  player = JEL_entity_create();
  JEL_SET(player, Position, spawn_x, spawn_y);
  JEL_SET(player, Physics, 0, 0, 0, 0);
  JEL_SET(player, Sprite, 1, TILE_SIZE, TILE_SIZE, 0, 0, 16, 16, 0);
  JEL_SET(player, Animation, (struct JIN_Animd *) JIN_resm_get("player_animation"), 1, 0, 0);
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


  struct JEL_Query q;
  JEL_QUERY(q, Guard);
  for (unsigned int t = 0; t < q.count; ++t) {
    struct JEL_EntityCIt e;
    JEL_IT(e, q.tables[t], JEL_EntityC);

    for (unsigned int i = 1; q.tables[t]->count > 1;) {
      JEL_entity_destroy(e.entity[i]);
    }
  }
  JEL_query_destroy(&q);
  /* Not ideal but it works for now */
  //struct JEL_Query q;
  JEL_QUERY(q, Sprite);
  for (unsigned int t = 0; t < q.count; ++t) {
    struct JEL_EntityCIt e;
    JEL_IT(e, q.tables[t], JEL_EntityC);

    for (unsigned int i = 1; q.tables[t]->count > 1;) {
      JEL_entity_destroy(e.entity[i]);
    }
  }
  JEL_query_destroy(&q);

  return 0;
}

/*
void handleAlarm(int sig) { // Handles the signal alarm
    if (mv >= 5) {
        seconds = 0;
    }
    seconds++;
    //printf("\rElapsed time : %f", seconds);
    alarm(1); // Sends alarm signal after one second 
}
int player_time(float mv) {
    float seconds = 0;
    setbuf(stdout, NULL);

    //printf("\rElapsed time : %f", seconds);
    signal(SIGALRM, handle); // Assigns a handler for the alarm signal 
    alarm(1); // Sends alarm signal after one second 
    while (1); // Prevents the process from terminating 
    return seconds;

}
*/

/*
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
      //phys.x_vel = -1 * player_time(max_vel) * phys.x_accel;
      JEL_SET_PROP(player, Physics, x_accel, phys.x_accel);
      JEL_SET_PROP(player, Physics, x_vel, phys.x_vel);
    }
    if (JIN_input.keys.d) {
        JEL_SET_PROP(player, Sprite, dir, 0);
        phys.x_accel = accel - accel * ((phys.x_vel * phys.x_vel) / (max_vel * max_vel));
        //phys.x_vel = player_time(max_vel) * phys.x_accel;
        JEL_SET_PROP(player, Physics, x_accel, phys.x_accel);
        JEL_SET_PROP(player, Physics, x_vel, phys.x_vel);
    }
  }
  else {
    phys.x_accel = phys.x_vel / -2;
    JEL_SET_PROP(player, Physics, x_accel, phys.x_accel);
  }

  if (JIN_input.keys.w || JIN_input.keys.s) {
    if (JIN_input.keys.w) {
      phys.y_accel = -1 * accel + accel * ((phys.y_vel * phys.y_vel) / (max_vel * max_vel));
      //phys.y_vel = player_time(max_vel) * phys.y_accel;
      JEL_SET_PROP(player, Physics, y_accel, phys.y_accel);
      JEL_SET_PROP(player, Physics, y_vel, phys.y_vel);
    }
    if (JIN_input.keys.s) {
      phys.y_accel = accel - accel * ((phys.y_vel * phys.y_vel) / (max_vel * max_vel));
      //phys.y_vel = player_time(max_vel) * phys.y_accel;
      JEL_SET_PROP(player, Physics, y_accel, phys.y_accel);
      JEL_SET_PROP(player, Physics, y_vel, phys.y_vel);
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
    
    switch (flag) {
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
    for (unsigned int i = 0; i < q.tables[t]->count; ++i) {
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

static void update_camera(void)
{
  struct Position pos;
  JEL_GET(player, Position, &pos);
  camera.x = pos.x + TILE_SIZE / 2 - WINDOW_WIDTH / 2;
  camera.y = pos.y + TILE_SIZE / 2 - WINDOW_HEIGHT / 2;
}

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

static int museum_fn_update(struct STM_S *state)
{
  JIN_anim_update();
  
  player_movement();
  update_guard();

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

  update_camera();

  if (JIN_input.keys.p == 1) {
    JIN_stm_queue("PAUSE", STM_PERSIST_PREV);
  }

  return 0;
}

static int museum_fn_draw(struct STM_S *state)
{
  JIN_gfx_sprite_draw(camera.x, camera.y);

  return 0;
}

int JIN_states_create_museum(struct STM_S *state)
{
  if (STM_s_create(state, 0, museum_fn_create, museum_fn_destroy, museum_fn_update, museum_fn_draw)) return -1;
  return 0;
}

