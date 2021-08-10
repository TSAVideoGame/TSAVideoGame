#include "wav_loader.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
 * @desc
 *   Load a wav into a char buffer.
 *   Wav spec from http://soundfile.sapp.org/doc/WaveFormat/
 */
#define READ(var, count) if (fread(var, sizeof(*var), count, file) != count) return -1;
#define CHECK(text, count) if (strncmp(buffer, text, count)) return -1;
int JIN_wav_load(const char *fpath, char *buffer, uint8_t *channels, int32_t *sample_rate, uint8_t *bits_per_sample, uint32_t *size)
{
  FILE *file;
  char  temp[4];

  if (!(file = fopen(fpath, "rb"))) return -1;

  /* Read the RIFF */
  READ(temp, 4); /* Chunk id */
  CHECK("RIFF", 4);
  READ(temp, 4); /* Chunk size */
  READ(temp, 4); /* File format */
  CHECK("WAVE", 4);

  /* Read fmt */
  READ(temp, 4); /* Chunk id */
  CHECK("fmt", 3);
  READ(temp, 4); /* Chunk size */
  READ(temp, 2); /* PCM */
  READ(temp, 2); /* Channel num */
  /* TODO write channel param */
  READ(temp, 4); /* Sample rate */
  READ(temp, 4); /* Byte rate */
  READ(temp, 2); /* Block align */
  READ(temp, 2); /* Bits per sample */

  /* Data */
  READ(temp, 4); /* Chunk id */
  CHECK("data", 4);
  READ(temp, 4); /* Chunk size */
  *size = *(uint32_t *) temp;

  buffer = malloc(*size * sizeof(char));
  READ(buffer, *size);

  return 0;
}

int JIN_wav_unload(char *buffer)
{
  free(buffer);

  return 0;
}
