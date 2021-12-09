#include "gfx.h"
#include <stdint.h>
#include <stdlib.h>
#include "core/gll/gll.h"
#include <stdio.h>
#include "lodepng/lodepng.h"
#include "core/core.h"
#include "cglm/cglm.h"
#include "resm/resm.h"

/* GFX FUNCTIONS */

/*
 * JIN_gfx_draw_sprite
 *
 * @desc
 *   TODO: Figure out a way to batch sprites instead of
 *   drawing them one by one
 * @param shader
 * @param texture
 * @param x
 * @param y
 * @param w
 * @param h
 * @param t_x
 * @param t_y
 * @param t_w
 * @param t_h
 * @return
 */
int JIN_gfx_draw_sprite(unsigned int *shader, unsigned int *texture, int x, int y, int w, int h, int t_x, int t_y, int t_w, int t_h)
{
  glUseProgram(*shader);

  mat4 model = GLM_MAT4_IDENTITY_INIT;

  glm_translate(model, (vec3) {x, y, 0.0f});
  glm_scale(model, (vec3) {w, h, 1.0f});

  glUniformMatrix4fv(glGetUniformLocation(*shader, "model"), 1, GL_FALSE, (float *) model);

  glUniform2f(glGetUniformLocation(*shader, "tex_scale"), t_w, t_h);
  glUniform2f(glGetUniformLocation(*shader, "tex_translate"), t_x, t_y);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, *texture);

  glBindVertexArray(((struct JIN_Model *) JIN_resm_get("JIN_MODEL_SPRITE"))->vao);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);

  return 0;
}

/* SHADER FUNCTIONS */

/*
 * file_to_str
 *
 * @desc
 * @param fpath
 * @return
 */
int file_to_str(const char *fpath, char **str)
{
  FILE  *file;
  size_t size;
  
  if (!(file = fopen(fpath, "r"))) ERR_EXIT(-1, "Couldn't open file: %s", fpath);

  size = 1; /* Account for '\0' */
  while (fgetc(file) != EOF) {
    ++size;
  }
  if (!(*str = malloc(size))) ERR_EXIT(-1, "Out of memory");
  if (fseek(file, 0, SEEK_SET)) ERR_EXIT(-1, "Could not seek in file");

  if (fread(*str, sizeof(char), size - 1, file) != size - 1) ERR_EXIT(-1, "Could not read file");
  (*str)[size - 1] = '\0';

  fclose(file);

  return 0;
}

/*
 * JIN_shader_create
 *
 * TODO: Clean this up
 * @desc
 * @param shader
 * @param fpath
 * @return
 */
int JIN_shader_create(unsigned int *shader, const char *fpath)
{
  FILE        *shdr;
  char         temp[4];
  unsigned int shaders[2];
  int          shader_index;
  int          success;

  if (!(shdr = fopen(fpath, "rb"))) ERR_EXIT(-1, "Could not open .shdr file file: %s", fpath);
  
  shader_index = 0;
  while (fread(temp, sizeof(char), 4, shdr) == 4) {
    GLenum shader_type;
    char   shader_name[64];
    char   shader_path[128];
    char  *shader_src;
    
    if (!strncmp(temp, "VRTX", 4)) {
      shader_type = GL_VERTEX_SHADER;
    }
    else if (!strncmp(temp, "FGMT", 4)) {
      shader_type = GL_FRAGMENT_SHADER;
    }
    else {
      LOG(ERR, "Unhandled shader type");
      continue;
    }

    /* Read the ':' */
    if (fseek(shdr, 1, SEEK_CUR)) ERR_EXIT(-1, "Could not skip past ':' in file");
   
    /* Figure out the path of the shader */
    if (fgets(shader_name, 64, shdr) != shader_name) ERR_EXIT(-1, "Could not read file (fgets)");
    int endpt;
    endpt = -1;
    while (shader_name[++endpt] != '\n');
    shader_name[endpt] = '\0';
    endpt = -1;
    while (fpath[++endpt] != '\0');
    while (fpath[--endpt] != '/');
    shader_path[0] = '\0';
    strncat(shader_path, fpath, ++endpt);
    strncat(shader_path, shader_name, 64);

    if (file_to_str(shader_path, &shader_src)) ERR_EXIT(-1, "Couldn't convert shader file to string (%s)", shader_path);
    shaders[shader_index] = glCreateShader(shader_type);
    const char *tmp = shader_src;
    glShaderSource(shaders[shader_index], 1, &tmp, NULL);
    glCompileShader(shaders[shader_index]);
 
    glGetShaderiv(shaders[shader_index], GL_COMPILE_STATUS, &success);
    if (!success) {
      char info[512];
      glGetShaderInfoLog(shaders[shader_index], 512, NULL, info);
      printf("ERROR: %s\n)", info);
      ERR_EXIT(-1, "Error while compiling the shader");
    }

    free(shader_src);

    ++shader_index;
  }

  *shader = glCreateProgram();
  for (int i = 0; i < 2; ++i)
    glAttachShader(*shader, shaders[i]);
  glLinkProgram(*shader);

  glGetProgramiv(*shader, GL_LINK_STATUS, &success);
  if (!success) ERR_EXIT(-1, "Error while linking shader");

  for (int i = 0; i < 2; ++i)
    glDeleteShader(shaders[i]);

  fclose(shdr);
  return 0;
}

/*
 * JIN_shader_destory
 *
 * @desc
 * @param shader
 * @return
 */
int JIN_shader_destory(unsigned int *shader)
{
  glDeleteShader(*shader);

  return 0;
}


/* TEXTURE FUNCTIONS */

/*
 * JIN_texture_create
 *
 * @desc
 * @param texture
 * @param fpath
 * @return
 */
int JIN_texture_create(unsigned int *texture, const char *fpath)
{
  unsigned int   error;
  unsigned char *image;
  unsigned int   width, height;

  error = lodepng_decode32_file(&image, &width, &height, fpath);
  if (error) ERR_EXIT(-1, "Could not load image (%s)", fpath);

  glGenTextures(1, texture);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, *texture);
  /* I prefer GL_REPEAT but RECTANGLE can't have that */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
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

/* MODEL FUNCTIONS */

/*
 * JIN_model_create
 *
 * @desc
 * @param model
 * @param fpath
 * @return
 */
int JIN_model_create(struct JIN_Model *model, const char *fpath)
{
  FILE        *file;
  float       *vertices;
  int32_t      count;

  if (!(file = fopen(fpath, "rb"))) ERR_EXIT(-1, "Could not open .mdld file: %s", fpath);

  if (fread(&count, sizeof(int32_t), 1, file) != 1) ERR_EXIT(-1, "Could not read from file");
  if (!(vertices = malloc(count * sizeof(int32_t)))) ERR_EXIT(-1, "Out of memory");
  if (fread(vertices, sizeof(float), count, file) != count) ERR_EXIT(-1, "Could not read from file");

  glGenVertexArrays(1, &model->vao);
  glGenBuffers(1, &model->vbo);

  glBindVertexArray(model->vao);
  glBindBuffer(GL_ARRAY_BUFFER, model->vbo);

  glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), vertices, GL_STATIC_DRAW);

  if (fread(&count, sizeof(int32_t), 1, file) != 1) ERR_EXIT(-1, "Could not read from file");
  for (int32_t i = 0; i < count; ++i) {
    int32_t num;
    int32_t stride;
    int32_t offset;
  
    if (fread(&num,    sizeof(int32_t), 1, file) != 1) ERR_EXIT(-1, "Could not read from file");
    if (fread(&stride, sizeof(int32_t), 1, file) != 1) ERR_EXIT(-1, "Could not read from file");
    if (fread(&offset, sizeof(int32_t), 1, file) != 1) ERR_EXIT(-1, "Could not read from file");

    glVertexAttribPointer(i, num, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void *) (offset * sizeof(float)));
    glEnableVertexAttribArray(i);
  }

  /* Figure out how to do cleanups, maybe make this a model struct with vao and vbo */
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  free(vertices);
  fclose(file);

  return 0;
}

/*
 * JIN_model_destory
 *
 * @desc
 * @param model
 * @return
 */
int JIN_model_destory(struct JIN_Model *model)
{
  glDeleteBuffers(1, &model->vbo);
  glDeleteVertexArrays(1, &model->vao);
  
  return 0;
}
