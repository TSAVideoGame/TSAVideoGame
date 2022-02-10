#ifndef JEL_TYPE_H
#define JEL_TYPE_H

/*
 * Types
 *
 * Types apply to entities and tables. Types
 * are a bit array representing what components
 * are being used. All entities and tables are
 * associated with a type, components are
 * associated with a type index
 *
 * Add more total components by increasing
 * JEL_TYPE_INTS or/and changing the type
 * of the array. Note that there is eventually
 * a limit on the amount of tables
 */

#include <stdint.h>
#include <stddef.h>

#define JEL_TYPE_INTS 4

typedef uint32_t     JEL_Type[JEL_TYPE_INTS];
typedef unsigned int JEL_TypeIndex;

/* i is a bit index */
#define JEL_TYPE_GET(type, i) (type[i / (sizeof(JEL_Type) / JEL_TYPE_INTS * 8)] & (1 << (i % (sizeof(JEL_Type) / JEL_TYPE_INTS * 8))))

/*
 * Type functions
 *
 * init   | Initializes a type to include JEL_EntityC
 * add    | Adds a bit corresponding to the type index
 * remove | Removes a bit corresponding to the type index
 * set    | Sets a type to another type
 */
void JEL_type_init       (JEL_Type);
void JEL_type_add        (JEL_Type, JEL_TypeIndex);
void JEL_type_remove     (JEL_Type, JEL_TypeIndex);
void JEL_type_set        (JEL_Type, JEL_Type);
int  JEL_type_cmp        (JEL_Type, JEL_Type);
int  JEL_type_index_check(JEL_Type, JEL_TypeIndex);
int  JEL_type_num        (JEL_Type);

#endif
