#ifndef JEL_COMPONENT_H
#define JEL_COMPONENT_H

/*
 * Component
 *
 * Defines some data, get attached
 * to entities
 *
 * Components are basically structs
 * Size is the size of each member
 * Offset is the offset of each member
 *
 * The component table is an array
 * of all components. There is a 0th
 * index component which is a NULL
 * component
 *
 * The component map is a hashmap
 * of component names to indices in
 * the component table
 */

#include <stdlib.h>
#include "type.h"
#include "component_magic.h"
#include "utility.h"

#define JEL_MAX_PROPS 16
#define JEL_MAX_COMPONENTS 128

struct JEL_Component {
  unsigned int props;
  size_t       sizes[JEL_MAX_PROPS];
  size_t       offsets[JEL_MAX_PROPS];
};

void   JEL_component_create(struct JEL_Component *component, unsigned int props, size_t *sizes, size_t *offsets);
size_t JEL_component_size(struct JEL_Component *component);

#define JEL_COMPONENT_IT_FN(name, m) typeof(((struct name *) 0)->m) *m;

#define JEL_COMPONENT_IT(name, ...) \
  struct name##It { \
    JEL_COMPONENT_ITERATE(JEL_COMPONENT_IT_FN, name, __VA_ARGS__) \
  };

#ifdef JEL_REGISTER_COMPONENTS
  #define JEL_COMPONENT_MEMBER_SIZE(component, m) sizeof(((struct component *) 0)->m),
  #define JEL_COMPONENT_MEMBER_OFFSET(component, m) offsetof(struct component, m),
  #define JEL_COMPONENT(name, ...) \
    static unsigned int name##_props = ARG_COUNT_16(__VA_ARGS__); \
    static size_t name##_sizes[ARG_COUNT_16(__VA_ARGS__)] =   { JEL_COMPONENT_ITERATE(JEL_COMPONENT_MEMBER_SIZE,   name, __VA_ARGS__) }; \
    static size_t name##_offsets[ARG_COUNT_16(__VA_ARGS__)] = { JEL_COMPONENT_ITERATE(JEL_COMPONENT_MEMBER_OFFSET, name, __VA_ARGS__) }; \
    JEL_COMPONENT_IT(name, __VA_ARGS__)

  #define JEL_REGISTER(component) \
    { \
      struct JEL_Component c; \
      JEL_component_create(&c, component##_props, component##_sizes, component##_offsets); \
      JEL_component_table_add(&JEL_context_current->component_table, &c); \
      JEL_component_map_add(&JEL_context_current->component_map, #component, JEL_context_current->component_table.count - 1); \
    }
#else
  #define JEL_COMPONENT(name, ...) \
    JEL_COMPONENT_IT(name, __VA_ARGS__)
#endif

struct JEL_ComponentTable {
  unsigned int          count;
  unsigned int          allocated;
  struct JEL_Component *components;
};

/*
 * Table functions
 */
int  JEL_component_table_create(struct JEL_ComponentTable *table);
void JEL_component_table_destroy(struct JEL_ComponentTable *table);
int  JEL_component_table_add(struct JEL_ComponentTable *table, struct JEL_Component *component);
int  JEL_component_table_allocate(struct JEL_ComponentTable *table, unsigned int count);

/* Map uses linked list collision handling */
struct JEL_ComponentMapItem {
  char                        *key;   /* Component name */
  JEL_TypeIndex                value; /* Component index */
  struct JEL_ComponentMapItem *next;  /* For collisions */
};

struct JEL_ComponentMap {
  struct JEL_ComponentMapItem components[JEL_MAX_COMPONENTS]; 
};

/*
 * Component functions
 */
void          JEL_component_map_create (struct JEL_ComponentMap *map);
void          JEL_component_map_destroy(struct JEL_ComponentMap *map);
int           JEL_component_map_add    (struct JEL_ComponentMap *map, const char *key, JEL_TypeIndex value);
JEL_TypeIndex JEL_component_map_get    (struct JEL_ComponentMap *map, const char *key);

#endif
