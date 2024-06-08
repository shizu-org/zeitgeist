#if !defined(COMMONS_H_INCLUDED)
#define COMMONS_H_INCLUDED

#include "Shizu/Runtime/Include.h"

#define Shizu_VirtualCall(TYPE, METHOD, ...) \
  TYPE##_Dispatch* dispatch = (TYPE##_Dispatch*)Shizu_State_getObjectDispatch(state, (Shizu_Object*)self); \
  if (!dispatch) { \
    fprintf(stderr, "%s:%d: fatal error (unreachable code reached): dispatch not created\n", __FILE__, __LINE__); \
    exit(EXIT_FAILURE); \
  } \
  if (!dispatch->METHOD) { \
    fprintf(stderr, "%s:%d: fatal error (unreachable code reached): pure virtual method call\n", __FILE__, __LINE__); \
    exit(EXIT_FAILURE); \
  } \
  dispatch->METHOD(state, __VA_ARGS__);

#define Shizu_VirtualCallWithReturn(TYPE, METHOD, ...) \
  TYPE##_Dispatch* dispatch = (TYPE##_Dispatch*)Shizu_State_getObjectDispatch(state, (Shizu_Object*)self); \
  if (!dispatch) { \
    fprintf(stderr, "%s:%d: fatal error (unreachable code reached): dispatch not created\n", __FILE__, __LINE__); \
    exit(EXIT_FAILURE); \
  } \
  if (!dispatch->METHOD) { \
    fprintf(stderr, "%s:%d: fatal error (unreachable code reached): pure virtual method call\n", __FILE__, __LINE__); \
    exit(EXIT_FAILURE); \
  } \
  return dispatch->METHOD(state, __VA_ARGS__);

#endif // COMMONS_H_INCLUDED