#ifndef JIN_WAV_UTIL_H
#define JIN_WAV_UTIL_H

#include <stdint.h>
#include <AL/al.h>

struct JIN_Wavd {
  uint8_t channels;
  int32_t sample_rate;
  uint8_t bits_per_sample;
  int32_t size;
};

int JIN_wav_load  (const char *fpath, struct JIN_Wavd *data, char **buffer, int32_t *data_start);
int JIN_wav_format(struct JIN_Wavd *data, ALenum *format);

#endif
