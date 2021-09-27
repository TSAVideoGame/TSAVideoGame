#include "states.h"
#include "resm/resm.h"
#include "core/core.h"
#include "snd/snd.h"
#include "gfx/gfx.h"
#include "glew/glew.h"
#include "cglm/cglm.h"
#include "anim/anim.h"
#include <JEL/jel.h>

static JEL_Entity player; /* Don't do this, create a state variable */

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";
#include "core/window/window.h"
    extern struct JIN_Window *root;
unsigned int vao;
static int test_fn_create(struct STM_State *state)
{
  //JIN_sndbgm_play();

  /*
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("Shader compilation failed: %s\n", infoLog);
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("Shader compilation failed: %s\n", infoLog);
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("Shader linking failed: %s\n", infoLog);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
*/
  JIN_resm_add(&JIN_resm, "triangle_shader", "res/shaders/test.shdr", JIN_RES_SHADER);

float vertices[] = {
         0.0f,  0.5f, 0.0f,  // top
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
    };
    unsigned int VBO;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 

        // draw our first triangle
        glUseProgram(* (unsigned int *) JIN_resm_get(&JIN_resm, "triangle_shader"));
        glBindVertexArray(vao); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawArrays(GL_TRIANGLES, 0, 3);

        JIN_window_buffer_swap(root);


  /*JIN_resm_add(&JIN_resm, "sprite_shader", "res/shaders/sprite.shdr", JIN_RES_SHADER);
  unsigned int *shader = JIN_resm_get(&JIN_resm, "sprite_shader");

  glUseProgram(*shader);

  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  mat4 projection;
  glm_ortho(0.0f, (float) WINDOW_WIDTH, (float) WINDOW_HEIGHT, 0.0f, -1.0f, 1.0f, projection);
  glUniformMatrix4fv(glGetUniformLocation(*shader, "projection"), 1, GL_FALSE, (float *) projection);

  JIN_resm_add(&JIN_resm, "test_image", "res/images/test_image.png", JIN_RES_PNG);

  JIN_resm_add(&JIN_resm, "test_shader", "res/shaders/test.shdr", JIN_RES_SHADER);
   3d fun */
  /*JIN_resm_add(&JIN_resm, "3d_shader", "res/shaders/3d.shdr", JIN_RES_SHADER);
  JIN_resm_add(&JIN_resm, "3d_spaceship", "res/models/space_ship.mdld", JIN_RES_MODEL);
*/
  /* Animation test */
  /*JIN_resm_add(&JIN_resm, "player_animation", "res/animations/player.animd", JIN_RES_ANIM);
  JIN_resm_add(&JIN_resm, "player_img", "res/images/dodger.png", JIN_RES_PNG);

  JEL_COMPONENT_REGISTER(Sprite);

  player = JEL_entity_create();
  JEL_ENTITY_ADD(player, Sprite);
  JEL_ENTITY_SET(player, Sprite, animd, JIN_resm_get(&JIN_resm, "player_animation"));
  JEL_ENTITY_SET(player, Sprite, anim, 0);
  JEL_ENTITY_SET(player, Sprite, frame, 0);
  JEL_ENTITY_SET(player, Sprite, ticks, 0);*/

  return 0;
}

static int test_fn_destroy(struct STM_State *state)
{
  return 0;
}

static int test_fn_update(struct STM_State *state)
{
  //JIN_anim_update();
  return 0;
}

static int ticks = 0;
static int test_fn_draw(struct STM_State *state)
{
  /* glClear works but nothing else */
  //glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
  //glClear(GL_COLOR_BUFFER_BIT);
  
        glUseProgram(* (unsigned int *) JIN_resm_get(&JIN_resm, "triangle_shader"));
        glBindVertexArray(vao); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, 3);
  //unsigned int *shader;
  //unsigned int *texture;

  /*
  shader = JIN_resm_get(&JIN_resm, "sprite_shader");
  texture = JIN_resm_get(&JIN_resm, "test_image");
  JIN_gfx_draw_sprite(shader, texture, 16, 16, 256, 256, 0, 0, 256, 256);
  JIN_gfx_draw_sprite(shader, texture, 16, 256, 256, 256, 32, 32, 224, 224);

  JIN_anim_draw();
*/
  /* Try and draw a triangle */
  //shader = JIN_resm_get(&JIN_resm, "test_shader");
  /* 3d fun */
  /*shader = JIN_resm_get(&JIN_resm, "3d_shader");
  glUseProgram(*shader);
  vec3 vec = {0.0f, 0.0f, 0.0f};
  mat4 model = GLM_MAT4_IDENTITY_INIT;
  vec[1] = 1.0f;
  glm_rotate(model, glm_rad((ticks += 30)) / 30, vec); 

  mat4 view = GLM_MAT4_IDENTITY_INIT;
  vec[0] = 4.0f;
  vec[1] = -4.0f;
  vec[2] = -20.0f;
  glm_translate(view, vec);

  mat4 projection = GLM_MAT4_IDENTITY_INIT;

  glm_perspective(glm_rad(45.0f), (float) WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f, projection);
*/
  /*glUniformMatrix4fv(glGetUniformLocation(*shader, "model"), 1, GL_FALSE, (const float *) model);
  glUniformMatrix4fv(glGetUniformLocation(*shader, "view"), 1, GL_FALSE, (const float *) view);
  glUniformMatrix4fv(glGetUniformLocation(*shader, "projection"), 1, GL_FALSE, (const float *) projection);

  glUniform3f(glGetUniformLocation(*shader, "light_color"), 1.0f, 1.0f, 1.0f);
  glUniform3f(glGetUniformLocation(*shader, "light_pos"), 4.0f, 0.0f, -40.0f);

  glBindVertexArray(((struct JIN_Model *) JIN_resm_get(&JIN_resm, "3d_spaceship"))->vao);
  glDrawArrays(GL_TRIANGLES, 0, 78);
*/
  return 0;
}

int JIN_states_test_create(struct STM_State *state)
{
  if (STM_state_create(state, "test", 0, 0, test_fn_create, test_fn_destroy, test_fn_update, test_fn_draw)) return -1;

  return 0;
}

