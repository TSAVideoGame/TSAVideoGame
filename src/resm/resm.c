#include "resm.h"
#include <string.h>
#include <stdlib.h>
#include "core/logger/logger.h"

/* Graphics */
#include "gfx/gfx.h"

#include "anim/anim.h"

/* Sound */
#include "snd/snd.h"

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
static int RESM_allocate(struct RESM_M *resm, unsigned int count)
{
  if (resm->count <= resm->allocated) return -1;

  ALLOC_VAR(void **,         resources);
  ALLOC_VAR(char **,         names);
  ALLOC_VAR(enum RESM_T *,   types);

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
int RESM_create(struct RESM_M *resm)
{
  if (!(resm->resources = malloc(INITIAL_COUNT * sizeof(void *       )))) ERR_EXIT(-1, "Create resm: Out of memory");
  if (!(resm->names     = malloc(INITIAL_COUNT * sizeof(char *       )))) ERR_EXIT(-1, "Create resm: Out of memory");
  if (!(resm->types     = malloc(INITIAL_COUNT * sizeof(enum RESM_T)))) ERR_EXIT(-1, "Create resm: Out of memory");

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
void RESM_destroy(struct RESM_M *resm)
{
  for (unsigned int i = 0; i < resm->count; ++i) {
    free(resm->names[i]);

    switch (resm->types[i]) {
      case RESM_SHADER:
        JIN_shader_destory(resm->resources[i]);
        break;
      case RESM_PNG:
        JIN_texture_destroy(resm->resources[i]);
        break;
      case RESM_MODEL:
        JIN_model_destory(resm->resources[i]);
        break;
      case RESM_ANIM:
        JIN_animd_destroy(resm->resources[i]);
        break;
      case RESM_SFX:
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
int RESM_add(struct RESM_M *resm, const char *name, const char *fpath, enum RESM_T type)
{
  if (resm->allocated <= resm->count) {
    if (RESM_allocate(resm, resm->count * GROWTH_FACTOR)) ERR_EXIT(-1, "Could not allocate resm");
  }

  size_t name_size = strlen(name) + 1;
  resm->names[resm->count] = malloc(name_size * sizeof(char));
  for (size_t i = 0; i < name_size; ++i) resm->names[resm->count][i] = name[i];

  resm->types[resm->count] = type;

  switch (type) {
    case RESM_SHADER:
      resm->resources[resm->count] = malloc(sizeof(unsigned int));
      JIN_shader_create(resm->resources[resm->count], fpath);
      break;
    case RESM_PNG:
      resm->resources[resm->count] = malloc(sizeof(unsigned int));
      JIN_texture_create(resm->resources[resm->count], fpath);
      break;
    case RESM_MODEL:
      resm->resources[resm->count] = malloc(sizeof(struct JIN_Model));
      JIN_model_create(resm->resources[resm->count], fpath);
      break;
    case RESM_ANIM:
      resm->resources[resm->count] = malloc(sizeof(struct JIN_Animd));
      JIN_animd_create(resm->resources[resm->count], fpath);
      break;
    case RESM_SFX:
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
 *   TODO: Use map instead of linear search
 * @param resm
 * @param name
 * @return
 */
void * RESM_get(struct RESM_M *resm, const char *name)
{
  for (unsigned int i = 0; i < resm->count; ++i) {
    if (!strcmp(resm->names[i], name)) {
      return resm->resources[i];
    }
  }

  return NULL;
}

/*
 * JIN_resm_add
 */
#include "core/globals.h"
int JIN_resm_add(const char *name, const char *fpath, enum RESM_T type)
{
  return RESM_add(&JIN_resm, name, fpath, type);
}

void * JIN_resm_get(const char *name)
{
  return RESM_get(&JIN_resm, name);
}
