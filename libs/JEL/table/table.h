#ifndef JEL_TABLE_H
#define JEL_TABLE_H

#include "../entity.h"
#include "../component/component_types.h"
#include "fragment.h"
#include "../component/component_utility.h"

/*
 * Tables
 *
 * Entities are primary keys, components are
 * data, and the data is further organized
 * into table fragments
 */

/* Generic table struct */
/* TODO: Some of this stuff could be const */
struct JEL_Table {
  JEL_EntityInt         allocated;
  JEL_EntityInt         num; /* Num is like num entries */
  void                 *buffer;
  JEL_ComponentInt      fragments_num;
  JEL_TypeIndex        *fragments_types;
  struct JEL_Fragment **fragments;
};

/* Table stack, holds pointers to all the tables */
struct JEL_TableStack {
  JEL_ComponentInt   tables_allocated;
  JEL_ComponentInt   tables_num;
  struct JEL_Table **tables;
  JEL_Type          *tables_types;
};

/* Utility */
struct JEL_Table * JEL_table_get(JEL_Type);

/* These functions are all from table_utility.h */
struct JEL_Table *      JEL_table_create_p(JEL_ComponentInt, ...);
struct JEL_Table *      JEL_table_create_type_p(JEL_Type);
int                     JEL_table_destroy_p(struct JEL_Table *);

int                     JEL_table_add_p(struct JEL_Table *, ...);
int                     JEL_table_add_buffer_p(struct JEL_Table *, void *);
/* TODO: Remove entity and remove index, so there is a faster version if there's more info */
int                     JEL_table_remove_p(struct JEL_Table *, JEL_Entity);

JEL_EntityInt           JEL_table_index_get_p(struct JEL_Table *, JEL_Entity);

int                     JEL_table_row_move_p(struct JEL_Table *, JEL_Entity, struct JEL_Table *);

int                     JEL_table_allocate_p(struct JEL_Table *, JEL_EntityInt);

struct JEL_TableStack * JEL_table_stack_create_p(void);
int                     JEL_table_stack_destroy_p(struct JEL_TableStack *);

int                     JEL_table_stack_push_p(struct JEL_Table *);

int                     JEL_table_stack_allocate_p(struct JEL_TableStack *, JEL_ComponentInt);

/* Back to non table_utility.h */
/* TODO: This could go in type */
#define JEL_TABLE_GET(table, ...) \
{ \
  JEL_Type type = {0, 0, 0, 0}; \
  JEL_TYPE_ADD(type, __VA_ARGS__); \
  table = JEL_table_get(type); \
} \

#define JEL_FRAGMENT_GET(fragment, table, component) \
{ \
  for (int JEL_i = 0; JEL_i < table->fragments_num; ++JEL_i) { \
    if (table->fragments_types[JEL_i] == component##_id) { \
      fragment = (struct component##Fragment *) table->fragments[JEL_i]; \
      break; \
    } \
  } \
}

/*
 * Entity Utility functions
 *
 * add    | Attaches a component to an entity and changes the entity's type to match
 * set    | Sets a value
 * get    | Gets a value
 * change | Changes a value with an expression ex. JEL_ENTITY_CHANGE(entity, Position, x, *= 2)
 */
#define JEL_ENTITY_ADD(entity, ...) \
{ \
  struct JEL_Table *old_table = JEL_table_get(JEL_context_current->entity_manager->types[JEL_entity_index_get(entity)]); \
  JEL_TYPE_ADD(JEL_context_current->entity_manager->types[JEL_entity_index_get(entity)], __VA_ARGS__); \
  struct JEL_Table *new_table = JEL_table_get(JEL_context_current->entity_manager->types[JEL_entity_index_get(entity)]); \
  if (new_table == NULL) { \
    new_table = JEL_table_create_type_p(JEL_context_current->entity_manager->types[JEL_entity_index_get(entity)]); \
    JEL_table_stack_push_p(new_table); \
  } \
  JEL_table_row_move_p(old_table, entity, new_table); \
}

#define JEL_ENTITY_SET(entity, component, member, value) \
{ \
  struct JEL_Table *table = JEL_table_get(JEL_context_current->entity_manager->types[JEL_entity_index_get(entity)]); \
  struct component##Fragment *fragment; \
  JEL_FRAGMENT_GET(fragment, table, component); \
  fragment->member[JEL_table_index_get_p(table, entity)] = value; \
}

#define JEL_ENTITY_GET(entity, component, member, out) \
{ \
  struct JEL_Table *table = JEL_table_get(JEL_context_current->entity_manager->types[JEL_entity_index_get(entity)]); \
  struct component##Fragment *fragment; \
  JEL_FRAGMENT_GET(fragment, table, component); \
  out = fragment->member[JEL_table_index_get_p(table, entity)]; \
}

#define JEL_ENTITY_CHANGE(entity, component, member, expr) \
{ \
  struct JEL_Table *table = JEL_table_get(JEL_context_current->entity_manager->types[JEL_entity_index_get(entity)]); \
  struct component##Fragment *fragment; \
  JEL_FRAGMENT_GET(fragment, table, component); \
  fragment->member[JEL_table_index_get_p(table, entity)] expr; \
}

#endif
