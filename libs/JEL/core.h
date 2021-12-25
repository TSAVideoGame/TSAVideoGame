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

#define JEL_GLOBAL_VAR
#ifdef _WIN32
  #undef JEL_GLOBAL_VAR
  #ifdef JEL
    #define JEL_GLOBAL_VAR __declspec(dllexport)
  #else
    #define JEL_GLOBAL_VAR __declspec(dllimport)
  #endif
#endif

JEL_GLOBAL_VAR extern struct JEL_Context *JEL_context_current;

int JEL_init(void);
int JEL_quit(void);

/* Entity Table Fragment for all tables */
/* TODO: Where should this go? */

#include "component/component.h"

JEL_COMPONENT_DEFINE(JEL_EntityC, JEL_Entity, entity)
/* JEL_COMPONENT_EXTERN(JEL_EntityC) */
JEL_GLOBAL_VAR extern JEL_TypeIndex JEL_EntityC_id;
JEL_GLOBAL_VAR extern struct JEL_FragmentInfo JEL_EntityC_info;
struct JEL_EntityCFragment;

#endif
