#ifndef JEL_ENTITY_MAGIC_H
#define JEL_ENTITY_MAGIC_H

/*
 * JEL_SET
 *
 * @desc
 *   Fully sets a component for an entity,
 *   adds the component if it's not added,
 *   creates a table if it's not created
 * @param entity
 *   The entity to set
 * @param component
 *   Name of the component (without struct tag)
 * @param ...
 *   What would go in the brace initialization
 *   of the component struct
 */
#define JEL_SET(entity, component, ...) \
  switch (0) { \
  case 0: { \
    JEL_Type etype; \
    JEL_type_set(etype, JEL_CTX->entity_manager.types[JEL_entity_index(entity)]); \
    JEL_TypeIndex ti = JEL_component_map_get(&JEL_CTX->component_map, #component); \
    if (!ti) break; \
    struct JEL_Table *table = JEL_table_stack_get(&JEL_CTX->table_stack, etype); \
    struct component c = { __VA_ARGS__ }; \
    if (!JEL_type_index_check(etype, ti)) { \
      JEL_type_add(etype, ti); \
      JEL_type_add(JEL_CTX->entity_manager.types[JEL_entity_index(entity)], ti); \
      struct JEL_Table *new_table = JEL_table_stack_get(&JEL_CTX->table_stack, etype); \
      if (!new_table) { \
        new_table = JEL_table_stack_push(&JEL_CTX->table_stack, etype); \
      } \
      JEL_table_add(new_table, entity); \
      for (unsigned int JEL_i = 1; JEL_i < table->types_num; ++JEL_i) { \
        struct JEL_Component *cd = &JEL_CTX->component_table.components[table->types[JEL_i]]; \
        size_t size = cd->offsets[cd->props - 1] + cd->sizes[cd->props - 1]; \
        void *buffer; \
        if (!(buffer = malloc(size))) break; \
        JEL_table_get(table, entity, table->types[JEL_i], buffer); \
        JEL_table_set(new_table, entity, table->types[JEL_i], buffer); \
        free(buffer); \
      } \
      JEL_table_remove(table, entity); \
      table = new_table; \
    } \
    JEL_table_set(table, entity, ti, &c); \
  } \
  }

/*
 * JEL_SET_STRUCT
 *
 * @desc
 *   Fully sets a component for an entity,
 *   adds the component if it's not added,
 *   creates a table if it's not created
 * @param entity
 *   The entity to set
 * @param component
 *   Name of the component (without struct tag)
 * @param struct
 *   Variable whose type is the same as struct
 *   component
 */
#define JEL_SET_STRUCT(entity, component, var) \
  switch (0) { \
  case 0: { \
    JEL_Type etype; \
    JEL_type_set(etype, JEL_CTX->entity_manager.types[JEL_entity_index(entity)]); \
    JEL_TypeIndex ti = JEL_component_map_get(&JEL_CTX->component_map, #component); \
    if (!ti) break; \
    struct JEL_Table *table = JEL_table_stack_get(&JEL_CTX->table_stack, etype); \
    if (!JEL_type_index_check(etype, ti)) { \
      JEL_type_add(etype, ti); \
      JEL_type_add(JEL_CTX->entity_manager.types[JEL_entity_index(entity)], ti); \
      struct JEL_Table *new_table = JEL_table_stack_get(&JEL_CTX->table_stack, etype); \
      if (!new_table) { \
        new_table = JEL_table_stack_push(&JEL_CTX->table_stack, etype); \
      } \
      JEL_table_add(new_table, entity); \
      for (unsigned int JEL_i = 1; JEL_i < table->types_num; ++JEL_i) { \
        struct JEL_Component *cd = &JEL_CTX->component_table.components[table->types[JEL_i]]; \
        size_t size = cd->offsets[cd->props - 1] + cd->sizes[cd->props - 1]; \
        void *buffer; \
        if (!(buffer = malloc(size))) break; \
        JEL_table_get(table, entity, table->types[JEL_i], buffer); \
        JEL_table_set(new_table, entity, table->types[JEL_i], buffer); \
        free(buffer); \
      } \
      JEL_table_remove(table, entity); \
      table = new_table; \
    } \
    JEL_table_set(table, entity, ti, &var); \
  } \
  }

/*
 * JEL_SET_PROP
 *
 * @desc
 *   Sets member (regular set must
 *   have been called first)
 * @param entity
 *   Entity to set
 * @param component
 *   Name of the component (no struct)
 * @param member
 *   Name of the member/property
 * @param data
 *   Varaible or literal data to set
 */
#define JEL_SET_PROP(entity, component, member, data) \
  switch (0) { \
  case 0: { \
    JEL_Type etype; \
    JEL_type_set(etype, JEL_CTX->entity_manager.types[JEL_entity_index(entity)]); \
    JEL_TypeIndex ti = JEL_component_map_get(&JEL_CTX->component_map, #component); \
    if (!ti) break; \
    struct JEL_Table *table = JEL_table_stack_get(&JEL_CTX->table_stack, etype); \
    TYPEOF(((struct component *) 0)->member) var = data; \
    JEL_table_set_member(table, entity, ti, sizeof(var), offsetof(struct component, member), &var); \
  } \
  }

/*
 * JEL_GET
 *
 * @desc
 *   Gets all data from a component
 *   (associated with an entity ofc)
 * @param entity
 *   The entity
 * @param component
 *   Name of the component (no struct)
 * @param var
 *   Where to put the data (void * buffer
 *   or address of a struct, most of the
 *   times you will use address of struct)
 */
#define JEL_GET(entity, component, var) \
  switch (0) { \
  case 0: { \
    JEL_Type etype; \
    JEL_type_set(etype, JEL_CTX->entity_manager.types[JEL_entity_index(entity)]); \
    JEL_TypeIndex ti = JEL_component_map_get(&JEL_CTX->component_map, #component); \
    if (!ti) break; \
    struct JEL_Table *table = JEL_table_stack_get(&JEL_CTX->table_stack, etype); \
    JEL_table_get(table, entity, ti, var); \
  } \
  }

#define JEL_CHECK(entity, component) JEL_type_index_check(JEL_CTX->entity_manager.types[JEL_entity_index(entity)], JEL_component_map_get(&JEL_CTX->component_map, #component))

#endif
