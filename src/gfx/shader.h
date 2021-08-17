#ifndef JIN_SHADER_H
#define JIN_SHADER_H

#include <GL/glew.h>

/*
 * SHADER
 *
 * Tools to deal with OpenGL Shaders
 */


/*
 * Shader functions
 *
 * create  | Create a shader (program)
 * destroy | Destroy a shader (program)
 */
int JIN_shader_create (unsigned int *shader, const char *fpath);
int JIN_shader_destory(unsigned int *shader);

#endif
