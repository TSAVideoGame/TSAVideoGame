#include "stm/states.h"
#include "core/core.h"

#define FPS 30
#define FRAME_DELAY (1000 / FPS)

int main(int argc, char *args[])
{
  JIN_core_init();

  /* Create states */
  struct STM_State test;
  JIN_states_test_create(&test);
  JIN_state_push(&test);

  /* Game loop */
  while (JIN_core_active()) {
    double frame_start = glfwGetTime() / 1000;

    JIN_core_input();
    JIN_core_update();
    JIN_core_draw();

    double frame_time = glfwGetTime() / 1000 - frame_start;

    if (FRAME_DELAY > frame_time) {
      JIN_sleep(FRAME_DELAY - frame_time);
    }
  }

  JIN_core_quit();
  
  return 0;
}
