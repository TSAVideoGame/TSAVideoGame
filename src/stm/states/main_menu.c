#include "stm/stm.h"
#include <JEL/jel.h>
#include "core/core.h"
#include "ui/ui.h"
#include "components/components.h"
#include "gfx/sprite.h"
#include "gfx/gfx.h"
#include "resm/resm.h"
#include <stdio.h>

/*
 * MAIN MENU
 *
 * UI for this is super whack,
 * idk what I'm doing tbh, as long
 * as it works it's all good
 */

static void to_lvlsel(void)
{
  JIN_stm_queue("LVL_SEL", 0);
}
#define MAIN_MENU_BTNS 1
JEL_Entity btns[MAIN_MENU_BTNS];

#define MAIN_MENU_LIST \
  X(0, 448, 320, 64, 32, to_lvlsel, "buttons", 64, 112, 64, 32, 1, 0) \

JEL_Entity cursor;
static int menu_hovered = 0;

struct {
  JEL_Entity title;
  JEL_Entity overlay;
  int map_x;
  int map_y;
  struct { int x; int y; int dx; int dy; } camera;
} main_menu_vars;

static int main_menu_fn_create(struct STM_S *state)
{
  /* Load the map */
  FILE *file;
  if (!(file = fopen("res/maps/title.tgmf", "rb"))) { ERR_EXIT(-1, "Could not open title map file"); }
  #define READ(data, count) if (fread(data, sizeof(*data), count, file) != count) { ERR_EXIT(-1, "Could not read from file"); }
  READ(&main_menu_vars.map_x, 1);
  READ(&main_menu_vars.map_y, 1);
  char temp;
  for (unsigned int i = 0; i < main_menu_vars.map_x * main_menu_vars.map_y; ++i) {
    READ(&temp, 1);
    JEL_Entity e = JEL_entity_create();
    JEL_SET(e, Position, (i % main_menu_vars.map_x) * 32, (i / main_menu_vars.map_x) * 32);
    JEL_SET(e, Sprite, -1, 32, 32, temp * 32, 16, 32, 32, 0);
  }
  #undef READ
  fclose(file);

  /* Main menu stuff */

  main_menu_vars.overlay = JEL_entity_create();
  JEL_SET(main_menu_vars.overlay, Position, 0, 0);
  JEL_SET(main_menu_vars.overlay, Sprite, 0, 960, 640, 383, 0, 1, 1, 0);
  JEL_SET(main_menu_vars.overlay, Fixed, 0, 0);

  /* UI Button stuff */
  #define X(n, xp, yp, wp, hp, fnp, txtp, txtx, txty, txtw, txth, hovp, dir) \
    btns[n] = JEL_entity_create(); \
    JEL_SET(btns[n], UI_btn, fnp, (unsigned int *) JIN_resm_get(txtp), hovp); \
    JEL_SET(btns[n], Position, xp, yp); \
    JEL_SET(btns[n], Sprite, 1, wp, hp, txtx, txty, txtw, txth, dir); \
    JEL_SET(btns[n], Fixed, xp, yp);
    
  MAIN_MENU_LIST
  #undef X

  cursor = JEL_entity_create();
  JEL_SET(cursor, Position, 400, 320);
  JEL_SET(cursor, Sprite, 1, 32, 32, 64, 0, 16, 16, 0);
  JEL_SET(cursor, Fixed, 400, 320);

  main_menu_vars.title = JEL_entity_create();
  JEL_SET(main_menu_vars.title, Position, 352, 32);
  JEL_SET(main_menu_vars.title, Sprite, 1, 256, 256, 0, 144, 128, 128, 0);
  JEL_SET(main_menu_vars.title, Fixed, 352, 32);

  main_menu_vars.camera.x = 0;
  main_menu_vars.camera.y = 0;
  /* Make sure these are negative */
  main_menu_vars.camera.dx = -4;
  main_menu_vars.camera.dy = -8;

  return 0;
}

static int main_menu_fn_destroy(struct STM_S *state)
{
  for (JEL_EntityInt i = 0; i < MAIN_MENU_BTNS; ++i) {
    JEL_entity_destroy(btns[i]);
  }

  struct JEL_Query q;
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

static int main_menu_fn_update(struct STM_S *state)
{
  struct Position cp;
  JEL_GET(cursor, Position, &cp);
  if (JIN_input.keys.w == 1) {
    cp.y -= 48;
    JEL_SET_PROP(cursor, Position, y, cp.y);
    if (--menu_hovered < 0) {
      menu_hovered = MAIN_MENU_BTNS - 1;
      cp.y += 48 * MAIN_MENU_BTNS;
      JEL_SET_PROP(cursor, Position, y, cp.y);
    }
  }
  if (JIN_input.keys.s == 1) {
    cp.y += 48;
    JEL_SET_PROP(cursor, Position, y, cp.y);
    if (++menu_hovered >= MAIN_MENU_BTNS) {
      menu_hovered = 0;
      cp.y -= 48 * MAIN_MENU_BTNS;
      JEL_SET_PROP(cursor, Position, y, cp.y);
    }
  }
  if (JIN_input.keys.o == 1) {
    struct UI_btn btn;
    JEL_GET(btns[menu_hovered], UI_btn, &btn);
    btn.fn();
  }

  /* Camera stuff */
  if (main_menu_vars.camera.x == main_menu_vars.map_x * 32 - 960 || main_menu_vars.camera.x == 0) {
    main_menu_vars.camera.dx *= -1;
  }
  if (main_menu_vars.camera.y == main_menu_vars.map_y * 32 - 640 || main_menu_vars.camera.y == 0) {
    main_menu_vars.camera.dy *= -1;
  }

  main_menu_vars.camera.x += main_menu_vars.camera.dx;
  main_menu_vars.camera.y += main_menu_vars.camera.dy;

  return 0;
}

static int main_menu_fn_draw(struct STM_S *state)
{
  JIN_gfx_sprite_draw(main_menu_vars.camera.x, main_menu_vars.camera.y);

  return 0;
}

int JIN_states_create_main_menu(struct STM_S *state)
{
  if (STM_s_create(state, 0, main_menu_fn_create, main_menu_fn_destroy, main_menu_fn_update, main_menu_fn_draw)) return -1;
  return 0;
}

