#if !defined(VISUALS_GL_CONTEXT_H_INCLUDED)
#define VISUALS_GL_CONTEXT_H_INCLUDED

#include "Visuals/Context.h"

/// @since 1.0
/// @brief The implementation of Visuals_Context for OpenGL.
Shizu_declareObjectType(Visuals_Gl_Context);

struct Visuals_Gl_Context_Dispatch {
  Visuals_Context_Dispatch _parent;
};

struct Visuals_Gl_Context {
  Visuals_Context _parent;
  struct {
    Shizu_Float32 left;
    Shizu_Float32 bottom;
    Shizu_Float32 width;
    Shizu_Float32 height;
  } viewport;
};

void
Visuals_Gl_Context_construct
  (
    Shizu_State2* state,
    Visuals_Gl_Context* self
  );

Visuals_Gl_Context*
Visuals_Gl_Context_create
  (
    Shizu_State2* state
  );

#endif // VISUALS_GL_CONTEXT_H_INCLUDED
