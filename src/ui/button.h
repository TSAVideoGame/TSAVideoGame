#ifndef UI_BUTTON_H
#define UI_BUTTON_H

#include <JEL/jel.h>

/*
 * BUTTON
 *
 * A UI Button
 */
typedef void (*UI_btn_fn)(void);
JEL_COMPONENT_DEFINE(UI_btn, UI_btn_fn, fn, unsigned int *, texture, int, hovered);
JEL_COMPONENT_EXTERN(UI_btn);

#endif
