#ifndef JEL_QUERY_H
#define JEL_QUERY_H

#include "table.h"
#include "../component/component_utility.h"
#include "../type.h"

/*
 * Query
 *
 * Query struct is actually the result of a
 * query
 */

struct JEL_Query {
  JEL_ComponentInt   tables_num;
  struct JEL_Table **tables;
};

struct JEL_Query * JEL_query_create(JEL_ComponentInt, ...);
struct JEL_Query * JEL_query_create_type(JEL_Type);
int                JEL_query_destroy(struct JEL_Query *);

#define JEL_QUERY(query, ...) \
{ \
  JEL_Type type = {0, 0, 0, 0}; \
  JEL_TYPE_ADD(type, JEL_EntityC, __VA_ARGS__); \
  query = JEL_query_create_type(type); \
}

#endif
