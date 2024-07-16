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

static void
Visuals_Context_constructImpl
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  );

static Shizu_ObjectTypeDescriptor const Visuals_Context_Type = {
  .postCreateType = NULL,
  .preDestroyType = NULL,
  .visitType = NULL,
  .size = sizeof(Visuals_Context),
  .construct = &Visuals_Context_constructImpl,
  .finalize = (Shizu_OnFinalizeCallback*)&Visuals_Context_finalize,
  .visit = NULL,
  .dispatchSize = sizeof(Visuals_Context_Dispatch),
  .dispatchInitialize = (Shizu_OnDispatchInitializeCallback*) & Visuals_Context_dispatchInitialize,
  .dispatchUninitialize = NULL,
};

Shizu_defineObjectType("Zeitgeist.Visuals.Context", Visuals_Context, Shizu_Object);

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

static void
Visuals_Context_constructImpl
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  )
{
  if (1 != numberOfArgumentValues) {
    Shizu_State2_setStatus(state, Shizu_Status_NumberOfArgumentsInvalid);
    Shizu_State2_jump(state);
  }
  Shizu_Type* TYPE = Visuals_Context_getType(state);
  Visuals_Context* SELF = (Visuals_Context*)Shizu_Value_getObject(&argumentValues[0]);
  Shizu_Object_construct(state, (Shizu_Object*)SELF);
  ((Shizu_Object*)SELF)->type = TYPE;
}

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
