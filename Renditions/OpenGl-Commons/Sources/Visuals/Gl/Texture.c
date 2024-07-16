
#include "Visuals/Gl/Texture.h"

static void
Visuals_Gl_Texture_finalize
  (
    Shizu_State2* state,
    Visuals_Gl_Texture* self
  );

static void
Visuals_Gl_Texture_materializeImpl
  (
    Shizu_State2* state,
    Visuals_Gl_Texture* self
  );

static void
Visuals_Gl_Texture_unmaterializeImpl
  (
    Shizu_State2* state,
    Visuals_Gl_Texture* self
  );

static void
Visuals_Gl_Texture_dispatchInitialize
  (
    Shizu_State1* state1,
    Visuals_Gl_Texture_Dispatch* self
  );

static void
Visuals_Gl_Texture_constructImpl
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  );

static Shizu_ObjectTypeDescriptor const Visuals_Gl_Texture_Type = {
  .postCreateType = NULL,
  .preDestroyType = NULL,
  .visitType = NULL,
  .size = sizeof(Visuals_Gl_Texture),
  .construct = Visuals_Gl_Texture_constructImpl,
  .finalize = (Shizu_OnFinalizeCallback*)&Visuals_Gl_Texture_finalize,
  .visit = NULL,
  .dispatchSize = sizeof(Visuals_Gl_Texture_Dispatch),
  .dispatchInitialize = (Shizu_OnDispatchInitializeCallback*)&Visuals_Gl_Texture_dispatchInitialize,
  .dispatchUninitialize = NULL,
};

Shizu_defineObjectType("Zeitgeist.Visuals.Gl.Texture", Visuals_Gl_Texture, Visuals_Texture);

static void
Visuals_Gl_Texture_finalize
  (
    Shizu_State2* state,
    Visuals_Gl_Texture* self
  )
{
  if (self->textureId) {
    glDeleteTextures(1, &self->textureId);
    self->textureId = 0;
  }
}

static void
Visuals_Gl_Texture_materializeImpl
  (
    Shizu_State2* state,
    Visuals_Gl_Texture* self
  )
{
  if (!self->textureId) {
    while (glGetError()) { }
    glGenTextures(1, &self->textureId);
    if (glGetError()) {
      fprintf(stderr, "%s:%d: %s failed\n", __FILE__, __LINE__, "glGenTextures");
      Shizu_State2_setStatus(state, 1);
      Shizu_State2_jump(state);
    }
  }
}

static void
Visuals_Gl_Texture_unmaterializeImpl
  (
    Shizu_State2* state,
    Visuals_Gl_Texture* self
  )
{
  if (self->textureId) {
    glDeleteTextures(1, &self->textureId);
    self->textureId = 0;
  }
}

static void
Visuals_Gl_Texture_dispatchInitialize
  (
    Shizu_State1* state1,
    Visuals_Gl_Texture_Dispatch* self
  )
{
  ((Visuals_Object_Dispatch*)self)->materialize = (void(*)(Shizu_State2*, Visuals_Object*)) & Visuals_Gl_Texture_materializeImpl;
  ((Visuals_Object_Dispatch*)self)->unmaterialize = (void(*)(Shizu_State2*, Visuals_Object*)) & Visuals_Gl_Texture_unmaterializeImpl;
}

static void
Visuals_Gl_Texture_constructImpl
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
  Shizu_Type* TYPE = Visuals_Gl_Texture_getType(state);
  Visuals_Gl_Texture* SELF = (Visuals_Gl_Texture*)Shizu_Value_getObject(&argumentValues[0]);
  {
    Shizu_Type* PARENTTYPE = Shizu_Types_getParentType(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), TYPE);
    Shizu_Value returnValue = Shizu_Value_Initializer();
    Shizu_Value argumentValues[] = { Shizu_Value_Initializer() };
    Shizu_Value_setObject(&argumentValues[0], (Shizu_Object*)SELF);
    Shizu_Type_getObjectTypeDescriptor(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), PARENTTYPE)->construct(state, &returnValue, 1, &argumentValues[0]);
  }
  SELF->textureId = 0;
  ((Shizu_Object*)SELF)->type = TYPE;
}

Visuals_Gl_Texture*
Visuals_Gl_Texture_create
  (
    Shizu_State2* state
  )
{
  Shizu_Value returnValue = Shizu_Value_Initializer();
  Shizu_Value argumentValues[] = { Shizu_Value_Initializer() };
  Shizu_Value_setType(&argumentValues[0], Visuals_Gl_Texture_getType(state));
  Shizu_Operations_create(state, &returnValue, 1, &argumentValues[0]);
  return (Visuals_Gl_Texture*)Shizu_Value_getObject(&returnValue);
}
