#include "stm.h"
#include <string.h>
#include "core/logger/logger.h"

/*
 * STM_state_create
 *
 * @desc
 *   Creates a state
 */
int STM_state_create(struct STM_State *state, const char *name, size_t stack_size, uint8_t flags, STM_create fn_create, STM_destroy fn_destroy, STM_update fn_update, STM_draw fn_draw)
{
  state->count = stack_size;
  if (!(state->data  = malloc(stack_size * sizeof(STM_Data)))) ERR_EXIT(-1, "Out of memory");
  for (int i = 0; i < 16; ++i) {
    state->name[i] = *name++;
    if (!*name) break;
  }
  state->flags      = flags;
  state->fn_create  = fn_create;
  state->fn_destroy = fn_destroy;
  state->fn_update  = fn_update;
  state->fn_draw    = fn_draw;

  return 0;
}

/*
 * STM_state_destroy
 *
 * @desc
 *   Destroys a state
 */
int STM_state_destroy(struct STM_State *state)
{
  free(state->data);

  return 0;
}

#define GROWTH_FACTOR 2
#define INITIAL_COUNT 8

/*
 * STATE_FN
 *
 * @desc
 *   Calls a state's function
 * @param index
 *   Index of the state from the top
 *   of the stack
 * @param name
 *   Name of the function
 */
#define STATE_FN(index, name) (stack->states[stack->count - (1 + index)]->fn_##name(stack->states[stack->count - (1 + index)]))

/*
 * STM_stack_allocate
 *
 * @desc
 *   Allocates space for more states
 * @param stack
 * @param count
 *   How many states to allocate
 * @return
 *   Success
 */
static int STM_stack_allocate(struct STM_Stack *stack, size_t count)
{
  if (count <= stack->allocated) ERR_EXIT(-1, "Stack already has enough space");

  struct STM_State **states_new;

  if (!(states_new = malloc(count * sizeof(struct STM_State)))) ERR_EXIT(-1, "Out of memory");

  memcpy(states_new, stack->states, stack->count * sizeof(struct STM_State *));
  free(stack->states);
  stack->states = states_new;

  stack->allocated = count;

  return 0;
}

/*
 * STM_stack_create
 *
 * @desc
 *   Create a stack
 * @param stack
 * @return success
 */
int STM_stack_create(struct STM_Stack *stack)
{
  if (!(stack->states = malloc(INITIAL_COUNT * sizeof(struct STM_State *)))) ERR_EXIT(-1, "Out of memory");
  stack->count = 0;
  stack->allocated = INITIAL_COUNT;

  return 0;
}

/*
 * STM_stack_destroy
 *
 * @desc
 *   Destroys a stack
 * @param stack
 * @return
 *   Success
 */
int STM_stack_destroy(struct STM_Stack *stack)
{
  for (size_t i = 0; i < stack->count; ++i) {
    STM_state_destroy(stack->states[i]);
  }
  free(stack->states);

  return 0;
}

/*
 * STM_stack_push
 *
 * @desc
 *   Push a state onto the stack
 * @param stack
 * @param state
 *   State to push
 * @return
 *   Success
 */
int STM_stack_push(struct STM_Stack *stack, struct STM_State *state)
{
  if (stack->allocated <= stack->count) {
    if (STM_stack_allocate(stack, stack->count * GROWTH_FACTOR)) ERR_EXIT(-1, "Could not allocate STM_stack");
  }

  stack->states[stack->count++] = state;
  STATE_FN(0, create);

  return 0;
}

/*
 * STM_stack_pop
 *
 * @desc
 *   Removes a state from the stack,
 *   like closing a pause screen state
 * @param stack
 * @return
 *   Success
 */
int STM_stack_pop(struct STM_Stack *stack)
{
  if (STATE_FN(0, destroy)) ERR_EXIT(-1, "Could not properly destroy state");
  --stack->count;

  return 0;
}

/*
 * STM_stack_update
 *
 * @desc
 *   Updates the top most state
 * @param stack
 * @return
 *   Success
 */
int STM_stack_update(struct STM_Stack *stack)
{
  if (STATE_FN(0, update)) ERR_EXIT(-1, "Could not properly update");

  return 0;
}

/*
 * STM_stack_draw
 *
 * @desc
 *   Draws the top most state, can also
 *   draw one state below with the flag,
 *   but doesn't check if there is a state
 *   below
 * @param stack
 * @return
 *   Success
 */
int STM_stack_draw(struct STM_Stack *stack)
{
  if (stack->states[stack->count - 1]->flags & STM_DRAW_PREV) {
    if (STATE_FN(1, draw)) ERR_EXIT(-1, "Could not draw previous state properly");
  }

  if (STATE_FN(0, draw)) ERR_EXIT(-1, "Could not draw properly");

  return 0;
}

/* JIN FUNCTIONS */
#include "../core/globals.h"
int JIN_state_push(struct STM_State *state)
{
  STM_stack_push(&JIN_states, state);

  return 0;
}
