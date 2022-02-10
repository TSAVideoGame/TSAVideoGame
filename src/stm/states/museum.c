#include "stm/stm.h"
#include <JEL/jel.h>
#include "resm/resm.h"
#include "gfx/gfx.h"
#include "components/components.h"
#include "core/core.h"
#include "anim/anim.h"
#include "core/gll/gll.h"

#define ASCII_0 48

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
int   map_meta[2];
char *map_tiles;
char *map_items;
char *map_collisions;

static int map_x, map_y;
static JEL_Entity *tiles;
static JEL_Entity player;
static struct Camera camera;
static int colliding_door;
static JEL_Entity tooltip;

extern int artifacts_total;
extern int artifacts_collected;

/* Modules */
#include "modules/guards.c"
#include "modules/lighting.c"
#include "modules/player.c"

/* Collision functions */
int  check_collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
  return (x1 < x2 + w2 &&
          x1 + w1 > x2 &&
          y1 < y2 + h2 &&
          y1 + h1 > y2);
}
static void dummy_collision_fn(JEL_Entity self, JEL_Entity other) { return; }
void tile_collision_fn(JEL_Entity tile, JEL_Entity other)
{

}
static void door_collision_fn(JEL_Entity tile, JEL_Entity other)
{
  struct Position pos;
  JEL_GET(tile, Position, &pos);

  colliding_door = 1;
  
  if (tooltip == 0) {
    tooltip = JEL_entity_create();
    JEL_SET(tooltip, Position, pos.x - 16, pos.y - 32);
    JEL_SET(tooltip, Sprite, 3, 64, 32, 256, 112, 64, 32, 0);
  }

  if (JIN_input.keys.o) {
    if (artifacts_collected > 0)
      JIN_stm_queue("GAME_WIN", 0);
    else
      JIN_stm_queue("GAME_SUS", 0);
  }
}
static void artifact_collision_fn(JEL_Entity item, JEL_Entity other)
{
  ++artifacts_collected;
  JEL_entity_destroy(item);
}
static void guard_collision_fn(JEL_Entity guard, JEL_Entity player)
{
  JIN_stm_queue("GAME_OVER", 0);
}

static int museum_fn_create(struct STM_S *state)
{
  float num_plights = 0;
  /* GL stuff */
  unsigned int *shader = JIN_resm_get("sprite_shader");
  glUseProgram(*shader);
  glUniform1f(glGetUniformLocation(*shader, "lighting"), 1.0f);
  glUniform1f(glGetUniformLocation(*shader, "ambience"), 1.0f);
  /* Map stuff */
  map_x = map_meta[0];
  map_y = map_meta[1];

  artifacts_collected = 0;
  artifacts_total = 0;

  colliding_door = 0;

  tiles = malloc(sizeof(JEL_Entity) * map_x * map_y);
 
  int spawn_x, spawn_y;

  for (int i = 0; i < map_x * map_y; ++i) {
    tiles[i] = JEL_entity_create();
    JEL_SET(tiles[i], Position, (i % map_x) * TILE_SIZE, (i / map_x) * TILE_SIZE);
    JEL_SET(tiles[i], SpriteO, 0, TILE_SIZE, TILE_SIZE, map_tiles[i] * 32, 16, 32, 32, 0);
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
        ++artifacts_total;
        break;
      case 3: /* Guard */
        new_item = JEL_entity_create();
        JEL_SET(new_item, Position, i % map_x * TILE_SIZE, i / map_x * TILE_SIZE);
        JEL_SET(new_item, Sprite, 1, TILE_SIZE, TILE_SIZE, 32, 0, 16, 16, 0);
        JEL_SET(new_item, AABB, TILE_SIZE, TILE_SIZE, guard_collision_fn);
        JEL_SET(new_item, Physics, 0, 5, 0, 0);
        JEL_SET(new_item, Guard, 0, 0, guard_patrol_vertical, dummy_collision_fn, dummy_collision_fn);
        JEL_SET(new_item, Animation, (struct JIN_Animd *) JIN_resm_get("guard_animation"), 0, 0, 0);
        JEL_SET(new_item, PointLight, 2.0, 0.001, 0.001, num_plights++);
        break;
      case 4: /* Guard */
        new_item = JEL_entity_create();
        JEL_SET(new_item, Position, i % map_x * TILE_SIZE, i / map_x * TILE_SIZE);
        JEL_SET(new_item, Sprite, 1, TILE_SIZE, TILE_SIZE, 32, 0, 16, 16, 0);
        JEL_SET(new_item, AABB, TILE_SIZE, TILE_SIZE, guard_collision_fn);
        JEL_SET(new_item, Physics, 5, 0, 0, 0);
        JEL_SET(new_item, Guard, 0, 0, guard_patrol_horizontal, dummy_collision_fn, dummy_collision_fn);
        JEL_SET(new_item, Animation, (struct JIN_Animd *) JIN_resm_get("guard_animation"), 0, 0, 0);
        JEL_SET(new_item, PointLight, 2.0, 0.001, 0.001, num_plights++);
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
  JEL_SET(player, PointLight, 1.0, 0.001, 0.000025, num_plights++);
 
  glUniform1f(glGetUniformLocation(*shader, "num_points"), num_plights);
  set_point_lights(shader);

  return 0;
}

static int museum_fn_destroy(struct STM_S *state)
{
  unsigned int *shader = JIN_resm_get("sprite_shader");
  glUseProgram(*shader);
  glUniform1f(glGetUniformLocation(*shader, "lighting"), 0.0f);
  
  for (int i = 0; i < map_x * map_y; ++i) {
    JEL_entity_destroy(tiles[i]);
  }

  free(tiles);

  JEL_entity_destroy(player);

  if (tooltip) {
    JEL_entity_destroy(tooltip);
    tooltip = 0;
  }

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

  free(map_tiles);
  free(map_items);
  free(map_collisions);

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

static void update_camera(void)
{
  struct Position pos;
  JEL_GET(player, Position, &pos);
  camera.x = pos.x + TILE_SIZE / 2 - WINDOW_WIDTH / 2;
  camera.y = pos.y + TILE_SIZE / 2 - WINDOW_HEIGHT / 2;
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

  update_guard();

  JEL_query_destroy(&q);
 
  colliding_door = 0;
  player_collisions();
  if (!colliding_door && tooltip) {
    JEL_entity_destroy(tooltip);
    tooltip = 0;
  }

  update_camera();
  update_point_lights();

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

