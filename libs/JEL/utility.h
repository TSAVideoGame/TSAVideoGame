#ifndef JEL_UTILITY_H
#define JEL_UTILITY_H

/*
 * Utility
 *
 * Useful macros or functions
 * that don't belong anywhere
 * since they are intertwined
 */

#include "type.h"

#ifdef _WIN32
  #define TYPEOF(x) __typeof__(x)
#else
  #define TYPEOF(x) typeof(x)
#endif

int JEL_type_set_str(JEL_Type t, const char *str);

#endif
