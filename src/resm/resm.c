#include "resm.h"
#include <string.h>
#include <stdlib.h>
#include "../core/logger.h"

/* Sound */
#include "../snd/snd.h"

#define GROWTH_FACTOR 2
#define INITIAL_COUNT 8

/*
 * JIN_resm_allocate
 *
 * @desc
 * @param resm
 * @param count
 * @return
 */
#define ALLOC_VAR(type, name) \
  { \
    type name; \
    if (!(name = malloc(count * sizeof(*name)))) ERR_EXIT(-1, "Allocate resm: Out of memory"); \
    memcpy(name, resm->name, resm->count * sizeof(*name)); \
    free(resm->name); \
    resm->name = name; \
  }
static int JIN_resm_allocate(struct JIN_Resm *resm, unsigned int count)
{
  if (resm->count <= resm->allocated) return -1;

  ALLOC_VAR(void **,         resources);
  ALLOC_VAR(char **,         names);
  ALLOC_VAR(enum JIN_Rest *, types);

  resm->allocated = count;

  return 0;
}

/*
 * JIN_resm_create
 *
 * @desc
 * @param resm
 * @return
 */
int JIN_resm_create(struct JIN_Resm *resm)
{
  if (!(resm->resources = malloc(INITIAL_COUNT * sizeof(void *       )))) ERR_EXIT(-1, "Create resm: Out of memory");
  if (!(resm->names     = malloc(INITIAL_COUNT * sizeof(char *       )))) ERR_EXIT(-1, "Create resm: Out of memory");
  if (!(resm->types     = malloc(INITIAL_COUNT * sizeof(enum JIN_Rest)))) ERR_EXIT(-1, "Create resm: Out of memory");

  resm->count = 0;
  resm->allocated = INITIAL_COUNT;

  return 0;
}

/*
 * JIN_resm_destroy
 *
 * @desc
 * @param resm
 * @return
 */
void JIN_resm_destroy(struct JIN_Resm *resm)
{
  for (unsigned int i = 0; i < resm->count; ++i) {
    free(resm->names[i]);

    switch (resm->types[i]) {
      case JIN_RES_PNG:
        break;
      case JIN_RES_SFX:
        JIN_sndsfx_destroy(resm->resources[i]);
        break;
    }

    free(resm->resources[i]);
  }

  free(resm->resources);
  free(resm->names);
  free(resm->types);
}

/*
 * JIN_resm_add
 *
 * @desc
 * @param resm
 * @param name
 * @param fpath
 * @param type
 * @return
 */
int JIN_resm_add(struct JIN_Resm *resm, const char *name, const char *fpath, enum JIN_Rest type)
{
  if (resm->allocated <= resm->count) {
    if (JIN_resm_allocate(resm, resm->count * GROWTH_FACTOR)) ERR_EXIT(-1, "Could not allocate resm");
  }

  size_t name_size = strlen(name) + 1;
  resm->names[resm->count] = malloc(name_size * sizeof(char));
  for (size_t i = 0; i < name_size; ++i) resm->names[resm->count][i] = name[i];

  resm->types[resm->count] = type;

  switch (type) {
    case JIN_RES_PNG:
      resm->resources[resm->count] = malloc(sizeof(struct JIN_Sndbgm));
      JIN_sndbgm_create(resm->resources[resm->count], fpath);
      break;
    case JIN_RES_SFX:
      resm->resources[resm->count] = malloc(sizeof(struct JIN_Sndsfx));
      JIN_sndsfx_create(resm->resources[resm->count], fpath);
      break;
  }

  ++resm->count;

  return 0;
}

/*
 * JIN_resm_get
 *
 * @desc
 * @param resm
 * @param name
 * @return
 */
void * JIN_resm_get(struct JIN_Resm *resm, const char *name)
{
  for (unsigned int i = 0; i < resm->count; ++i) {
    if (!strcmp(resm->names[i], name)) {
      return resm->resources[i];
    }
  }

  return NULL;
}
