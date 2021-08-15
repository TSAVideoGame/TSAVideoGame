#include "snd.h"
#include <AL/alc.h>
#include <AL/al.h>
#include <stdlib.h>
#include <string.h>

#include "wav_loader.h"

/*
 * GLOBALS AND MACROS
 *
 * context      | Global ALCcontext
 * device       | Global ALCdevice
 * BUFFERS_NUM  | Buffers for bgm
 * BUFFERS_SIZE | Size of a bgm buffer
 */
static ALCcontext        *context;
static ALCdevice         *device;

#define BUFFERS_NUM 4
#define BUFFERS_SIZE 32768

/* SOUND FUNCTIONS */

/*
 * JIN_snd_init
 *
 * @desc
 * @return
 */
int JIN_snd_init(void)
{
  /* Device and context */
  if (!(device = alcOpenDevice(NULL))) return -1;
  context = alcCreateContext(device, NULL);

  alcMakeContextCurrent(context);

  return 0;
}

/*
 * JIN_snd_quit
 *
 * @desc
 * @return
 */
int JIN_snd_quit(void)
{
  alcMakeContextCurrent(NULL);
  alcDestroyContext(context);
  alcCloseDevice(device);
  
  return 0;
}

/* SFX FUNCTIONS */

/*
 * JIN_sndsfx_create
 *
 * @desc
 * @param sfx
 * @param fpath
 * @return
 */
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

/*
 * JIN_sndsfx_destroy
 *
 * @desc
 * @param sfx
 * @return
 */
int JIN_sndsfx_destroy(struct JIN_Sndsfx *sfx)
{
  alDeleteBuffers(1, &sfx->buffer);
  return 0;
}

/* BGM FUNCTIONS */

#include "../core/globals.h"

/*
 * JIN_sndbgm_update
 *
 * @desc
 * @param bgm
 * @return
 */
int JIN_sndbgm_update(struct JIN_Sndbgm *bgm)
{
  ALint buffers_processed = 0;
  alGetSourcei(bgm->source, AL_BUFFERS_PROCESSED, &buffers_processed);

  if (buffers_processed <= 0) return 0;

  while (buffers_processed--) {
    ALuint buffer;
    alSourceUnqueueBuffers(bgm->source, 1, &buffer);

    char data[BUFFERS_SIZE];

    size_t copy_size = BUFFERS_SIZE;
    if (bgm->cursor + BUFFERS_SIZE > bgm->data_size) {
      copy_size = bgm->data_size - bgm->cursor;
    }

    memcpy(data, &bgm->data[bgm->cursor], copy_size);
    bgm->cursor += copy_size;

    /* Ensure no empty space, will loop immediately */
    if (copy_size < BUFFERS_SIZE) {
      bgm->cursor = 0;
      memcpy(&data[copy_size], &bgm->data[bgm->cursor], BUFFERS_SIZE - copy_size);
      bgm->cursor = BUFFERS_SIZE - copy_size;
    }

    alBufferData(buffer, bgm->format, data, BUFFERS_SIZE, bgm->sample_rate);
    alSourceQueueBuffers(bgm->source, 1, &buffer);
  }

  return 0;
}

/*
 * JIN_sndbgm_create
 *
 * @desc
 * @param bgm
 * @param fpath
 * @return
 */
int JIN_sndbgm_create(struct JIN_Sndbgm *bgm, const char *fpath)
{
  uint8_t  channels;
  uint8_t  bits_per_sample;

  if (JIN_wav_load(fpath, &bgm->data, &channels, &bgm->sample_rate, &bits_per_sample, &bgm->data_size)) return -1;

  if (!(bgm->buffers = malloc(BUFFERS_NUM * sizeof(ALuint)))) return -1;
  alGenBuffers(BUFFERS_NUM, bgm->buffers);

  if (channels == 1 && bits_per_sample == 8)
    bgm->format = AL_FORMAT_MONO8;
  else if (channels == 1 && bits_per_sample == 16)
    bgm->format = AL_FORMAT_MONO16;
  else if (channels == 2 && bits_per_sample == 8)
    bgm->format = AL_FORMAT_MONO8;
  else if (channels == 2 && bits_per_sample == 16)
    bgm->format = AL_FORMAT_STEREO16;
  else
    return -1; /* Unknown format */

  /* Audio data must be bigger than BUFFERS_NUM * BUFFERS_SIZE */
  if (bgm->data_size < BUFFERS_NUM * BUFFERS_SIZE) return -1;
  for (int i = 0; i < BUFFERS_NUM; ++i) {
    alBufferData(bgm->buffers[i], bgm->format, &bgm->data[i * BUFFERS_SIZE], BUFFERS_SIZE, bgm->sample_rate);
  }

  alGenSources(1, &bgm->source);
  alSourcef(bgm->source, AL_PITCH, 1);
  alSourcef(bgm->source, AL_GAIN, 1.0f);
  alSource3f(bgm->source, AL_POSITION, 0, 0, 0);
  alSource3f(bgm->source, AL_VELOCITY, 0, 0, 0);
  alSourcei(bgm->source, AL_LOOPING, AL_FALSE);

  alSourceQueueBuffers(JIN_sndbgm.source, BUFFERS_NUM, JIN_sndbgm.buffers);

  bgm->cursor = BUFFERS_SIZE * BUFFERS_NUM;

  return 0;
}

/*
 * JIN_sndsbgm_destroy
 *
 * @desc
 * @param bgm
 * @return
 */
int JIN_sndbgm_destroy(struct JIN_Sndbgm *bgm)
{
  alDeleteSources(1, &bgm->source);

  JIN_wav_unload(&bgm->data);
  alDeleteBuffers(BUFFERS_NUM, bgm->buffers);
  free(bgm->buffers);
  return 0;
}

/*
 * JIN_sndbgm_set
 *
 * @desc
 * @param bgm
 * @return
 */
int JIN_sndbgm_set(const char *fpath)
{
  JIN_sndbgm_destroy(&JIN_sndbgm);
  JIN_sndbgm_create(&JIN_sndbgm, fpath);
  
  return 0;
}

/*
 * JIN_sndbgm_play
 *
 * @desc
 * @return
 */
int JIN_sndbgm_play(void)
{
  alSourcePlay(JIN_sndbgm.source);

  return 0;
}

/*
 * JIN_sndbgm_stop
 *
 * @desc
 * @return
 */
int JIN_sndbgm_stop()
{
  alSourcePause(JIN_sndbgm.source);

  return 0;
}
