#include "snd.h"
#include <AL/alc.h>
#include <AL/al.h>
#include <stdlib.h>

#include <stdio.h> /* Temp */
#include "wav_loader.h"

static ALCcontext *context;
static ALCdevice  *device;

/* SOUND FUNCTIONS */

int JIN_snd_init(void)
{
  if (!(device = alcOpenDevice(NULL))) return -1;
  context = alcCreateContext(device, NULL);

  alcMakeContextCurrent(context);

  /* Temp code */
  /*
  uint8_t  channels;
  int32_t  sample_rate;
  uint8_t  bits_per_sample;
  int32_t  size;
  char    *data = NULL;

  if (JIN_wav_load("res/L.wav", &data, &channels, &sample_rate, &bits_per_sample, &size)) printf("Error loading wav\n");
  
  ALuint buffer;
  alGenBuffers(1, &buffer);

  ALenum format;
  if (channels == 1 && bits_per_sample == 8)
    format = AL_FORMAT_MONO8;
  else if (channels == 1 && bits_per_sample == 16)
    format = AL_FORMAT_MONO16;
  else if (channels == 2 && bits_per_sample == 8)
    format = AL_FORMAT_STEREO8;
  else if (channels == 2 && bits_per_sample == 16)
    format = AL_FORMAT_STEREO16;
  else
    printf("Unknown format\n");

  ALenum error;
  if ((error = alGetError()) != AL_NO_ERROR) printf("Error before generating buffer\n");
  alBufferData(buffer, format, data, size, sample_rate);
  if ((error = alGetError()) != AL_NO_ERROR) printf("Error after generating buffer\n");
  JIN_wav_unload(&data);

  ALuint source;
  alGenSources(1, &source);
  alSourcef(source, AL_PITCH, 1);
  alSourcef(source, AL_GAIN, 1.0f);
  alSource3f(source, AL_POSITION, 0, 0, 0);
  alSource3f(source, AL_VELOCITY, 0, 0, 0);
  alSourcei(source, AL_LOOPING, AL_FALSE);
  alSourcei(source, AL_BUFFER, buffer);

  alSourcePlay(source);

  ALint state = AL_PLAYING;
  while (state == AL_PLAYING)
    alGetSourcei(source, AL_SOURCE_STATE, &state);

  alDeleteSources(1, &source);
  alDeleteBuffers(1, &buffer);
  */

  return 0;
}

int JIN_snd_quit(void)
{
  alcMakeContextCurrent(NULL);
  alcDestroyContext(context);
  alcCloseDevice(device);
  
  return 0;
}

/* SFX FUNCTIONS */

int JIN_sndsfx_create(struct JIN_Sndsfx *sfx, const char *fpath)
{
  uint8_t  channels;
  int32_t  sample_rate;
  uint8_t  bits_per_sample;
  int32_t  size;
  char    *data;

  if (JIN_wav_load(fpath, &data, &channels, &sample_rate, &bits_per_sample, &size)) return -1;

  alGenBuffers(1, &sfx->buffer);

  ALenum format;
  if (channels == 1 && bits_per_sample == 8)
    format = AL_FORMAT_MONO8;
  else if (channels == 1 && bits_per_sample == 16)
    format = AL_FORMAT_MONO16;
  else if (channels == 2 && bits_per_sample == 8)
    format = AL_FORMAT_MONO8;
  else if (channels == 2 && bits_per_sample == 16)
    format = AL_FORMAT_STEREO16;
  else
    return -1; /* Unknown format */

  alBufferData(sfx->buffer, format, data, size, sample_rate);
  JIN_wav_unload(&data);

  return 0;
}

int JIN_sndsfx_destroy(struct JIN_Sndsfx *sfx)
{
  alDeleteBuffers(1, &sfx->buffer);
  return 0;
}

/* BGM FUNCTIONS */
int JIN_sndbgm_create (struct JIN_Sndbgm *);
int JIN_sndsbgm_destroy(struct JIN_Sndbgm *);
