#ifndef JEL_ENTITY_MAGIC_H
#define JEL_ENTITY_MAGIC_H

/*
 * JEL_ENTITY_SET
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
#define JEL_ENTITY_SET(entity, component, ...) \
  switch (0) { \
  case 0: { \
    JEL_Type etype; \
    JEL_type_set(etype, JEL_CTX->entity_manager->types[JEL_entity_index(entity)]); \
    JEL_TypeIndex ti = JEL_component_map_get(&JEL_CTX->component_map, #component); \
    if (!ti) break; \
    struct JEL_Table *table = JEL_table_stack_get(&JEL_CTX->table_stack, etype); \
    struct component c = { __VA_ARGS__ }; \
    if (!JEL_type_index_check(etype, ti)) { \
      JEL_type_add(etype, ti); \
      JEL_type_add(JEL_CTX->entity_manager->types[JEL_entity_index(entity)], ti); \
      struct JEL_Table *new_table = JEL_table_stack_get(&JEL_CTX->table_stack, etype); \
      if (!new_table) { \
        new_table = JEL_table_stack_push(&JEL_CTX->table_stack, etype); \
      } \
      JEL_table_add(new_table, entity); \
      for (unsigned int i = 1; i < table->types_num; ++i) { \
        struct JEL_Component *cd = &JEL_CTX->component_table.components[table->types[i]]; \
        size_t size = cd->offsets[cd->props - 1] + cd->sizes[cd->props - 1]; \
        void *buffer; \
        if (!(buffer = malloc(size))) break; \
        JEL_table_get(table, entity, table->types[i], buffer); \
        JEL_table_set(new_table, entity, table->types[i], buffer); \
        free(buffer); \
      } \
      JEL_table_remove(table, entity); \
      table = new_table; \
    } \
    JEL_table_set(table, entity, ti, &c); \
  } \
  }

#define JEL_ENTITY_GET(entity, component, var) \
  switch (0) { \
  case 0: { \
    JEL_Type etype; \
    JEL_type_set(etype, JEL_CTX->entity_manager->types[JEL_entity_index(entity)]); \
    JEL_TypeIndex ti = JEL_component_map_get(&JEL_CTX->component_map, #component); \
    if (!ti) break; \
    struct JEL_Table *table = JEL_table_stack_get(&JEL_CTX->table_stack, etype); \
    JEL_table_get(table, entity, ti, var); \
  } \
  }

#endif
