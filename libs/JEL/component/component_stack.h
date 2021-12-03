#ifndef JEL_COMPONENT_STACK_H
#define JEL_COMPONENT_STACK_H

#include "component_types.h"
#include <stddef.h>
#include "../table/fragment.h"

/*
 * Component Stack
 *
 * Stores run-time information needed for
 * components
 *
 * - The amount of components registered
 * - The size of fragment structs
 *   - Given an id, I need to determine the
 *     size of the corresponding struct
 *     TODO: Specific example of when it's used
 * - A pointer to the fragment info
 *     TODO: Specific example of when it's used
 */

struct JEL_ComponentStack {
  JEL_ComponentInt          components_num;
  JEL_ComponentInt          allocated;

  size_t                   *fragments_sizes;
  struct JEL_FragmentInfo **fragments_infos; /* Could be pointer to const pointer? */
};

/* From component_utility.h */
int JEL_component_stack_push_p(size_t, struct JEL_FragmentInfo *);

#endif
