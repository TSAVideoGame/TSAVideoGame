#ifndef JIN_COMPONENTS_H
#define JIN_COMPONENTS_H

/*
 * COMPONENTS
 *
 * Some components that are global or do not really go anywhere else
 * (at least for now)
 */

#include <JEL/jel.h>

struct Position {
  int x;
  int y;
};
JEL_COMPONENT(Position, x, y);
struct Physics {
  float x_vel;
  float y_vel;
  float x_accel;
  float y_accel;
};
JEL_COMPONENT(Physics, x_vel, y_vel, x_accel, y_accel);
struct AABB {
  int x;
  int y;
  int w;
  int h;
};
JEL_COMPONENT(AABB, x, y, w, h);

#endif
