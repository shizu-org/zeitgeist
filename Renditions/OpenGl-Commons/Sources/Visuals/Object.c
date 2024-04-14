#include "Visuals/Object.h"

Shizu_TypeDescriptor const Visuals_Object_Type = {
  .staticInitialize = NULL,
  .staticFinalize = NULL,
  .staticVisit = NULL,
  .finalize = NULL,
  .visit = NULL,
};

Shizu_defineDlType(Visuals_Object, Shizu_Object);

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
