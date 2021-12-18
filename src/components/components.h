#ifndef JIN_COMPONENTS_H
#define JIN_COMPONENTS_H

/*
 * COMPONENTS
 *
 * Some components that are global or do not really go anywhere else
 * (at least for now)
 */

#include <JEL/jel.h>

JEL_COMPONENT_DEFINE(Position, int, x, int, y);
JEL_COMPONENT_EXTERN(Position);

JEL_COMPONENT_DEFINE(Physics, float, x_vel, float, y_vel, float, x_accel, float, y_accel);
JEL_COMPONENT_EXTERN(Physics);

JEL_COMPONENT_DEFINE(AABB, int, x, int, y, int, w, int, h);
JEL_COMPONENT_EXTERN(AABB);

#endif
