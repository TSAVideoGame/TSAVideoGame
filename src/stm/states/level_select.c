#include "stm/stm.h"
#include <JEL/jel.h>
#include "core/core.h"
#include "ui/ui.h"
#include "components/components.h"
#include "gfx/sprite.h"
#include "gfx/gfx.h"
#include "resm/resm.h"

#define LVL_SEL_BTNS 11
#define LVL_SEL_L_BTNS 1
#define LVL_SEL_R_COLS 5
struct {
  JEL_Entity cursor;
  JEL_Entity btns[LVL_SEL_BTNS];
  int        group;
  struct {
    int y;
  } l;
  struct {
    int x;
    int y;
  } r;
} lvlsel_vars;

static void to_menu(void)
{
  JIN_stm_queue("MAIN_MENU", 0);
}

static void to_museum()
{
  JIN_stm_queue("MUSEUM", 0);
}

#define LVL_SEL_LIST \
  X( 0,  80, 32, 64, 32, to_menu, 0, 16, 64, 32, 0, 1) \
  X( 1, 256, 32, 64, 32, to_museum, 0, 16, 64, 32, 0, 1) \
  X( 2, 384, 32, 64, 32, to_museum, 0, 16, 64, 32, 0, 1) \
  X( 3, 512, 32, 64, 32, to_museum, 0, 16, 64, 32, 0, 1) \
  X( 4, 640, 32, 64, 32, to_museum, 0, 16, 64, 32, 0, 1) \
  X( 5, 768, 32, 64, 32, to_museum, 0, 16, 64, 32, 0, 1) \
  X( 6, 256, 96, 64, 32, to_museum, 0, 16, 64, 32, 0, 1) \
  X( 7, 384, 96, 64, 32, to_museum, 0, 16, 64, 32, 0, 1) \
  X( 8, 512, 96, 64, 32, to_museum, 0, 16, 64, 32, 0, 1) \
  X( 9, 640, 96, 64, 32, to_museum, 0, 16, 64, 32, 0, 1) \
  X(10, 768, 96, 64, 32, to_museum, 0, 16, 64, 32, 0, 1) \

static int lvlsel_fn_create(struct STM_S *state)
{
  #define X(n, x, y, w, h, fn, tx, ty, tw, th, dir, hov) \
    lvlsel_vars.btns[n] = JEL_entity_create(); \
    JEL_SET(lvlsel_vars.btns[n], UI_btn, fn, (unsigned int *) JIN_resm_get("spritesheet"), hov); \
    JEL_SET(lvlsel_vars.btns[n], Position, x, y); \
    JEL_SET(lvlsel_vars.btns[n], Sprite, 0, w, h, tx, ty, tw, th, dir);
  
  LVL_SEL_LIST
  #undef X

  lvlsel_vars.cursor = JEL_entity_create();
  JEL_SET(lvlsel_vars.cursor, Position, 32, 32);
  JEL_SET(lvlsel_vars.cursor, Sprite, 0, 32, 32, 0, 0, 16, 16, 1);
  lvlsel_vars.l.y = 0;
  lvlsel_vars.r.x = 0;
  lvlsel_vars.r.y = 0;

  return 0;
}

static int lvlsel_fn_destroy(struct STM_S *state)
{
  for (JEL_EntityInt i = 0; i < LVL_SEL_BTNS; ++i) {
    JEL_entity_destroy(lvlsel_vars.btns[i]);
  }
  
  JEL_entity_destroy(lvlsel_vars.cursor);
  return 0;
}

static int lvlsel_fn_update(struct STM_S *state)
{
  if (JIN_input.keys.d == 1) {
    struct Position pos;
    JEL_GET(lvlsel_vars.cursor, Position, &pos);
    if (!lvlsel_vars.group) {
      lvlsel_vars.group = 1;
      pos.x += 176;
      JEL_SET_PROP(lvlsel_vars.cursor, Position, x, pos.x);
    }
    else {
      if (lvlsel_vars.r.x < LVL_SEL_R_COLS - 1) {
        ++lvlsel_vars.r.x;
        pos.x += 128;
        JEL_SET_PROP(lvlsel_vars.cursor, Position, x, pos.x);
      }
    }
  }

  if (JIN_input.keys.a == 1) {
    struct Position pos;
    JEL_GET(lvlsel_vars.cursor, Position, &pos);
    if (lvlsel_vars.group) {
      if (!lvlsel_vars.r.x) {
        lvlsel_vars.group = 0;
        pos.x -= 176;
        JEL_SET_PROP(lvlsel_vars.cursor, Position, x, pos.x);
      }
      else {
        --lvlsel_vars.r.x;
        pos.x -= 128;
        JEL_SET_PROP(lvlsel_vars.cursor, Position, x, pos.x);
      }
    }
  }

  if (JIN_input.keys.s == 1) {
    struct Position pos;
    JEL_GET(lvlsel_vars.cursor, Position, &pos);
    if (!lvlsel_vars.group) {
      if (lvlsel_vars.l.y < LVL_SEL_L_BTNS - 1) {
        ++lvlsel_vars.l.y;
        pos.y += 128;
        JEL_SET_PROP(lvlsel_vars.cursor, Position, y, pos.y);
      }
    }
    else {
      if (lvlsel_vars.r.y < (LVL_SEL_BTNS - LVL_SEL_L_BTNS - 1) / LVL_SEL_R_COLS) {
        ++lvlsel_vars.r.y;
        pos.y += 64;
        JEL_SET_PROP(lvlsel_vars.cursor, Position, y, pos.y);
      }
    }
  }
  
  if (JIN_input.keys.o == 1) {
    struct UI_btn btn;
    if (lvlsel_vars.group) { /* Right side */
      JEL_GET(lvlsel_vars.btns[LVL_SEL_L_BTNS + lvlsel_vars.r.y / LVL_SEL_R_COLS + lvlsel_vars.r.x], UI_btn, &btn);
    }
    else { /* Left side */
      JEL_GET(lvlsel_vars.btns[lvlsel_vars.l.y], UI_btn, &btn);
    }
    btn.fn();
  }

  return 0;
}

static int lvlsel_fn_draw(struct STM_S *state)
{
  JIN_gfx_sprite_draw();
  return 0;
}

int JIN_states_create_lvlsel(struct STM_S *state)
{
  if (STM_s_create(state, 0, lvlsel_fn_create, lvlsel_fn_destroy, lvlsel_fn_update, lvlsel_fn_draw)) return -1;
  return 0;
}

