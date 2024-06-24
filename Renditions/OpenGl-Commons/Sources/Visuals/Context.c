#include "Visuals/Context.h"

static void
Visuals_Context_finalize
  (
    Shizu_State2* state,
    Visuals_Context* self
  );

static void
Visuals_Context_dispatchInitialize
  (
    Shizu_State1* state1,
    Visuals_Context_Dispatch* self
  );

static Shizu_TypeDescriptor const Visuals_Context_Type = {
  .postCreateType = NULL,
  .preDestroyType = NULL,
  .visitType = NULL,
  .size = sizeof(Visuals_Context),
  .finalize = (Shizu_OnFinalizeCallback*)&Visuals_Context_finalize,
  .visit = NULL,
  .dispatchSize = sizeof(Visuals_Context_Dispatch),
  .dispatchInitialize = (Shizu_OnDispatchInitializeCallback*) & Visuals_Context_dispatchInitialize,
  .dispatchUninitialize = NULL,
};

Shizu_defineType(Visuals_Context, Shizu_Object);

static void
Visuals_Context_finalize
  (
    Shizu_State2* state,
    Visuals_Context* self
  )
{/*Intentionally empty.*/}

static void
Visuals_Context_dispatchInitialize
  (
    Shizu_State1* state1,
    Visuals_Context_Dispatch* self
  )
{/*Intentionally empty.*/}

void
Visuals_Context_construct
  (
    Shizu_State2* state,
    Visuals_Context* self
  )
{
  Shizu_Type* TYPE = Visuals_Context_getType(state);
  Shizu_Object_construct(state, (Shizu_Object*)self);
  ((Shizu_Object*)self)->type = TYPE;
}
