#include "snd.h"
#include <AL/alc.h>
#include <stdlib.h>

static ALCcontext *context;
static ALCdevice  *device;

int JIN_snd_init(void)
{

  if (!(device = alcOpenDevice(NULL))) return -1;
  context = alcCreateContext(device, NULL);

  alcMakeContextCurrent(context);

  return 0;
}

int JIN_snd_quit(void)
{
  alcMakeContextCurrent(NULL);
  alcDestroyContext(context);
  alcCloseDevice(device);

  return 0;
}
