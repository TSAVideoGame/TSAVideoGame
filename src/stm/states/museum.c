#include "stm/stm.h"
#include <JEL/jel.h>
#include "resm/resm.h"
#include "gfx/gfx.h"

#define ASCII_0 48

#define TILE_SIZE 32
#include "tile.h"
JEL_COMPONENT_CREATE(Tile, int, x, int, y, int, id);

/*
 * MUSEUM
 *
 * This is a museum level, whether
 * it's an actual level or procedurally
 * generated shouldn't matter
 */
static char *test_map_meta = "7700";
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

static int museum_fn_create(struct STM_S *state)
{
  map_x = test_map_meta[0] - ASCII_0;
  map_y = test_map_meta[1] - ASCII_0;

  tiles = malloc(sizeof(JEL_Entity) * map_x * map_y);

  for (int i = 0; i < map_x * map_y; ++i) {
    tiles[i] = JEL_entity_create();
    JEL_ENTITY_ADD(tiles[i], Tile);
    JEL_ENTITY_SET(tiles[i], Tile, x, (i % map_y) * TILE_SIZE);
    JEL_ENTITY_SET(tiles[i], Tile, y, (i / map_y) * TILE_SIZE);
    JEL_ENTITY_SET(tiles[i], Tile, id, test_map[0] - ASCII_0);

    test_map += 4;
  }
  test_map -= (map_x * map_y - 1) * 4;

  return 0;
}

static int museum_fn_destroy(struct STM_S *state)
{
  for (int i = 0; i < map_x * map_y; ++i) {
    JEL_entity_destroy(tiles[i]);
  }

  free(tiles);

  return 0;
}

static int museum_fn_update(struct STM_S *state)
{
  return 0;
}

#include "core/logger/logger.h"
static int museum_fn_draw(struct STM_S *state)
{
  unsigned int *shader;
  unsigned int *sprite;

  shader = JIN_resm_get("sprite_shader");
  sprite = JIN_resm_get("tiles");

  for (int i = 0; i < map_x * map_y; ++i) {
    int x, y;
    int id;
    JEL_ENTITY_GET(tiles[i], Tile, x, x);
    JEL_ENTITY_GET(tiles[i], Tile, y, y);
    JEL_ENTITY_GET(tiles[i], Tile, id, id);
    JIN_gfx_draw_sprite(shader, sprite, x, y, TILE_SIZE, TILE_SIZE, id * 32, 0, 32, 32);
  }

  return 0;
}

int JIN_states_create_museum(struct STM_S *state)
{
  if (STM_s_create(state, 0, museum_fn_create, museum_fn_destroy, museum_fn_update, museum_fn_draw)) return -1;
  return 0;
}

