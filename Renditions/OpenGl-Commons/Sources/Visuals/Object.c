#include "Visuals/Object.h"

static void
dispatchInitialize
  (
    Shizu_State* state,
    Visuals_Object_Dispatch* self
  );

static Shizu_TypeDescriptor const Visuals_Object_Type = {
  .staticInitialize = NULL,
  .staticFinalize = NULL,
  .staticVisit = NULL,
  .size = sizeof(Visuals_Object),
  .finalize = NULL,
  .visit = NULL,
  .dispatchSize = sizeof(Visuals_Object_Dispatch),
  .dispatchInitialize = NULL,
  .dispatchUninitialize = NULL,
};

Shizu_defineType(Visuals_Object, Shizu_Object);

static void
dispatchInitialize
  (
    Shizu_State* state,
    Visuals_Object_Dispatch* self
  )
{
}

void
Visuals_Object_construct
  (
    Shizu_State* state,
    Visuals_Object* self
  )
{
  Shizu_Type* type = Visuals_Object_getType(state);
  self->notifyVisualsShutdown = NULL;
  SeviceGl_registerVisualsObject(state, self);
  ((Shizu_Object*)self)->type = type;
}
