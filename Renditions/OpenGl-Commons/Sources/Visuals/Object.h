#if !defined(VISUALS_OBJECT_H_INCLUDED)
#define VISUALS_OBJECT_H_INCLUDED

#include "ServiceGl.h"

extern Shizu_TypeDescriptor const Visuals_Object_Type;
Shizu_declareDlType(Visuals_Object);
struct Visuals_Object {
  Shizu_Object _parent;
  void (*notifyVisualsShutdown)(Shizu_State*, Visuals_Object*);
};

void
Visuals_Object_construct
  (
    Shizu_State* state,
    Visuals_Object* self
  );

/**
 * @since 1.0
 * Notify a Visuals.Object on the shutdown of the visuals backend.
 * The Visuals.Object must destroy all its native resources.
 */
static inline void
Visuals_Object_notifyVisualsShutdown
  (
    Shizu_State *state,
    Visuals_Object* self
  )
{
  if (self->notifyVisualsShutdown) {
    self->notifyVisualsShutdown(state, self);
  }
}

#endif // VISUALS_OIBJECT_H_INCLUDED
