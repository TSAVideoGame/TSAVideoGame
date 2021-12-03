#ifndef JEL_FRAGMENT_H
#define JEL_FRAGMENT_H

#include "../component/component_utility.h"
#include "fragment_update_pointers.h"
#include "../entity.h"
#include <stdlib.h>

/*
 * Component (Table) Fragments
 *
 * Fragments of a table that make up a
 * JEL_Table. Fragments (when casted to their
 * specific type) have pointers to each of
 * their members. All fragments have a fragment
 * head, which has generic fragment information.
 *
 * This info and stuff is needed so any table
 * can be allocated properly without knowing
 * it's type.
 *
 * Generic types and casting works by having
 * the first member of every fragment struct
 * being a JEL_FragmentHead_P struct, pretty
 * cool stuff.
 */

struct JEL_FragmentInfo {
  uint8_t const        members_num;
  size_t  const *const members_sizes;
  size_t  const        members_sizes_total;
  void         (*const update_pointers)(void *, JEL_EntityInt);
};

/* Header info for all fragments (private) */
struct JEL_FragmentHead_P {
  struct JEL_FragmentInfo *info; /* Ideally should be const */
  void                    *buffer_start; /* Where fragment's portion of the table buffer starts */
};

/* Generic fragment struct */
struct JEL_Fragment {
  struct JEL_FragmentHead_P head;
};

/*
 * Component Registration
 *
 * - Create the fragment struct
 * - Create the fragment update pointers function
 * - Create the fragment info struct
 */

/* Helper macros */
#define JEL_FRAGMENT_MEMBERS_SET_P(type, name) type *name;

#define JEL_COMPONENT_MEMBERS_SIZES_P(type, name) \
  sizeof(type),

#define JEL_COMPONENT_MEMBERS_SIZES_TOTAL_P(type, name) \
  sizeof(type) +

/* Fragment Creation */
#define JEL_FRAGMENT_CREATE_P(component, ...) \
  void component##_update_pointers_p(void *fragment, JEL_EntityInt allocated) \
  { \
    JEL_FRAGMENT_POINTERS_UPDATE_P(component, __VA_ARGS__) \
  } \
  struct JEL_FragmentInfo component##_info = { \
    .members_num = JEL_COMPONENT_MEMBERS_COUNT_P(__VA_ARGS__), \
    .members_sizes = (size_t []){JEL_COMPONENT_MEMBERS_ITERATE_P(JEL_COMPONENT_MEMBERS_SIZES_P, __VA_ARGS__)}, \
    .members_sizes_total = JEL_COMPONENT_MEMBERS_ITERATE_P(JEL_COMPONENT_MEMBERS_SIZES_TOTAL_P, __VA_ARGS__) 0, \
    .update_pointers = component##_update_pointers_p \
  }; \

#define JEL_FRAGMENT_DEFINE_P(component, ...) \
  struct component##Fragment { \
    struct JEL_FragmentHead_P head; \
    JEL_COMPONENT_MEMBERS_ITERATE_P(JEL_FRAGMENT_MEMBERS_SET_P, __VA_ARGS__) \
  };

#endif
