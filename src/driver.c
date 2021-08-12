#include "resource_manager/resm.h"
#include "sound/snd.h"

#include <stdio.h>

int main(int argc, char *args[])
{
  JIN_snd_init();

  struct JIN_Resm resm;
  JIN_resm_create(&resm);

  JIN_resm_add(&resm, "L", "res/L.wav", JIN_RES_SFX); 
  
  ALuint source;
  alGenSources(1, &source);
  alSourcef(source, AL_PITCH, 1);
  alSourcef(source, AL_GAIN, 1.0f);
  alSource3f(source, AL_POSITION, 0, 0, 0);
  alSource3f(source, AL_VELOCITY, 0, 0, 0);
  alSourcei(source, AL_LOOPING, AL_FALSE);
  alSourcei(source, AL_BUFFER, ((struct JIN_Sndsfx *) JIN_resm_get(&resm, "L"))->buffer);

  alSourcePlay(source);

  ALint state = AL_PLAYING;
  while (state == AL_PLAYING)
    alGetSourcei(source, AL_SOURCE_STATE, &state);

  alDeleteSources(1, &source);

  JIN_resm_destroy(&resm);
  JIN_snd_quit();
  
  return 0;
}
