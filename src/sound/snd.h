#ifndef JIN_SND_H
#define JIN_SND_H

#include <AL/al.h>

/*
 * SOUND
 *
 * Plays audio
 *
 * There is background music (bgm) and
 * sound effects (sfx)
 *
 * tasty tutorial: http://www.david-amador.com/2011/06/playing-sound-using-openal/
 *
 * TYPES:
 * struct JIN_Sndsfx | Sound effects (OpenAL buffer, not source)
 * struct JIN_Sndbgm | Background music
 */

struct JIN_Sndsfx {
  ALuint buffer; 
};

struct JIN_Sndbgm {
};

/*
 * Sound functions
 */
int JIN_snd_init(void);
int JIN_snd_quit(void);

/*
 * Sfx functions
 */
int JIN_sndsfx_create (struct JIN_Sndsfx *, const char *fpath);
int JIN_sndsfx_destroy(struct JIN_Sndsfx *);

/*
 * Bgm functions
 */
int JIN_sndbgm_create (struct JIN_Sndbgm *);
int JIN_sndsbgm_destroy(struct JIN_Sndbgm *);

#endif
