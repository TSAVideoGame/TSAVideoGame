#include "globals.h"
#include "../resm/resm.h"
#include "../stm/stm.h"
#include "core.h"
#include "../snd/snd.h"
#include "input.h"

struct JIN_Resm   JIN_resm;
struct STM_Stack  JIN_states;
struct JIN_Window JIN_window;
struct JIN_Sndbgm JIN_sndbgm;
struct JIN_Input  JIN_input = JIN_INPUT_INIT;

int WINDOW_WIDTH;
int WINDOW_HEIGHT;
