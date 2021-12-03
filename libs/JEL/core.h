#ifndef JEL_CORE_H
#define JEL_CORE_H

#include "context.h"

/* 
 * Core
 *
 * Only global variable in library for the
 * current context
 *
 * Errors are specific to a context
 *
 * Utility functions for contexts
 */

extern struct JEL_Context *JEL_context_current;

int JEL_init(void);
int JEL_quit(void);

/* Entity Table Fragment for all tables */
/* TODO: Where should this go? */

#include "component/component.h"

JEL_COMPONENT_DEFINE(JEL_EntityC, JEL_Entity, entity)
JEL_COMPONENT_EXTERN(JEL_EntityC)

#endif
