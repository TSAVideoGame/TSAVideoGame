#include "texture.h"
#include <GL/glew.h>
#include "lodepng/lodepng.h"
#include "../core/logger.h"
#include <stdlib.h>

/*
 * JIN_texture_create
 *
 * @desc
 * @param texture
 * @param fpath
 * @return
 */
int JIN_texture_create (unsigned int *texture, const char *fpath)
{
  unsigned int   error;
  unsigned char *image;
  unsigned int   width, height;

  error = lodepng_decode32_file(&image, &width, &height, fpath);
  if (error) ERR_EXIT(-1, "Could not load image");

  glGenTextures(1, texture);
  glBindTexture(GL_TEXTURE_RECTANGLE, *texture);
  glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  
  glBindTexture(GL_TEXTURE_RECTANGLE, *texture);
  glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

  free(image);

  return 0;
}

/*
 * JIN_texture_destroy
 *
 * @desc
 * @param texture
 * @return
 */
int JIN_texture_destroy(unsigned int *texture)
{
  glDeleteTextures(1, texture);

  return 0;
}
