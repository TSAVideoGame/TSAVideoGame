#ifndef JIN_INPUT_H
#define JIN_INPUT_H

/*
 * INPUT
 * 
 * All input information
 */

struct JIN_Keys {
  int f1;
  int f2;
  int f3;
  int f4;
  int a;
  int d;
  int s;
  int w;
};

struct JIN_Input {
  int quit;
  struct JIN_Keys keys;
};

#define JIN_INPUT_INIT(input) \
  input.quit = 0; \
  input.keys.f1 = 0; \
  input.keys.f2 = 0; \
  input.keys.f3 = 0; \
  input.keys.f4 = 0; \
  input.keys.a  = 0; \
  input.keys.d  = 0; \
  input.keys.s  = 0; \
  input.keys.w  = 0; \

/*
 * JIN_input_loop
 *
 * @desc
 *   Continue getting inputs until
 *   a 'quit' input
 * @return
 */
int JIN_input_loop(void);

#endif
