#ifndef JIN_SND_H
#define JIN_SND_H

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
 * struct JIN_Sndd | Sound data
 */

struct JIN_Sndd {
};

/*
 * Sound functions
 */
int JIN_snd_init(void);
int JIN_snd_quit(void);

/*
 * Sound data functions
 */
int JIN_sndd_create (struct JIN_Sndd *);
int JIN_sndd_destroy(struct JIN_Sndd *);

#endif
