#ifndef JEL_CORE_H
#define JEL_CORE_H

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

#include "entity.h"
#include "component.h"
#include "table.h"

struct JEL_Context {
  struct JEL_EntityManager *entity_manager;
  struct JEL_ComponentTable component_table;
  struct JEL_ComponentMap   component_map;
  struct JEL_TableStack     table_stack;
};

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
#define JEL_CTX JEL_context_current

/* Entity Component */
struct JEL_EntityC { JEL_Entity entity; };
struct JEL_EntityCIt { JEL_Entity *entity; };

/*
 * Core functions
 *
 * init | Initializes JEL with a current context
 * quit | Quits JEL (closes the current context)
 */
int  JEL_init(void);
void JEL_quit(void);

#endif
