
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

static Shizu_TypeDescriptor const Visuals_Gl_Texture_Type = {
  .postCreateType = NULL,
  .preDestroyType = NULL,
  .visitType = NULL,
  .size = sizeof(Visuals_Gl_Texture),
  .finalize = (Shizu_OnFinalizeCallback*)&Visuals_Gl_Texture_finalize,
  .visit = NULL,
  .dispatchSize = sizeof(Visuals_Gl_Texture_Dispatch),
  .dispatchInitialize = (Shizu_OnDispatchInitializeCallback*)&Visuals_Gl_Texture_dispatchInitialize,
  .dispatchUninitialize = NULL,
};

Shizu_defineType(Visuals_Gl_Texture, Visuals_Texture);

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

void
Visuals_Gl_Texture_construct
  (
    Shizu_State2* state,
    Visuals_Gl_Texture* self
  )
{
  Shizu_Type* type = Visuals_Gl_Texture_getType(state);
  Visuals_Texture_construct(state, (Visuals_Texture*)self);
  self->textureId = 0;
  ((Shizu_Object*)self)->type = type;
}

Visuals_Gl_Texture*
Visuals_Gl_Texture_create
  (
    Shizu_State2* state
  )
{
  Visuals_Gl_Texture* self = (Visuals_Gl_Texture*)Shizu_Gc_allocateObject(state, sizeof(Visuals_Gl_Texture));
  Visuals_Gl_Texture_construct(state, self);
  return self;
}
