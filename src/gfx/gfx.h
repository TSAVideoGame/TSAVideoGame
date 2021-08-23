#ifndef JIN_GFX_H
#define JIN_GFX_H

/*
 * GRAPHICS
 *
 * Draw stuff on a window
 *
 * TYPES:
 * struct JIN_Model | An OpenGL vertex array and vertex buffer
 */
struct JIN_Model {
  unsigned int vao;
  unsigned int vbo;
};

/*
 * GFX functions
 */
int JIN_gfx_draw_sprite(unsigned int *shader, unsigned int *texture, int x, int y, int w, int h, int t_x, int t_y, int t_w, int t_h);

/*
 * Shader functions
 *
 * create  | Create a shader (program)
 * destroy | Destroy a shader (program)
 */
int JIN_shader_create (unsigned int *shader, const char *fpath);
int JIN_shader_destory(unsigned int *shader);

/*
 * Texture functions
 */
int JIN_texture_create (unsigned int *texture, const char *fpath);
int JIN_texture_destroy(unsigned int *texture);

/*
 * MODEL functions
 *
 * create  | Create a shader (program)
 * destroy | Destroy a shader (program)
 */
int JIN_model_create (struct JIN_Model *model, const char *fpath);
int JIN_model_destory(struct JIN_Model *model);

#endif
