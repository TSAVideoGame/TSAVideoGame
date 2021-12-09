#ifndef JIN_STATES_H
#define JIN_STATES_H

#include "stm.h"

/* Test state */
int JIN_states_create_img      (struct STM_S *);
int JIN_states_create_animation(struct STM_S *);
int JIN_states_create_3d       (struct STM_S *);

/* Game states */
int JIN_states_create_main_menu(struct STM_S *);
int JIN_states_create_museum   (struct STM_S *);

#endif
