#ifndef JIN_STATES_H
#define JIN_STATES_H

#include "stm.h"

/* Game states */
int JIN_states_create_game_over(struct STM_S *);
int JIN_states_create_game_win (struct STM_S *);
int JIN_states_create_main_menu(struct STM_S *);
int JIN_states_create_lvlsel   (struct STM_S *);
int JIN_states_create_museum   (struct STM_S *);
int JIN_states_create_pause    (struct STM_S *);

#endif
