#ifndef JEL_TYPE_H
#define JEL_TYPE_H

/*
 * Types
 *
 * Types apply to entities and tables. Types
 * are a bit array representing what components
 * are being used
 */

#include <stdint.h>
#include <stddef.h>

#define JEL_TYPE_INTS 4
typedef uint32_t JEL_Type[JEL_TYPE_INTS];

typedef size_t JEL_TypeIndex;

/*
 * Type functions
 *
 * init      | Initializes a type to include JEL_EntityC
 * index_add | Adds a bit corresponding to the type index
 * set       | Sets a type to another type
 */
int JEL_type_init(JEL_Type);
int JEL_type_index_add(JEL_Type, JEL_TypeIndex);
int JEL_type_set(JEL_Type, JEL_Type);

#define JEL_TYPE_ADD_HELPER_HELPER_P(component) JEL_type[component##_id / 32] |= 1 << (component##_id % 32);

#define JEL_TYPE_ADD_HELPER_P(id, ...) \
  JEL_COMPONENTS_ITERATE_P(JEL_TYPE_ADD_HELPER_HELPER_P, __VA_ARGS__)

/*
 * JEL_TYPE_ADD
 *
 * Adds type bits to a type given component names
 */
#define JEL_TYPE_ADD(type, ...) \
{ \
  JEL_Type JEL_type; \
  JEL_type_set(JEL_type, type); \
  JEL_TYPE_ADD_HELPER_P(JEL_type, __VA_ARGS__); \
  JEL_type_set(type, JEL_type); \
}

#endif
