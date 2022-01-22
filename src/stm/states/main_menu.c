#include "stm/stm.h"
#include <JEL/jel.h>
#include "core/core.h"
#include "ui/ui.h"
#include "components/components.h"
#include "gfx/sprite.h"
#include "gfx/gfx.h"
#include "resm/resm.h"

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
} main_menu_vars;

static int main_menu_fn_create(struct STM_S *state)
{
  #define X(n, xp, yp, wp, hp, fnp, txtp, txtx, txty, txtw, txth, hovp, dir) \
    btns[n] = JEL_entity_create(); \
    JEL_SET(btns[n], UI_btn, fnp, (unsigned int *) JIN_resm_get(txtp), hovp); \
    JEL_SET(btns[n], Position, xp, yp); \
    JEL_SET(btns[n], Sprite, 0, wp, hp, txtx, txty, txtw, txth, dir); \
    
  MAIN_MENU_LIST
  #undef X

  cursor = JEL_entity_create();
  JEL_SET(cursor, Position, 400, 320);
  JEL_SET(cursor, Sprite, 0, 32, 32, 64, 0, 16, 16, 0);

  main_menu_vars.title = JEL_entity_create();
  JEL_SET(main_menu_vars.title, Position, 352, 32);
  JEL_SET(main_menu_vars.title, Sprite, 0, 256, 256, 0, 144, 128, 128, 0);

  return 0;
}

static int main_menu_fn_destroy(struct STM_S *state)
{
  for (JEL_EntityInt i = 0; i < MAIN_MENU_BTNS; ++i) {
    JEL_entity_destroy(btns[i]);
  }

  JEL_entity_destroy(cursor);
  JEL_entity_destroy(main_menu_vars.title);
  
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

  return 0;
}

static int main_menu_fn_draw(struct STM_S *state)
{
  JIN_gfx_sprite_draw(0, 0);

  return 0;
}

int JIN_states_create_main_menu(struct STM_S *state)
{
  if (STM_s_create(state, 0, main_menu_fn_create, main_menu_fn_destroy, main_menu_fn_update, main_menu_fn_draw)) return -1;
  return 0;
}

