#include "states.h"
#include "../resm/resm.h"
#include "../globals.h"
#include "snd/snd.h"

static int test_fn_create(struct STM_State *state)
{
  JIN_resm_add(&JIN_resm, "L", "res/L.wav", JIN_RES_SFX); 
  
  return 0;
}

static int test_fn_destroy(struct STM_State *state)
{
  return 0;
}

static int test_fn_update(struct STM_State *state)
{
  ALuint source;
  alGenSources(1, &source);
  alSourcef(source, AL_PITCH, 1);
  alSourcef(source, AL_GAIN, 1.0f);
  alSource3f(source, AL_POSITION, 0, 0, 0);
  alSource3f(source, AL_VELOCITY, 0, 0, 0);
  alSourcei(source, AL_LOOPING, AL_FALSE);
  alSourcei(source, AL_BUFFER, ((struct JIN_Sndsfx *) JIN_resm_get(&JIN_resm, "L"))->buffer);

  alSourcePlay(source);

  ALint status = AL_PLAYING;
  while (status == AL_PLAYING)
    alGetSourcei(source, AL_SOURCE_STATE, &status);

  alDeleteSources(1, &source);

  return 0;
}

static int test_fn_draw(struct STM_State *state)
{
  return 0;
}

int JIN_states_test_create(struct STM_State *state)
{
  if (STM_state_create(state, "test", 0, 0, test_fn_create, test_fn_destroy, test_fn_update, test_fn_draw)) return -1;

  return 0;
}

