#include "snd.h"
#include <AL/alc.h>
#include <AL/al.h>
#include <stdlib.h>

#include "wav_loader.h"

/*
 * GLOBALS AND MACROS
 *
 * context      | Global ALCcontext
 * device       | Global ALCdevice
 * bgm_source   | Global ALC source for bgm
 * bgm_current  | Pointer to the set bgm struct
 * BUFFERS_NUM  | Buffers for bgm
 * BUFFERS_SIZE | Size of a bgm buffer
 */
static ALCcontext        *context;
static ALCdevice         *device;
static ALuint             bgm_source;
static struct JIN_Sndbgm *bgm_current;

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

  /* BGM source */
  alGenSources(1, &bgm_source);
  alSourcef(bgm_source, AL_PITCH, 1);
  alSourcef(bgm_source, AL_GAIN, 1.0f);
  alSource3f(bgm_source, AL_POSITION, 0, 0, 0);
  alSource3f(bgm_source, AL_VELOCITY, 0, 0, 0);
  alSourcei(bgm_source, AL_LOOPING, AL_FALSE);
  
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
  alDeleteSources(1, &bgm_source);
  
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

/*
 * JIN_sndbgm_update
 *
 * @desc
 * @param bgm
 * @return
 */
int JIN_sndbgm_update(struct JIN_Sndbgm *bgm)
{
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
  int32_t  size;

  if (JIN_wav_load(fpath, &bgm->data, &channels, &bgm->sample_rate, &bits_per_sample, &size)) return -1;

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
  if (size < BUFFERS_NUM * BUFFERS_SIZE) return -1;
  for (int i = 0; i < BUFFERS_NUM; ++i) {
    alBufferData(bgm->buffers[i], bgm->format, &bgm->data[i * BUFFERS_SIZE], BUFFERS_SIZE, bgm->sample_rate);
  }

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
int JIN_sndbgm_set(struct JIN_Sndbgm *bgm)
{
  alSourceQueueBuffers(bgm_source, BUFFERS_NUM, bgm->buffers);
  
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
  return 0;
}
