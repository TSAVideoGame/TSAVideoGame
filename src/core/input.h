#ifndef JIN_INPUT_H
#define JIN_INPUT_H

#include <GLFW/glfw3.h>

/*
 * INPUT
 *
 * Get input from keys and the mouse.
 */

#define JIN_INPUT_KEYS_NUM 28

struct JIN_InputKey {
  int key;      /* What key this is */
  int duration; /* How many frames it was pressed */
};

struct JIN_Input {
  struct JIN_InputKey keys[JIN_INPUT_KEYS_NUM];
};

enum JIN_InputKeys {
  JIN_KEY_SPACE,
  JIN_KEY_0,
  JIN_KEY_1,
  JIN_KEY_2,
  JIN_KEY_3,
  JIN_KEY_4,
  JIN_KEY_5,
  JIN_KEY_6,
  JIN_KEY_7,
  JIN_KEY_8,
  JIN_KEY_9,
  JIN_KEY_W,
  JIN_KEY_A,
  JIN_KEY_S,
  JIN_KEY_D,
  JIN_KEY_ESCAPE,
  JIN_KEY_RIGHT,
  JIN_KEY_LEFT,
  JIN_KEY_DOWN,
  JIN_KEY_UP,
  JIN_KEY_F1,
  JIN_KEY_F2,
  JIN_KEY_F3,
  JIN_KEY_F4,
  JIN_KEY_F5,
  JIN_KEY_F6,
  JIN_KEY_F7,
  JIN_KEY_F8
};

#define JIN_INPUT_INIT { \
  { \
    {GLFW_KEY_SPACE,  0}, \
    {GLFW_KEY_0,      0}, \
    {GLFW_KEY_1,      0}, \
    {GLFW_KEY_2,      0}, \
    {GLFW_KEY_3,      0}, \
    {GLFW_KEY_4,      0}, \
    {GLFW_KEY_5,      0}, \
    {GLFW_KEY_6,      0}, \
    {GLFW_KEY_7,      0}, \
    {GLFW_KEY_8,      0}, \
    {GLFW_KEY_9,      0}, \
    {GLFW_KEY_A,      0}, \
    {GLFW_KEY_D,      0}, \
    {GLFW_KEY_S,      0}, \
    {GLFW_KEY_W,      0}, \
    {GLFW_KEY_ESCAPE, 0}, \
    {GLFW_KEY_RIGHT,  0}, \
    {GLFW_KEY_LEFT,   0}, \
    {GLFW_KEY_DOWN,   0}, \
    {GLFW_KEY_UP,     0}, \
    {GLFW_KEY_F1,     0}, \
    {GLFW_KEY_F2,     0}, \
    {GLFW_KEY_F3,     0}, \
    {GLFW_KEY_F4,     0}, \
    {GLFW_KEY_F5,     0}, \
    {GLFW_KEY_F6,     0}, \
    {GLFW_KEY_F7,     0}, \
    {GLFW_KEY_F8,     0}, \
  } \
};

#endif
