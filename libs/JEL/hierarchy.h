#ifndef JEL_HIERARCHY_h
#define JEL_HIERARCHY_h

#include "entity.h"

struct JEL_Node {
  JEL_Entity       entity;
  struct JEL_Node *parent;
  struct JEL_Node *child_first;
  struct JEL_Node *sibling_next;
};

struct JEL_Hierarchy {
  struct JEL_Node *root;
};


struct JEL_Node * JEL_node_create(JEL_Entity);
/* 
 * Hierarchy functions
 *
 * create  | Creates a hierarchy
 * destroy | Destroyes a hierarchy
 * add     | Adds a child to a parent node
 * attach  | Attachs a hierarchy to a node
 * detach  | Removes a node (and all children) and make it a hierarchy
 */
struct JEL_Hierarchy * JEL_hierarchy_create(void);
int                    JEL_hierarchy_destroy(struct JEL_Hierarchy *);
int                    JEL_hierarchy_add(struct JEL_Node *, struct JEL_Node *);
int                    JEL_hierarchy_attach(struct JEL_Node *, struct JEL_Hierarchy *);
struct JEL_Hierarchy * JEL_hierarchy_detach(struct JEL_Node *);

void                   JEL_hierarchy_iterate_up(struct JEL_Node *, void (*)(struct JEL_Node *));
void                   JEL_hierarchy_iterate_down(struct JEL_Node *, void (*)(struct JEL_Node *));

#endif
