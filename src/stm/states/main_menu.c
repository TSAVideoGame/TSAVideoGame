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

static void to_museum(void)
{
  JIN_stm_queue("MUSEUM", 0);
}

static void to_quit(void)
{
  LOG(LOG, "Hi!");
}

#define MAIN_MENU_BTNS 2
JEL_Entity btns[MAIN_MENU_BTNS];
JEL_Entity btn_ptr;

#define MAIN_MENU_LIST \
  X(0, 48, 0, 64, 32, to_museum, "buttons", 0, 16, 64, 32, 1, 0) \
  X(1, 48,48, 64, 32, to_quit, "buttons", 64, 16, 64, 32, 0, 0)

JEL_Entity cursor;
static int menu_hovered = 0;

static int main_menu_fn_create(struct STM_S *state)
{
  #define X(n, xp, yp, wp, hp, fnp, txtp, txtx, txty, txtw, txth, hovp, dir) \
    btns[n] = JEL_entity_create(); \
    JEL_ENTITY_SET(btns[n], UI_btn, fnp, (unsigned int *) JIN_resm_get(txtp), hovp); \
    JEL_ENTITY_SET(btns[n], Position, xp, yp); \
    JEL_ENTITY_SET(btns[n], Sprite, 0, wp, hp, txtx, txty, txtw, txth, dir); \
    
  MAIN_MENU_LIST
  #undef X

  cursor = JEL_entity_create();
  JEL_ENTITY_SET(cursor, Position, 0, 0);
  JEL_ENTITY_SET(cursor, Sprite, 0, 32, 32, 0, 0, 16, 16, 1);

  return 0;
}

static int main_menu_fn_destroy(struct STM_S *state)
{
  for (JEL_EntityInt i = 0; i < MAIN_MENU_BTNS; ++i) {
    JEL_entity_destroy(btns[i]);
  }

  JEL_entity_destroy(cursor);
  
  return 0;
}

static int main_menu_fn_update(struct STM_S *state)
{
  struct Position cp;
  JEL_ENTITY_GET(cursor, Position, &cp);
  if (JIN_input.keys.w == 1) {
    cp.y -= 48;
    JEL_ENTITY_SET_PROP(cursor, Position, y, cp.y);
    if (--menu_hovered < 0) {
      menu_hovered = MAIN_MENU_BTNS - 1;
      cp.y += 48 * MAIN_MENU_BTNS;
      JEL_ENTITY_SET_PROP(cursor, Position, y, cp.y);
    }
  }
  if (JIN_input.keys.s == 1) {
    cp.y += 48;
    JEL_ENTITY_SET_PROP(cursor, Position, y, cp.y);
    if (++menu_hovered >= MAIN_MENU_BTNS) {
      menu_hovered = 0;
      cp.y -= 48 * MAIN_MENU_BTNS;
      JEL_ENTITY_SET_PROP(cursor, Position, y, cp.y);
    }
  }
  if (JIN_input.keys.d == 1) {
    struct UI_btn btn;
    JEL_ENTITY_GET(btns[menu_hovered], UI_btn, &btn);
    btn.fn();
  }

  return 0;
}

static int main_menu_fn_draw(struct STM_S *state)
{
  JIN_gfx_sprite_draw();

  return 0;
}

int JIN_states_create_main_menu(struct STM_S *state)
{
  if (STM_s_create(state, 0, main_menu_fn_create, main_menu_fn_destroy, main_menu_fn_update, main_menu_fn_draw)) return -1;
  return 0;
}

