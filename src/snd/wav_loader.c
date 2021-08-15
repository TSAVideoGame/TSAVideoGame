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
#define CHECK(text, count) (strncmp(temp, text, count))
int JIN_wav_load(const char *fpath, char **buffer, uint8_t *channels, int32_t *sample_rate, uint8_t *bits_per_sample, int32_t *size)
{
  FILE    *file;
  char     temp[4];

  if (!(file = fopen(fpath, "rb"))) return -1;

  /* Read the RIFF */
  READ(temp, 4); /* Chunk id */
  READ(temp, 4); /* Chunk size */
  READ(temp, 4); /* File format */
  CHECK("WAVE", 4);

  /* Read fmt (some files have JUNK after RIFF) */
  READ(temp, 4); /* Chunk id */
  if (CHECK("fmt", 3)) {
    while (CHECK("fmt", 3)) {
      READ(temp, 4); /* Chunk size */
      uint32_t offset = *(uint32_t *) temp;
      if (fseek(file, offset, SEEK_CUR)) return -1; /* Skip past the chunk */
      READ(temp, 4); /* Chunk id */
    }
  }
  READ(temp, 4); /* Chunk size */
  uint32_t fmt_end = ftell(file) + *(uint32_t *) temp;
  READ(temp, 2); /* PCM */
  READ(temp, 2); /* Channel num */
  *channels = *(uint8_t *) temp;
  READ(temp, 4); /* Sample rate */
  *sample_rate = *(int32_t *) temp;
  READ(temp, 4); /* Byte rate */
  READ(temp, 2); /* Block align */
  READ(temp, 2); /* Bits per sample */
  *bits_per_sample = *(uint8_t *) temp;

  if (ftell(file) != fmt_end)
    fseek(file, fmt_end, SEEK_SET);

  /* Data */
  READ(temp, 4); /* Chunk id */
  if (CHECK("data", 4)) {
    while (CHECK("data", 4)) {
      READ(temp, 4); /* Chunk size */
      uint32_t offset = *(uint32_t *) temp;
      if (fseek(file, offset, SEEK_CUR)) return -1; /* Skip past the chunk */
      READ(temp, 4); /* Chunk id */
    }
  }
  READ(temp, 4); /* Chunk size */
  *size = *(int32_t *) temp;

  *buffer = malloc(*size * sizeof(char));
  READ(*buffer, *size);

  fclose(file);

  return 0;
}

int JIN_wav_unload(char **buffer)
{
  free(*buffer);

  return 0;
}
