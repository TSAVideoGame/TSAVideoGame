#include "stm/stm.h"
#include <JEL/jel.h>
#include "core/core.h"
#include "ui/ui.h"
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
  JIN_inputv.quit = 1;
}

#define MAIN_MENU_BTNS 2
JEL_Entity btns[MAIN_MENU_BTNS];
JEL_Entity btn_ptr;

#define MAIN_MENU_LIST \
  X(0, 48, 0, 64, 32, to_museum, "buttons", 0, 0, 64, 32, 1) \
  X(1, 48,48, 64, 32, to_quit, "buttons", 64, 0, 64, 32, 0)

static int menu_hovered = 0;
static int cursor_pos_x = 0;
static int cursor_pos_y = 0;

static int main_menu_fn_create(struct STM_S *state)
{
  #define X(n, xp, yp, wp, hp, fnp, txtp, txtx, txty, txtw, txth, hovp) \
    btns[n] = JEL_entity_create(); \
    JEL_ENTITY_ADD(btns[n], UI_btn); \
    JEL_ENTITY_SET(btns[n], UI_btn, x, xp); \
    JEL_ENTITY_SET(btns[n], UI_btn, y, yp); \
    JEL_ENTITY_SET(btns[n], UI_btn, w, wp); \
    JEL_ENTITY_SET(btns[n], UI_btn, h, hp); \
    JEL_ENTITY_SET(btns[n], UI_btn, fn, fnp); \
    JEL_ENTITY_SET(btns[n], UI_btn, texture, (unsigned int *) JIN_resm_get(txtp)); \
    JEL_ENTITY_SET(btns[n], UI_btn, txt_x, txtx); \
    JEL_ENTITY_SET(btns[n], UI_btn, txt_y, txty); \
    JEL_ENTITY_SET(btns[n], UI_btn, txt_w, txtw); \
    JEL_ENTITY_SET(btns[n], UI_btn, txt_h, txth); \
    JEL_ENTITY_SET(btns[n], UI_btn, hovered, hovp);
  MAIN_MENU_LIST
  #undef X

  return 0;
}

static int main_menu_fn_destroy(struct STM_S *state)
{
  for (JEL_EntityInt i = 0; i < MAIN_MENU_BTNS; ++i) {
    JEL_entity_destroy(btns[i]);
  }
  
  return 0;
}

static int main_menu_fn_update(struct STM_S *state)
{
  if (JIN_input.keys.w == 1) {
    cursor_pos_y -= 48;
    if (--menu_hovered < 0) {
      menu_hovered = MAIN_MENU_BTNS - 1;
      cursor_pos_y += 48 * MAIN_MENU_BTNS;
    }
  }
  if (JIN_input.keys.s == 1) {
    cursor_pos_y += 48;
    if (++menu_hovered >= MAIN_MENU_BTNS) {
      menu_hovered = 0;
      cursor_pos_y -= 48 * MAIN_MENU_BTNS;
    }
  }
  if (JIN_input.keys.d == 1) {
    UI_btn_fn fn;
    JEL_ENTITY_GET(btns[menu_hovered], UI_btn, fn, fn);
    fn();
  }

  return 0;
}

static int main_menu_fn_draw(struct STM_S *state)
{
  JIN_gfx_draw_sprite((unsigned int *) JIN_resm_get("sprite_shader"), (unsigned int *) JIN_resm_get("player_img"), cursor_pos_x, cursor_pos_y, 32, 32, 0, 0, 16, 16); 

  struct JEL_Query *q;
  JEL_QUERY(q, UI_btn);

  unsigned int *shader;

  shader = JIN_resm_get("sprite_shader");
  
  for (JEL_ComponentInt i = 0; i < q->tables_num; ++i) {
    struct UI_btnFragment *btn;
    JEL_FRAGMENT_GET(btn, q->tables[i], UI_btn);

    for (JEL_EntityInt j = 0; j < q->tables[i]->num; ++j) {
      JIN_gfx_draw_sprite(shader, btn->texture[j],
          btn->x[j], btn->y[j], btn->w[j], btn->h[j],
          btn->txt_x[j], btn->txt_y[j], btn->txt_w[j], btn->txt_h[j]); 
    }
  }

  JEL_query_destroy(q);

  return 0;
}

int JIN_states_create_main_menu(struct STM_S *state)
{
  if (STM_s_create(state, 0, main_menu_fn_create, main_menu_fn_destroy, main_menu_fn_update, main_menu_fn_draw)) return -1;
  return 0;
}

