#ifndef JIN_WAV_LOADER_H
#define JIN_WAV_LOADER_H

#include <stdint.h>

int JIN_wav_load  (const char *fpath, char *buffer, uint8_t *channels, int32_t *sample_rate, uint8_t *bits_per_sample, uint32_t *size);
int JIN_wav_unload(char *buffer);

#endif
