#include "Visuals/Gl/Context.h"

#include "Visuals/Gl/Program.h"
#include "Visuals/Gl/RenderBuffer.h"
#include "Visuals/Gl/VertexBuffer.h"
#include "Visuals/Service.package.h"

static void
Visuals_Gl_Context_finalize
  (
    Shizu_State2* state,
    Visuals_Gl_Context* self
  );

static void
Visuals_Gl_Context_dispatchInitialize
  (
    Shizu_State1* state1,
    Visuals_Gl_Context_Dispatch* self
  );

static Visuals_Program*
Visuals_Gl_Context_createProgramImpl
  (
    Shizu_State2* state,
    Visuals_Gl_Context* self,
    Shizu_String* vertexSource,
    Shizu_String* fragmentSource
  );

static Visuals_RenderBuffer*
Visuals_Gl_Context_createRenderBufferImpl
  (
    Shizu_State2* state,
    Visuals_Gl_Context* self
  );

static Visuals_VertexBuffer*
Visuals_Gl_Context_createVertexBufferImpl
  (
    Shizu_State2* state,
    Visuals_Gl_Context* self
  );

static void
Visuals_Gl_Context_setClearColorImpl
  (
    Shizu_State2* state,
    Visuals_Gl_Context* self,
    Shizu_Float32 r,
    Shizu_Float32 g,
    Shizu_Float32 b,
    Shizu_Float32 a
  );

static void
Visuals_Gl_Context_setClearDepthImpl
  (
    Shizu_State2* state,
    Visuals_Gl_Context* self,
    Shizu_Float32 z
  );

static void
Visuals_Gl_Context_setBlendFactorsImpl
  (
    Shizu_State2* state,
    Visuals_Gl_Context* self,
    Visuals_BlendFactor source,
    Visuals_BlendFactor target
  );

static void
Visuals_Gl_Context_setCullModeImpl
  (
    Shizu_State2* state,
    Visuals_Gl_Context* self,
    Visuals_CullMode cullMode
  );

static void
Visuals_Gl_Context_setDepthFunctionImpl
  (
    Shizu_State2* state,
    Visuals_Gl_Context* self,
    Visuals_DepthFunction depthFunction
  );

static void
Visuals_Gl_Context_setViewportImpl
  (
    Shizu_State2* state,
    Visuals_Gl_Context* self,
    Shizu_Float32 left,
    Shizu_Float32 bottom,
    Shizu_Float32 width,
    Shizu_Float32 height
  );

static void
Visuals_Gl_Context_clearImpl
  (
    Shizu_State2* state,
    Visuals_Gl_Context* self,
    Shizu_Boolean colorBuffer,
    Shizu_Boolean depthBuffer
  );

static inline void
Visuals_Gl_Context_setRenderBufferImpl
  (
    Shizu_State2* state,
    Visuals_Gl_Context* self,
    Visuals_Gl_RenderBuffer* renderBuffer
  );

static inline void
Visuals_Gl_Context_renderImpl
  (
    Shizu_State2* state,
    Visuals_Gl_Context* self,
    Visuals_Gl_VertexBuffer* vertexBuffer,
    Visuals_Gl_Program* program
  );

static void
Visuals_Gl_Context_constructImpl
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  );

static Shizu_ObjectTypeDescriptor const Visuals_Gl_Context_Type = {
  .postCreateType = NULL,
  .preDestroyType = NULL,
  .visitType = NULL,
  .size = sizeof(Visuals_Gl_Context),
  .construct = &Visuals_Gl_Context_constructImpl,
  .finalize = (Shizu_OnFinalizeCallback*)&Visuals_Gl_Context_finalize,
  .visit = NULL,
  .dispatchSize = sizeof(Visuals_Gl_Context_Dispatch),
  .dispatchInitialize = (Shizu_OnDispatchInitializeCallback*)&Visuals_Gl_Context_dispatchInitialize,
  .dispatchUninitialize = NULL,
};

Shizu_defineObjectType("Zeitgeist.Visuals.Gl.Context", Visuals_Gl_Context, Visuals_Context);

static void
Visuals_Gl_Context_finalize
  (
    Shizu_State2* state,
    Visuals_Gl_Context* self
  )
{/*Intentionally empty.*/}

static void
Visuals_Gl_Context_dispatchInitialize
  (
    Shizu_State1* state1,
    Visuals_Gl_Context_Dispatch* self
  )
{
  ((Visuals_Context_Dispatch*)self)->createProgram = (Visuals_Program*(*)(Shizu_State2*,Visuals_Context*,Shizu_String*,Shizu_String*)) & Visuals_Gl_Context_createProgramImpl;
  ((Visuals_Context_Dispatch*)self)->createRenderBuffer = (Visuals_RenderBuffer * (*)(Shizu_State2*, Visuals_Context*)) &Visuals_Gl_Context_createRenderBufferImpl;
  ((Visuals_Context_Dispatch*)self)->createVertexBuffer = (Visuals_VertexBuffer * (*)(Shizu_State2*, Visuals_Context*)) &Visuals_Gl_Context_createVertexBufferImpl;
  ((Visuals_Context_Dispatch*)self)->setClearColor = (void (*)(Shizu_State2*, Visuals_Context*, Shizu_Float32 r, Shizu_Float32 g, Shizu_Float32 b, Shizu_Float32 a)) & Visuals_Gl_Context_setClearColorImpl;
  ((Visuals_Context_Dispatch*)self)->setClearDepth = (void (*)(Shizu_State2*, Visuals_Context*, Shizu_Float32 z)) & Visuals_Gl_Context_setClearDepthImpl;
  ((Visuals_Context_Dispatch*)self)->setBlendFactors = (void (*)(Shizu_State2*, Visuals_Context*, Visuals_BlendFactor, Visuals_BlendFactor)) & Visuals_Gl_Context_setBlendFactorsImpl;
  ((Visuals_Context_Dispatch*)self)->setCullMode = (void (*)(Shizu_State2*, Visuals_Context*, Visuals_CullMode)) & Visuals_Gl_Context_setCullModeImpl;
  ((Visuals_Context_Dispatch*)self)->setDepthFunction = (void (*)(Shizu_State2*, Visuals_Context*, Visuals_DepthFunction)) & Visuals_Gl_Context_setDepthFunctionImpl;
  ((Visuals_Context_Dispatch*)self)->setViewport = (void (*)(Shizu_State2*, Visuals_Context*, Shizu_Float32, Shizu_Float32, Shizu_Float32, Shizu_Float32)) & Visuals_Gl_Context_setViewportImpl;
  ((Visuals_Context_Dispatch*)self)->clear = (void (*)(Shizu_State2*, Visuals_Context*, bool, bool)) & Visuals_Gl_Context_clearImpl;
  ((Visuals_Context_Dispatch*)self)->setRenderBuffer = (void (*)(Shizu_State2*, Visuals_Context*, Visuals_RenderBuffer*)) & Visuals_Gl_Context_setRenderBufferImpl;
  ((Visuals_Context_Dispatch*)self)->render = (void (*)(Shizu_State2*, Visuals_Context*, Visuals_VertexBuffer*, Visuals_Program*)) & Visuals_Gl_Context_renderImpl;
}

static Visuals_Program*
Visuals_Gl_Context_createProgramImpl
  (
    Shizu_State2* state,
    Visuals_Gl_Context* self,
    Shizu_String* vertexSource,
    Shizu_String* fragmentSource
  )
{
  Visuals_Program* p = (Visuals_Program*)Visuals_Gl_Program_create(state, vertexSource, fragmentSource);
  Shizu_JumpTarget jumpTarget;
  Shizu_State2_pushJumpTarget(state, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    Visuals_Service_registerVisualsObject(state, (Visuals_Object*)p);
    Shizu_State2_popJumpTarget(state);
  } else {
    Shizu_State2_popJumpTarget(state);
    Visuals_Object_unmaterialize(state, (Visuals_Object*)p);
    Shizu_State2_jump(state);
  }
  return p;
}

static Visuals_RenderBuffer*
Visuals_Gl_Context_createRenderBufferImpl
  (
    Shizu_State2* state,
    Visuals_Gl_Context* self
  )
{
  Visuals_RenderBuffer* p = (Visuals_RenderBuffer*)Visuals_Gl_RenderBuffer_create(state);
  Shizu_JumpTarget jumpTarget;
  Shizu_State2_pushJumpTarget(state, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    Visuals_Service_registerVisualsObject(state, (Visuals_Object*)p);
    Shizu_State2_popJumpTarget(state);
  } else {
    Shizu_State2_popJumpTarget(state);
    Visuals_Object_unmaterialize(state, (Visuals_Object*)p);
    Shizu_State2_jump(state);
  }
  return p;
}

static Visuals_VertexBuffer*
Visuals_Gl_Context_createVertexBufferImpl
  (
    Shizu_State2* state,
    Visuals_Gl_Context* self
  )
{
  Visuals_VertexBuffer* p = (Visuals_VertexBuffer*)Visuals_Gl_VertexBuffer_create(state);
  Shizu_JumpTarget jumpTarget;
  Shizu_State2_pushJumpTarget(state,&jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    Visuals_Service_registerVisualsObject(state, (Visuals_Object*)p);
    Shizu_State2_popJumpTarget(state);
  } else {
    Shizu_State2_popJumpTarget(state);
    Visuals_Object_unmaterialize(state, (Visuals_Object*)p);
    Shizu_State2_jump(state);
  }
  return p;
}

static void
Visuals_Gl_Context_setClearColorImpl
  (
    Shizu_State2* state,
    Visuals_Gl_Context* self,
    Shizu_Float32 r,
    Shizu_Float32 g,
    Shizu_Float32 b,
    Shizu_Float32 a
  )
{ glClearColor(r, g, b, a); }

static void
Visuals_Gl_Context_setClearDepthImpl
  (
    Shizu_State2* state,
    Visuals_Gl_Context* self,
    Shizu_Float32 z
  )
{ glClearDepth(z); }

static void
Visuals_Gl_Context_setBlendFactorsImpl
  (
    Shizu_State2* state,
    Visuals_Gl_Context* self,
    Visuals_BlendFactor source,
    Visuals_BlendFactor target
  )
{
  GLenum sourceGl;

  switch (source) {
    case Visuals_BlendFactor_SourceAlpha: {
      sourceGl = GL_SRC_ALPHA;
    } break;
    case Visuals_BlendFactor_OneMinusSourceAlpha: {
      sourceGl = GL_ONE_MINUS_SRC_ALPHA;
    } break;
    default: {
      Shizu_State2_setStatus(state, Shizu_Status_ArgumentOutOfRange);
      Shizu_State2_jump(state);
    } break;
  };
  GLenum targetGl;
  switch (target) {
    case Visuals_BlendFactor_SourceAlpha: {
      targetGl = GL_SRC_ALPHA;
    } break;
    case Visuals_BlendFactor_OneMinusSourceAlpha: {
      targetGl = GL_ONE_MINUS_SRC_ALPHA;
    } break;
    default: {
      Shizu_State2_setStatus(state, Shizu_Status_ArgumentOutOfRange);
      Shizu_State2_jump(state);
    } break;
  };
  glEnable(GL_BLEND);
  glBlendFunc(sourceGl, targetGl);
}

static void
Visuals_Gl_Context_setCullModeImpl
  ( 
    Shizu_State2* state,
    Visuals_Gl_Context* self,
    Visuals_CullMode cullMode
  )
{
  switch (cullMode) {
    case Visuals_CullMode_Back: {
      glEnable(GL_CULL_FACE);
      glCullFace(GL_BACK);
    } break;
    case Visuals_CullMode_Front: {
      glEnable(GL_CULL_FACE);
      glCullFace(GL_FRONT);
    } break;
    case Visuals_CullMode_FrontAndBack: {
      glEnable(GL_CULL_FACE);
      glCullFace(GL_FRONT_AND_BACK);
    } break;
    case Visuals_CullMode_None: {
      glDisable(GL_CULL_FACE);
      glCullFace(GL_BACK);
    } break;
    default: {
      Shizu_State2_setStatus(state, Shizu_Status_ArgumentOutOfRange);
      Shizu_State2_jump(state);
    } break;
  };
}

static void
Visuals_Gl_Context_setDepthFunctionImpl
  (
    Shizu_State2* state,
    Visuals_Gl_Context* self,
    Visuals_DepthFunction depthFunction
  )
{
  switch (depthFunction) {
    case Visuals_DepthFunction_LessThan: {
      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LESS);
    } break;
    case Visuals_DepthFunction_LessThanOrEqualTo: {
      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LEQUAL);
    } break;
    case Visuals_DepthFunction_GreaterThan: {
      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_GREATER);
    } break;
    case Visuals_DepthFunction_GreaterThanOrEqualTo: {
      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_GEQUAL);
    } break;
    case Visuals_DepthFunction_NotEqualTo: {
      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_NOTEQUAL);
    } break;
    case Visuals_DepthFunction_EqualTo: {
      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_EQUAL);
    } break;
    case Visuals_DepthFunction_Always: {
      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_ALWAYS);
    } break;
    case Visuals_DepthFunction_Never: {
      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_NEVER);
    } break;
    default: {
      Shizu_State2_setStatus(state, Shizu_Status_ArgumentOutOfRange);
      Shizu_State2_jump(state);
    } break;
  };
}

static void
Visuals_Gl_Context_setViewportImpl
  (
    Shizu_State2* state,
    Visuals_Gl_Context* self,
    Shizu_Float32 left,
    Shizu_Float32 bottom,
    Shizu_Float32 width,
    Shizu_Float32 height
  )
{
  Shizu_Integer32 clientWidth, clientHeight;
  Visuals_Gl_Service_getClientSize(state, &clientWidth, &clientHeight);
  self->viewport.left = left;
  self->viewport.bottom = bottom;
  self->viewport.width = width;
  self->viewport.height = height;
  glViewport(left * clientWidth, bottom * clientHeight, width * clientWidth, height * clientHeight);
  glEnable(GL_SCISSOR_TEST);
  glScissor(self->viewport.left * clientWidth, self->viewport.bottom * clientHeight, self->viewport.width * clientWidth, self->viewport.height * clientHeight);
}

static void
Visuals_Gl_Context_clearImpl
  (
    Shizu_State2* state,
    Visuals_Gl_Context* self,
    Shizu_Boolean colorBuffer,
    Shizu_Boolean depthBuffer
  )
{
  GLbitfield mask = 0; 
  if (colorBuffer) {
    mask |= GL_COLOR_BUFFER_BIT;
  }
  if (depthBuffer) {
    mask |= GL_DEPTH_BUFFER_BIT;
  }
  Shizu_Integer32 clientWidth, clientHeight;
  Visuals_Gl_Service_getClientSize(state, &clientWidth, &clientHeight);
  
  glViewport(self->viewport.left * clientWidth, self->viewport.bottom * clientHeight, self->viewport.width * clientWidth, self->viewport.height * clientHeight);

  glEnable(GL_SCISSOR_TEST);
  glScissor(self->viewport.left * clientWidth, self->viewport.bottom * clientHeight, self->viewport.width * clientWidth, self->viewport.height * clientHeight);
  
  glClear(mask);
}

static inline void
Visuals_Gl_Context_setRenderBufferImpl
  (
    Shizu_State2* state,
    Visuals_Gl_Context* self,
    Visuals_Gl_RenderBuffer* renderBuffer
  )
{
  if (!renderBuffer) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  } else {
    if (!renderBuffer->frameBufferId) {
      Shizu_State2_setStatus(state, Shizu_Status_OperationInvalid);
      Shizu_State2_jump(state);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, renderBuffer->frameBufferId);
  }
}

static inline void
Visuals_Gl_Context_renderImpl
  (
    Shizu_State2* state,
    Visuals_Gl_Context* self,
    Visuals_Gl_VertexBuffer* vertexBuffer,
    Visuals_Gl_Program* program
  )
{
  glUseProgram(program->programId);
  glBindVertexArray(vertexBuffer->vertexArrayId);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, ((Visuals_VertexBuffer*)vertexBuffer)->numberOfVertices);
}

static void
Visuals_Gl_Context_constructImpl
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
  Shizu_Type* TYPE = Visuals_Gl_Context_getType(state);
  Visuals_Gl_Context* SELF = (Visuals_Gl_Context*)Shizu_Value_getObject(&argumentValues[0]);
  {
    Shizu_Type* PARENTTYPE = Shizu_Types_getParentType(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), TYPE);
    Shizu_Value returnValue = Shizu_Value_Initializer();
    Shizu_Value argumentValues[] = { Shizu_Value_Initializer() };
    Shizu_Value_setObject(&argumentValues[0], (Shizu_Object*)SELF);
    Shizu_Type_getObjectTypeDescriptor(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), PARENTTYPE)->construct(state, &returnValue, 1, &argumentValues[0]);
  }
  SELF->viewport.left = 0.f;
  SELF->viewport.bottom = 0.f;
  SELF->viewport.width = 1.f;
  SELF->viewport.height = 1.f;
  ((Shizu_Object*)SELF)->type = TYPE;
}

Visuals_Gl_Context*
Visuals_Gl_Context_create
  (
    Shizu_State2* state
  )
{
  Shizu_Value returnValue = Shizu_Value_Initializer();
  Shizu_Value argumentValues[] = { Shizu_Value_Initializer() };
  Shizu_Value_setType(&argumentValues[0], Visuals_Gl_Context_getType(state));
  Shizu_Operations_create(state, &returnValue, 1, &argumentValues[0]);
  return (Visuals_Gl_Context*)Shizu_Value_getObject(&returnValue);
}
