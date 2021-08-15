#ifndef JIN_GLOBALS_H
#define JIN_GLOBALS_H

#include "../resm/resm.h"
#include "../stm/stm.h"
#include "core.h"

/*
 * GLOBALS
 *
 * Really these are more like singletons
 */

extern struct JIN_Resm   JIN_resm;
extern struct STM_Stack  JIN_states;
extern struct JIN_Window JIN_window;

#endif
