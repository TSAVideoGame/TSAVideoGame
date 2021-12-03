#ifndef JEL_COMPONENT_H
#define JEL_COMPONENT_H

#include "component_types.h"
#include "component_struct.h"
#include "../table/table.h"

/* 
 * Components
 *
 * Components is a general term referring to data,
 * it could be the struct that represents the data,
 * the table where the data is stored, or the data
 * itself in the table. Since it's somewhat
 * ambiguous what a component is, it is somewhat
 * fitting how it uses macro magic.
 *
 * Since I need the size of the members and there
 * can be a variable amount of members, macros are
 * used to create the structs
 *
 * Create components in .c files
 * Define components in .h files
 * Extern if you want to access the info and id variables
 *
 * Use the exact same arguments in the macros
 */

#define JEL_COMPONENT_MAX (~((JEL_ComponentInt) 0))

/* Component Creation */
#define JEL_COMPONENT_CREATE(component, ...) \
  JEL_FRAGMENT_CREATE_P(component, __VA_ARGS__) \
  JEL_TypeIndex component##_id = 0;

/* Component Definition */
#define JEL_COMPONENT_DEFINE(component, ...) \
  JEL_COMPONENT_STRUCT_DEFINE_P(component, __VA_ARGS__) \
  JEL_FRAGMENT_DEFINE_P(component, __VA_ARGS__) \

/* Component Registration */
#define JEL_COMPONENT_REGISTER(component) \
  component##_id = JEL_context_current->component_stack->components_num; \
  JEL_component_stack_push_p(sizeof(struct component##Fragment), &component##_info);

/* Component Extern */
#define JEL_COMPONENT_EXTERN(component) \
  extern JEL_TypeIndex component##_id; \
  extern struct JEL_FragmentInfo component##_info; \
  struct component##Fragment;

#endif
