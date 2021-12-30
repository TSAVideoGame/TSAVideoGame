#ifndef JEL_QUERY_H
#define JEL_QUERY_H

#include "type.h"
#include "table.h"

/*
 * Query
 *
 * A struct with a query result
 */

struct JEL_Query {
  unsigned int       count;
  struct JEL_Table **tables;
};

int JEL_query_create (struct JEL_Query *query, JEL_Type type);
int JEL_query_destroy(struct JEL_Query *query);

#define JEL_QUERY(query, ...) \
  { \
    JEL_Type t; \
    JEL_type_init(t); \
    JEL_type_set_str(t, #__VA_ARGS__); \
    JEL_query_create(&q, t); \
  }

#endif
