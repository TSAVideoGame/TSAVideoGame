#ifndef JEL_ENTITY_H
#define JEL_ENTITY_H

#include <stdint.h>
#include "type.h"

/*
 * JEL_Entity
 *
 * Entities are simply numbers
 * They are weak references, which is why some bits will go
 * to their index, and some for their generation
 *
 * A generation is when an index has to be re-used
 *
 * All the entities will be stored in an array,
 * and will be managed through JEL_EntityManager
 *
 * The reason for two typdefs is to distinguish
 * what is an integer the same size as an entity,
 * and what is an entity itself.
 *
 * It is typedef-ed because you don't need to
 * use a uint32_t, you could do a uint64_t or
 * uint16_t
 */

#define JEL_ENTITY_INDEX_BITS      24
#define JEL_ENTITY_GENERATION_BITS  8

typedef uint32_t   JEL_Entity;
typedef JEL_Entity JEL_EntityInt;

/*
 * JEL_EntityManager
 *
 * Stores generations of entities to check
 * if they're alive and stores the entities
 * types
 *
 * The free indicies is more internal to the
 * manager, it helps with recycling generations
 * instead of creating new ones
 */

struct JEL_EntityManager {
  JEL_EntityInt  entities_allocated;
  JEL_EntityInt  entities_num;
  JEL_EntityInt *generations;
  JEL_Type      *types;

  JEL_EntityInt *free_indices;
  JEL_EntityInt  free_indices_num;
  JEL_EntityInt  free_indices_allocated;
};

/*
 * Entity functions
 *
 * create  | Create an entity
 * destroy | Destroy an entity
 * index   | Get the index of an entity
 * gen     | Get the generation of an entity
 * alive   | Check if the entity is alive
 */

JEL_Entity    JEL_entity_create (void);
int           JEL_entity_destroy(JEL_Entity);
JEL_EntityInt JEL_entity_index  (JEL_Entity);
JEL_EntityInt JEL_entity_gen    (JEL_Entity);
int           JEL_entity_alive  (JEL_Entity);

#endif
