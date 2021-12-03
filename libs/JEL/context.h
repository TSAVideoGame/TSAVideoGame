#ifndef JEL_CONTEXT_H
#define JEL_CONTEXT_H

#include "entity.h"
#include "error.h"
#include "table/table.h"
#include "component/component_stack.h"

/* 
 * JEL_Context
 *
 * The ECS library needs to have certain
 * 'global' variables, but these are implemented
 * as contexes. Whenever functions are called,
 * they will be referencing a current context
 *
 * There is no context switching because of
 * how registering components works
 */

struct JEL_Context {
  struct JEL_ErrorStack     *error_stack;
  struct JEL_EntityManager  *entity_manager;
  struct JEL_TableStack     *table_stack;
  struct JEL_ComponentStack *component_stack;
};

#endif
