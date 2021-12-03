#ifndef JEL_PREFAB_H
#define JEL_PREFAB_H

#include "type.h"
#include "hierarchy.h"

/*
 * JEL_Prefab
 *
 * A Prefab is a ready made hierarchy of
 * entities with data
 *
 * This is useful for creating a lot of
 * entities with the same properties, like
 * projectiles
 */

struct JEL_Prefab {
  JEL_Type           type;
  void              *data;
  struct JEL_Prefab *parent;
  struct JEL_Prefab *child_first;
  struct JEL_Prefab *sibling_next;
};

/*
 * Prefab functions
 *
 * create             | Creates a prefab
 * destroy            | Destroys a prefab (the whole tree)
 * add                | Adds a child to a parent
 * generate           | Creates an entity with premade data
 * generate hierarchy | Generates a hierarchy from a prefab
 */

struct JEL_Prefab *    JEL_prefab_create(JEL_Type, void *);
int                    JEL_prefab_destroy(struct JEL_Prefab *);
int                    JEL_prefab_add(struct JEL_Prefab *, struct JEL_Prefab *);
JEL_Entity             JEL_prefab_generate(struct JEL_Prefab *);
struct JEL_Hierarchy * JEL_prefab_generate_hierarchy(struct JEL_Prefab *);

#endif
