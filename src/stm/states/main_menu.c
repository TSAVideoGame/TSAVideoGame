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
  JIN_stm_queue("IMG", 0);
}

static void to_quit(void)
{
  JIN_input.quit = 1;
}

#define MAIN_MENU_BTNS 2
JEL_Entity btns[2];
JEL_Entity btn_ptr;

#define MAIN_MENU_LIST \
  X(0, 48, 0, 64, 32, to_museum, 0, 1) \
  X(1, 48,48, 64, 32, to_quit, 0, 0)

static int menu_hovered = 0;
static int cursor_pos_x = 0;
static int cursor_pos_y = 0;

static int main_menu_fn_create(struct STM_S *state)
{
  #define X(n, xp, yp, wp, hp, fnp, txtp, hovp) \
    btns[n] = JEL_entity_create(); \
    JEL_ENTITY_ADD(btns[n], UI_btn); \
    JEL_ENTITY_SET(btns[n], UI_btn, x, xp); \
    JEL_ENTITY_SET(btns[n], UI_btn, y, yp); \
    JEL_ENTITY_SET(btns[n], UI_btn, w, wp); \
    JEL_ENTITY_SET(btns[n], UI_btn, h, hp); \
    JEL_ENTITY_SET(btns[n], UI_btn, fn, fnp); \
    JEL_ENTITY_SET(btns[n], UI_btn, texture, txtp); \
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
      cursor_pos_y = 0;
    }
  }
  if (JIN_input.keys.s == 1) {
    cursor_pos_y += 48;
    if (++menu_hovered >= MAIN_MENU_BTNS) {
      menu_hovered = 0;
      cursor_pos_y = 0;
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
  JIN_gfx_draw_sprite((unsigned int *) JIN_resm_get("sprite_shader"), (unsigned int *) JIN_resm_get("player_img"), cursor_pos_x, cursor_pos_y, 32, 32, 0, 0, 32, 32); 

  return 0;
}

#include "core/gll/gll.h"
#include "cglm/cglm.h"
int JIN_states_create_main_menu(struct STM_S *state)
{
  unsigned int *shader = JIN_resm_get("sprite_shader");

  glUseProgram(*shader);

  mat4 projection;
  glm_ortho(0.0f, (float) WINDOW_WIDTH, (float) WINDOW_HEIGHT, 0.0f, -1.0f, 1.0f, projection);
  glUniformMatrix4fv(glGetUniformLocation(*shader, "projection"), 1, GL_FALSE, (float *) projection);
  
  if (STM_s_create(state, 0, main_menu_fn_create, main_menu_fn_destroy, main_menu_fn_update, main_menu_fn_draw)) return -1;
  return 0;
}

