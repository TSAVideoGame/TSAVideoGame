#ifndef JIN_TEXTURE_H
#define JIN_TEXTURE_H

/*
 * TEXTURE
 *
 * Png images
 * The resource is just an unsigned int
 */

/*
 * Texture Functions
 */
int JIN_texture_create (unsigned int *texture, const char *fpath);
int JIN_texture_destroy(unsigned int *texture);

#endif
