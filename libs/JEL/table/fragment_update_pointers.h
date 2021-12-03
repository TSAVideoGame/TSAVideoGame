#ifndef JEL_FRAGMENT_UPDATE_POINTERS_H
#define JEL_FRAGMENT_UPDATE_POINTERS_H

/*
 * Table Fragment Update Pointers
 *
 * First you need to start where the buffer
 * starts
 *
 * Afterwards, you just loop through the
 * members and set stuff equal to the previous
 * member plus how many entries are being
 * allocated
 */

/*
 * Macro stuff
 *
 * This macro needs more than just type and name
 * so it is seperate from the main utility one
 *
 * These macros are for fragments
 */

/* Recursive Macros */
#define JEL_FRAGMENT_POINTERS_UPDATE_1_P(component, prev, type, name) \
  ((struct component##Fragment *) fragment)->name = (type *) (((struct component##Fragment *) fragment)->prev + allocated);

#define JEL_FRAGMENT_POINTERS_UPDATE_2_P(component, prev, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) (((struct component##Fragment *) fragment)->prev + allocated); \
  JEL_FRAGMENT_POINTERS_UPDATE_1_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_3_P(component, prev, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) (((struct component##Fragment *) fragment)->prev + allocated); \
  JEL_FRAGMENT_POINTERS_UPDATE_2_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_4_P(component, prev, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) (((struct component##Fragment *) fragment)->prev + allocated); \
  JEL_FRAGMENT_POINTERS_UPDATE_3_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_5_P(component, prev, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) (((struct component##Fragment *) fragment)->prev + allocated); \
  JEL_FRAGMENT_POINTERS_UPDATE_4_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_6_P(component, prev, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) (((struct component##Fragment *) fragment)->prev + allocated); \
  JEL_FRAGMENT_POINTERS_UPDATE_5_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_7_P(component, prev, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) (((struct component##Fragment *) fragment)->prev + allocated); \
  JEL_FRAGMENT_POINTERS_UPDATE_6_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_8_P(component, prev, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) (((struct component##Fragment *) fragment)->prev + allocated); \
  JEL_FRAGMENT_POINTERS_UPDATE_7_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_9_P(component, prev, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) (((struct component##Fragment *) fragment)->prev + allocated); \
  JEL_FRAGMENT_POINTERS_UPDATE_8_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_10_P(component, prev, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) (((struct component##Fragment *) fragment)->prev + allocated); \
  JEL_FRAGMENT_POINTERS_UPDATE_9_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_11_P(component, prev, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) (((struct component##Fragment *) fragment)->prev + allocated); \
  JEL_FRAGMENT_POINTERS_UPDATE_10_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_12_P(component, prev, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) (((struct component##Fragment *) fragment)->prev + allocated); \
  JEL_FRAGMENT_POINTERS_UPDATE_11_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_13_P(component, prev, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) (((struct component##Fragment *) fragment)->prev + allocated); \
  JEL_FRAGMENT_POINTERS_UPDATE_12_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_14_P(component, prev, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) (((struct component##Fragment *) fragment)->prev + allocated); \
  JEL_FRAGMENT_POINTERS_UPDATE_13_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_15_P(component, prev, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) (((struct component##Fragment *) fragment)->prev + allocated); \
  JEL_FRAGMENT_POINTERS_UPDATE_14_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_16_P(component, prev, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) (((struct component##Fragment *) fragment)->prev + allocated); \
  JEL_FRAGMENT_POINTERS_UPDATE_15_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_17_P(component, prev, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) (((struct component##Fragment *) fragment)->prev + allocated); \
  JEL_FRAGMENT_POINTERS_UPDATE_16_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_18_P(component, prev, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) (((struct component##Fragment *) fragment)->prev + allocated); \
  JEL_FRAGMENT_POINTERS_UPDATE_17_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_19_P(component, prev, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) (((struct component##Fragment *) fragment)->prev + allocated); \
  JEL_FRAGMENT_POINTERS_UPDATE_18_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_20_P(component, prev, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) (((struct component##Fragment *) fragment)->prev + allocated); \
  JEL_FRAGMENT_POINTERS_UPDATE_19_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_21_P(component, prev, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) (((struct component##Fragment *) fragment)->prev + allocated); \
  JEL_FRAGMENT_POINTERS_UPDATE_20_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_22_P(component, prev, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) (((struct component##Fragment *) fragment)->prev + allocated); \
  JEL_FRAGMENT_POINTERS_UPDATE_21_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_23_P(component, prev, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) (((struct component##Fragment *) fragment)->prev + allocated); \
  JEL_FRAGMENT_POINTERS_UPDATE_22_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_24_P(component, prev, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) (((struct component##Fragment *) fragment)->prev + allocated); \
  JEL_FRAGMENT_POINTERS_UPDATE_23_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_25_P(component, prev, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) (((struct component##Fragment *) fragment)->prev + allocated); \
  JEL_FRAGMENT_POINTERS_UPDATE_24_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_26_P(component, prev, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) (((struct component##Fragment *) fragment)->prev + allocated); \
  JEL_FRAGMENT_POINTERS_UPDATE_25_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_27_P(component, prev, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) (((struct component##Fragment *) fragment)->prev + allocated); \
  JEL_FRAGMENT_POINTERS_UPDATE_26_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_28_P(component, prev, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) (((struct component##Fragment *) fragment)->prev + allocated); \
  JEL_FRAGMENT_POINTERS_UPDATE_27_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_29_P(component, prev, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) (((struct component##Fragment *) fragment)->prev + allocated); \
  JEL_FRAGMENT_POINTERS_UPDATE_28_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_30_P(component, prev, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) (((struct component##Fragment *) fragment)->prev + allocated); \
  JEL_FRAGMENT_POINTERS_UPDATE_29_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_31_P(component, prev, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) (((struct component##Fragment *) fragment)->prev + allocated); \
  JEL_FRAGMENT_POINTERS_UPDATE_30_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_32_P(component, prev, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) (((struct component##Fragment *) fragment)->prev + allocated); \
  JEL_FRAGMENT_POINTERS_UPDATE_31_P(component, name, __VA_ARGS__)

/* Start Macros */
#define JEL_FRAGMENT_POINTERS_UPDATE_START_1_P(component, type, name) \
  ((struct component##Fragment *) fragment)->name = (type *) ((struct component##Fragment *) fragment)->head.buffer_start;

#define JEL_FRAGMENT_POINTERS_UPDATE_START_2_P(component, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) ((struct component##Fragment *) fragment)->head.buffer_start; \
  JEL_FRAGMENT_POINTERS_UPDATE_1_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_START_3_P(component, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) ((struct component##Fragment *) fragment)->head.buffer_start; \
  JEL_FRAGMENT_POINTERS_UPDATE_2_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_START_4_P(component, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) ((struct component##Fragment *) fragment)->head.buffer_start; \
  JEL_FRAGMENT_POINTERS_UPDATE_3_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_START_5_P(component, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) ((struct component##Fragment *) fragment)->head.buffer_start; \
  JEL_FRAGMENT_POINTERS_UPDATE_4_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_START_6_P(component, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) ((struct component##Fragment *) fragment)->head.buffer_start; \
  JEL_FRAGMENT_POINTERS_UPDATE_5_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_START_7_P(component, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) ((struct component##Fragment *) fragment)->head.buffer_start; \
  JEL_FRAGMENT_POINTERS_UPDATE_6_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_START_8_P(component, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) ((struct component##Fragment *) fragment)->head.buffer_start; \
  JEL_FRAGMENT_POINTERS_UPDATE_7_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_START_9_P(component, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) ((struct component##Fragment *) fragment)->head.buffer_start; \
  JEL_FRAGMENT_POINTERS_UPDATE_8_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_START_10_P(component, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) ((struct component##Fragment *) fragment)->head.buffer_start; \
  JEL_FRAGMENT_POINTERS_UPDATE_9_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_START_11_P(component, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) ((struct component##Fragment *) fragment)->head.buffer_start; \
  JEL_FRAGMENT_POINTERS_UPDATE_10_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_START_12_P(component, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) ((struct component##Fragment *) fragment)->head.buffer_start; \
  JEL_FRAGMENT_POINTERS_UPDATE_11_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_START_13_P(component, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) ((struct component##Fragment *) fragment)->head.buffer_start; \
  JEL_FRAGMENT_POINTERS_UPDATE_12_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_START_14_P(component, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) ((struct component##Fragment *) fragment)->head.buffer_start; \
  JEL_FRAGMENT_POINTERS_UPDATE_13_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_START_15_P(component, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) ((struct component##Fragment *) fragment)->head.buffer_start; \
  JEL_FRAGMENT_POINTERS_UPDATE_14_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_START_16_P(component, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) ((struct component##Fragment *) fragment)->head.buffer_start; \
  JEL_FRAGMENT_POINTERS_UPDATE_15_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_START_17_P(component, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) ((struct component##Fragment *) fragment)->head.buffer_start; \
  JEL_FRAGMENT_POINTERS_UPDATE_16_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_START_18_P(component, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) ((struct component##Fragment *) fragment)->head.buffer_start; \
  JEL_FRAGMENT_POINTERS_UPDATE_17_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_START_19_P(component, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) ((struct component##Fragment *) fragment)->head.buffer_start; \
  JEL_FRAGMENT_POINTERS_UPDATE_18_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_START_20_P(component, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) ((struct component##Fragment *) fragment)->head.buffer_start; \
  JEL_FRAGMENT_POINTERS_UPDATE_19_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_START_21_P(component, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) ((struct component##Fragment *) fragment)->head.buffer_start; \
  JEL_FRAGMENT_POINTERS_UPDATE_20_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_START_22_P(component, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) ((struct component##Fragment *) fragment)->head.buffer_start; \
  JEL_FRAGMENT_POINTERS_UPDATE_21_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_START_23_P(component, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) ((struct component##Fragment *) fragment)->head.buffer_start; \
  JEL_FRAGMENT_POINTERS_UPDATE_22_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_START_24_P(component, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) ((struct component##Fragment *) fragment)->head.buffer_start; \
  JEL_FRAGMENT_POINTERS_UPDATE_23_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_START_25_P(component, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) ((struct component##Fragment *) fragment)->head.buffer_start; \
  JEL_FRAGMENT_POINTERS_UPDATE_24_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_START_26_P(component, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) ((struct component##Fragment *) fragment)->head.buffer_start; \
  JEL_FRAGMENT_POINTERS_UPDATE_25_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_START_27_P(component, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) ((struct component##Fragment *) fragment)->head.buffer_start; \
  JEL_FRAGMENT_POINTERS_UPDATE_26_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_START_28_P(component, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) ((struct component##Fragment *) fragment)->head.buffer_start; \
  JEL_FRAGMENT_POINTERS_UPDATE_27_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_START_29_P(component, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) ((struct component##Fragment *) fragment)->head.buffer_start; \
  JEL_FRAGMENT_POINTERS_UPDATE_28_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_START_30_P(component, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) ((struct component##Fragment *) fragment)->head.buffer_start; \
  JEL_FRAGMENT_POINTERS_UPDATE_29_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_START_31_P(component, type, name, ...) \
  ((struct component##Fragment *) fragment)->name = (type *) ((struct component##Fragment *) fragment)->head.buffer_start; \
  JEL_FRAGMENT_POINTERS_UPDATE_30_P(component, name, __VA_ARGS__)

#define JEL_FRAGMENT_POINTERS_UPDATE_P(component, ...) \
  JEL_COMPONENT_MEMBERS_FUNCTION_GET_P(__VA_ARGS__, \
  dummy, JEL_FRAGMENT_POINTERS_UPDATE_START_31_P, \
  dummy, JEL_FRAGMENT_POINTERS_UPDATE_START_30_P, \
  dummy, JEL_FRAGMENT_POINTERS_UPDATE_START_29_P, \
  dummy, JEL_FRAGMENT_POINTERS_UPDATE_START_28_P, \
  dummy, JEL_FRAGMENT_POINTERS_UPDATE_START_27_P, \
  dummy, JEL_FRAGMENT_POINTERS_UPDATE_START_26_P, \
  dummy, JEL_FRAGMENT_POINTERS_UPDATE_START_25_P, \
  dummy, JEL_FRAGMENT_POINTERS_UPDATE_START_24_P, \
  dummy, JEL_FRAGMENT_POINTERS_UPDATE_START_23_P, \
  dummy, JEL_FRAGMENT_POINTERS_UPDATE_START_22_P, \
  dummy, JEL_FRAGMENT_POINTERS_UPDATE_START_21_P, \
  dummy, JEL_FRAGMENT_POINTERS_UPDATE_START_20_P, \
  dummy, JEL_FRAGMENT_POINTERS_UPDATE_START_19_P, \
  dummy, JEL_FRAGMENT_POINTERS_UPDATE_START_18_P, \
  dummy, JEL_FRAGMENT_POINTERS_UPDATE_START_17_P, \
  dummy, JEL_FRAGMENT_POINTERS_UPDATE_START_16_P, \
  dummy, JEL_FRAGMENT_POINTERS_UPDATE_START_15_P, \
  dummy, JEL_FRAGMENT_POINTERS_UPDATE_START_14_P, \
  dummy, JEL_FRAGMENT_POINTERS_UPDATE_START_13_P, \
  dummy, JEL_FRAGMENT_POINTERS_UPDATE_START_12_P, \
  dummy, JEL_FRAGMENT_POINTERS_UPDATE_START_11_P, \
  dummy, JEL_FRAGMENT_POINTERS_UPDATE_START_10_P, \
  dummy, JEL_FRAGMENT_POINTERS_UPDATE_START_9_P, \
  dummy, JEL_FRAGMENT_POINTERS_UPDATE_START_8_P, \
  dummy, JEL_FRAGMENT_POINTERS_UPDATE_START_7_P, \
  dummy, JEL_FRAGMENT_POINTERS_UPDATE_START_6_P, \
  dummy, JEL_FRAGMENT_POINTERS_UPDATE_START_5_P, \
  dummy, JEL_FRAGMENT_POINTERS_UPDATE_START_4_P, \
  dummy, JEL_FRAGMENT_POINTERS_UPDATE_START_3_P, \
  dummy, JEL_FRAGMENT_POINTERS_UPDATE_START_2_P, \
  dummy, JEL_FRAGMENT_POINTERS_UPDATE_START_1_P)(component, __VA_ARGS__)

#endif
