#if !defined(VISUALS_OBJECT_H_INCLUDED)
#define VISUALS_OBJECT_H_INCLUDED

#include "ServiceGl.h"

Shizu_declareType(Visuals_Object);

struct Visuals_Object_Dispatch {
  Shizu_Object_Dispatch _parent;
  void (*materialize)(Shizu_State* state, Visuals_Object*);
  void (*unmaterialize)(Shizu_State* state, Visuals_Object*);
};

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
 * Notify a Visuals.Object before the shutdown of the visuals backend.
 * The Visuals.Object must relinquish all its native resources.
 * The default implementation calls Visuals_Object_unmaterialize.
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

static inline void
Visuals_Object_materialize
  (
    Shizu_State* state,
    Visuals_Object* self
  )
{
  Visuals_Object_Dispatch* dispatch = (Visuals_Object_Dispatch*)Shizu_State_getObjectDispatch(state, (Shizu_Object*)self);
  if (!dispatch) {
    fprintf(stderr, "%s:%d: fatal error (unreachable code reached): dispatch not created\n", __FILE__, __LINE__);
    exit(EXIT_FAILURE);
  }
  if (dispatch->materialize) {
    dispatch->materialize(state, self);
  }
}

static inline void
Visuals_Object_unmaterialize
  (
    Shizu_State* state,
    Visuals_Object* self
  )
{
  Visuals_Object_Dispatch* dispatch = (Visuals_Object_Dispatch*)Shizu_State_getObjectDispatch(state, (Shizu_Object*)self);
  if (!dispatch) {
    fprintf(stderr, "%s:%d: fatal error (unreachable code reached): dispatch not created\n", __FILE__, __LINE__);
    exit(EXIT_FAILURE);
  }
  if (dispatch->unmaterialize) {
    dispatch->unmaterialize(state, self);
  }
}

#endif // VISUALS_OIBJECT_H_INCLUDED
