#ifndef JEL_COMPONENT_MAGIC_H
#define JEL_COMPONENT_MAGIC_H

#define NTH_ARG_16( \
    _01, _02, _03, _04, \
    _05, _06, _07, _08, \
    _09, _10, _11, _12, \
    _13, _14, _15, _16, \
    N, ...) N

#define ARG_COUNT_16(...) NTH_ARG_16(__VA_ARGS__, \
    16, 15, 14, 13, \
    12, 11, 10,  9, \
     8,  7,  6,  5, \
     4,  3,  2,  1)

#define JEL_COMPONENT_FN_1(fn, component, arg) \
  fn(component, arg)

#define JEL_COMPONENT_FN_2(fn, component, arg, ...) \
  fn(component, arg) \
  JEL_COMPONENT_FN_1(fn, component, __VA_ARGS__)

#define JEL_COMPONENT_FN_3(fn, component, arg, ...) \
  fn(component, arg) \
  JEL_COMPONENT_FN_2(fn, component,  __VA_ARGS__)

#define JEL_COMPONENT_FN_4(fn, component, arg, ...) \
  fn(component, arg) \
  JEL_COMPONENT_FN_3(fn, component, __VA_ARGS__)

#define JEL_COMPONENT_FN_5(fn, component, arg, ...) \
  fn(component, arg) \
  JEL_COMPONENT_FN_4(fn, component, __VA_ARGS__)

#define JEL_COMPONENT_FN_6(fn, component, arg, ...) \
  fn(component, arg) \
  JEL_COMPONENT_FN_5(fn, component, __VA_ARGS__)

#define JEL_COMPONENT_FN_7(fn, component, arg, ...) \
  fn(component, arg) \
  JEL_COMPONENT_FN_6(fn, component, __VA_ARGS__)

#define JEL_COMPONENT_FN_8(fn, component, arg, ...) \
  fn(component, arg) \
  JEL_COMPONENT_FN_7(fn, component, __VA_ARGS__)

#define JEL_COMPONENT_FN_9(fn, component, arg, ...) \
  fn(component, arg) \
  JEL_COMPONENT_FN_8(fn, component, __VA_ARGS__)

#define JEL_COMPONENT_FN_10(fn, component, arg, ...) \
  fn(component, arg) \
  JEL_COMPONENT_FN_9(fn, component, __VA_ARGS__)

#define JEL_COMPONENT_FN_11(fn, component, arg, ...) \
  fn(component, arg) \
  JEL_COMPONENT_FN_10(fn, component, __VA_ARGS__)

#define JEL_COMPONENT_FN_12(fn, component, arg, ...) \
  fn(component, arg) \
  JEL_COMPONENT_FN_11(fn, component, __VA_ARGS__)

#define JEL_COMPONENT_FN_13(fn, component, arg, ...) \
  fn(component, arg) \
  JEL_COMPONENT_FN_12(fn, component, __VA_ARGS__)

#define JEL_COMPONENT_FN_14(fn, component, arg, ...) \
  fn(component, arg) \
  JEL_COMPONENT_FN_13(fn, component, __VA_ARGS__)

#define JEL_COMPONENT_FN_15(fn, component, arg, ...) \
  fn(component, arg) \
  JEL_COMPONENT_FN_14(fn, component, __VA_ARGS__)

#define JEL_COMPONENT_FN_16(fn, component, arg, ...) \
  fn(component, arg) \
  JEL_COMPONENT_FN_15(fn, component, __VA_ARGS__)

#define JEL_COMPONENT_ITERATE(fn, component, ...) \
  NTH_ARG_16(__VA_ARGS__, \
      JEL_COMPONENT_FN_16, \
      JEL_COMPONENT_FN_15, \
      JEL_COMPONENT_FN_14, \
      JEL_COMPONENT_FN_13, \
      JEL_COMPONENT_FN_12, \
      JEL_COMPONENT_FN_11, \
      JEL_COMPONENT_FN_10, \
      JEL_COMPONENT_FN_9, \
      JEL_COMPONENT_FN_8, \
      JEL_COMPONENT_FN_7, \
      JEL_COMPONENT_FN_6, \
      JEL_COMPONENT_FN_5, \
      JEL_COMPONENT_FN_4, \
      JEL_COMPONENT_FN_3, \
      JEL_COMPONENT_FN_2, \
      JEL_COMPONENT_FN_1) \
      (fn, component, __VA_ARGS__)

#endif
