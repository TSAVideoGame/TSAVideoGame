#include "snd/snd.h"
#include "stm/stm.h"
#include "stm/states.h"
#include "globals.h"

int main(int argc, char *args[])
{
  /* Init libraries */
  JIN_snd_init();
 
  /* Init singletons/globals */
  JIN_resm_create(&JIN_resm);
  STM_stack_create(&JIN_states);

  /* Create states */
  struct STM_State test;
  JIN_states_test_create(&test);
  STM_stack_push(&JIN_states, &test);

  /* "Game loop" */
  STM_stack_update(&JIN_states);

  /* Clean singletons/globals */
  STM_stack_destroy(&JIN_states);
  JIN_resm_destroy(&JIN_resm);

  /* Clean libraries */
  JIN_snd_quit();
  
  return 0;
}
