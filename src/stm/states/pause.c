#include "stm/stm.h"
#include <JEL/jel.h>
#include "core/core.h"
#include "ui/ui.h"
#include "components/components.h"
#include "gfx/sprite.h"
#include "gfx/gfx.h"
#include "resm/resm.h"
#include "core/gll/gll.h"

#define PAUSE_BTNS 2
struct {
  JEL_Entity cursor;
  JEL_Entity screen; /* (TODO) Transparent black square */
  JEL_Entity btns[PAUSE_BTNS];
  int menu_hovered;
} pause_vars;

static void to_back_museum()
{
  JIN_stm_queue("MUSEUM", 0);
}

static void to_back_lvlsel()
{
  JIN_stm_queue("LVL_SEL", 0);
}

#define PAUSE_LIST \
  X(0, 448, 320, 64, 32, to_back_museum, 128, 112, 64, 32, 0, 1) \
  X(1, 448, 368, 64, 32, to_back_lvlsel, 192, 112, 64, 32, 0, 0) \

static int pause_fn_create(struct STM_S *state)
{
  unsigned int *shader = JIN_resm_get("sprite_shader");
  glUseProgram(*shader);
  glUniform1f(glGetUniformLocation(*shader, "lighting"), 0.0f);
  
#define X(n, x, y, w, h, fn, tx, ty, tw, th, dir, hov) \
    pause_vars.btns[n] = JEL_entity_create(); \
    JEL_SET(pause_vars.btns[n], UI_btn, fn, (unsigned int *) JIN_resm_get("spritesheet"), hov); \
    JEL_SET(pause_vars.btns[n], Position, x, y); \
    JEL_SET(pause_vars.btns[n], Sprite, 1001, w, h, tx, ty, tw, th, dir);
  
  PAUSE_LIST
  #undef X

  pause_vars.screen = JEL_entity_create();
  JEL_SET(pause_vars.screen, Position, 0, 0);
  JEL_SET(pause_vars.screen, Sprite, 1000, 960, 640, 7, 6, 1, 1, 0);
 
  pause_vars.cursor = JEL_entity_create();
  JEL_SET(pause_vars.cursor, Position, 400, 320);
  JEL_SET(pause_vars.cursor, Sprite, 1001, 32, 32, 64, 0, 16, 16, 0);
  
  pause_vars.menu_hovered = 0;

  return 0;
}

static int pause_fn_destroy(struct STM_S *state)
{
  unsigned int *shader = JIN_resm_get("sprite_shader");
  glUseProgram(*shader);
  glUniform1f(glGetUniformLocation(*shader, "ambience"), 0.2f);
  
  for (JEL_EntityInt i = 0; i < PAUSE_BTNS; ++i) {
    JEL_entity_destroy(pause_vars.btns[i]);
  }
  
  JEL_entity_destroy(pause_vars.cursor);
  JEL_entity_destroy(pause_vars.screen);
  
  return 0;
}

static int pause_fn_update(struct STM_S *state)
{
  struct Position cp;
  JEL_GET(pause_vars.cursor, Position, &cp);
  if (JIN_input.keys.w == 1) {
    cp.y -= 48;
    JEL_SET_PROP(pause_vars.cursor, Position, y, cp.y);
    if (--pause_vars.menu_hovered < 0) {
      pause_vars.menu_hovered = PAUSE_BTNS - 1;
      cp.y += 48 * PAUSE_BTNS;
      JEL_SET_PROP(pause_vars.cursor, Position, y, cp.y);
    }
  }
  if (JIN_input.keys.s == 1) {
    cp.y += 48;
    JEL_SET_PROP(pause_vars.cursor, Position, y, cp.y);
    if (++pause_vars.menu_hovered >= PAUSE_BTNS) {
      pause_vars.menu_hovered = 0;
      cp.y -= 48 * PAUSE_BTNS;
      JEL_SET_PROP(pause_vars.cursor, Position, y, cp.y);
    }
  }
  if (JIN_input.keys.o == 1) {
    struct UI_btn btn;
    JEL_GET(pause_vars.btns[pause_vars.menu_hovered], UI_btn, &btn);
    btn.fn();
  }

  return 0;
}

static int pause_fn_draw(struct STM_S *state)
{
  JIN_gfx_sprite_draw(0, 0);
  
  return 0;
}

int JIN_states_create_pause(struct STM_S *state)
{
  if (STM_s_create(state, 0, pause_fn_create, pause_fn_destroy, pause_fn_update, pause_fn_draw)) return -1;
  return 0;
}

