#ifndef JEL_COMPONENT_STRUCT_H
#define JEL_COMPONENT_STRUCT_H

#include "component_utility.h"

#define JEL_COMPONENT_STRUCT_MEMBERS_SET_P(type, name) \
  type name;

#define JEL_COMPONENT_STRUCT_DEFINE_P(component, ...) \
  struct component { \
    JEL_COMPONENT_MEMBERS_ITERATE_P(JEL_COMPONENT_STRUCT_MEMBERS_SET_P, __VA_ARGS__) \
  };

#endif
