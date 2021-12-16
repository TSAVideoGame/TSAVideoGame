#include "components.h"

JEL_COMPONENT_CREATE(Position, int, x, int, y);

JEL_COMPONENT_CREATE(Sprite, unsigned, texture, int, w, int, h, int, tex_x, int, tex_y, int, tex_w, int, tex_h);

JEL_COMPONENT_CREATE(Physics, float, x_vel, float, y_vel, float, x_accel, float, y_accel);

JEL_COMPONENT_CREATE(AABB, int, x, int, y, int, w, int, h);

