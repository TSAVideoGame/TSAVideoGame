#include "resm/resm.h"
#include "core/logger/logger.h"

/*
 * CORE INIT RES
 *
 * A list of resources to initialize
 */

#define INIT_RESOURCE_LIST \
  X("player_animation", "res/animations/player.animd", RESM_ANIM) \
  X("guard_animation", "res/animations/guard.animd", RESM_ANIM) \

static int init_resources(void)
{
  #define X(name, path, type) if (JIN_resm_add(name, path, type)) ERR_EXIT(-1, "Can't create resource at " #path ". Res type is " #type);
  INIT_RESOURCE_LIST
  #undef X
  return 0;
}
