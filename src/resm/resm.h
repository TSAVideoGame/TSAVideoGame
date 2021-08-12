#ifndef JIN_RESM_H
#define JIN_RESM_H

/*
 * RESOURCE MANAGER
 *
 * A resource is a 'thing' the game uses like
 * an image, a sound, or a script (they are
 * all external files)
 *
 * Resources are shared across the game, so
 * they will be accesed by a unsigned long int
 * id and a type
 *
 * TYPES:
 * enum   JIN_Rest | Types of resources
 * struct JIN_Resm | A resource manager
 *
 */

enum JIN_Rest {
  /* Image */
  JIN_RES_PNG,
  /* Sound */
  JIN_RES_SFX,
  JIN_RES_BGM
};

struct JIN_Resm {
  unsigned int   allocated;
  unsigned int   count;
  void         **resources;
  char         **names;
  enum JIN_Rest *types;
};

/*
 * Resource Manager Functions
 *
 * create  | Create a resource manager
 * destroy | Destroy a resource manager
 * add     | Add a resource
 * get     | Get a resource
 */
int    JIN_resm_create (struct JIN_Resm *resm);
void   JIN_resm_destroy(struct JIN_Resm *resm);
int    JIN_resm_add    (struct JIN_Resm *resm, const char *name, const char *fpath, enum JIN_Rest type);
void * JIN_resm_get    (struct JIN_Resm *resm, const char *name);

#endif
