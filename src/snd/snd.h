#ifndef JIN_SND_H
#define JIN_SND_H

#include <AL/al.h>
#include <stdint.h>
#include <stdlib.h>

/*
 * SOUND
 *
 * Plays audio
 *
 * There is background music (bgm) and
 * sound effects (sfx)
 *
 * TYPES:
 * struct JIN_Sndsfx | Sound effects (OpenAL buffer, not source)
 * struct JIN_Sndbgm | Background music
 */

struct JIN_Sndsfx {
  ALuint buffer; 
};

struct JIN_Sndbgm {
  ALuint *buffers;
  ALenum  format;
  int32_t sample_rate;
  char   *data;
  int32_t data_size;
  size_t  cursor;
  ALuint  source;
};

/*
 * Sound functions
 *
 * init   | Set up sound
 * quit   | Quit sound
 * update | Update for bgm
 */
int JIN_snd_init(void);
int JIN_snd_quit(void);

/*
 * Sfx functions
 *
 * create  | Create a sfx
 * destroy | Destroy a sfx
 */
int JIN_sndsfx_create (struct JIN_Sndsfx *, const char *fpath);
int JIN_sndsfx_destroy(struct JIN_Sndsfx *);

/*
 * Bgm functions
 *
 * update  | Update the bgm buffers
 * create  | Create bgm
 * destroy | Destroy bgm
 * set     | Set what bgm will play
 * play    | Play the bgm
 * stop    | Stop/pause the bgm
 */
int JIN_sndbgm_update (struct JIN_Sndbgm *);
int JIN_sndbgm_create (struct JIN_Sndbgm *, const char *fpath);
int JIN_sndbgm_destroy(struct JIN_Sndbgm *);
int JIN_sndbgm_set    (const char *fpath);
int JIN_sndbgm_play   (void);
int JIN_sndbgm_stop   (void);

#endif
