#if !defined(VISUALS_CONTEXT_H_INCLUDED)
#define VISUALS_CONTEXT_H_INCLUDED

#include "Commons.h"
typedef struct Visuals_Program Visuals_Program;
typedef struct Visuals_RenderBuffer Visuals_RenderBuffer;
typedef struct Visuals_VertexBuffer Visuals_VertexBuffer;

typedef enum Visuals_BlendFactor {
  Visuals_BlendFactor_SourceAlpha,
  Visuals_BlendFactor_OneMinusSourceAlpha
} Visuals_BlendFactor;

typedef enum Visuals_CullMode {
  Visuals_CullMode_None,
  Visuals_CullMode_Front,
  Visuals_CullMode_Back,
  Visuals_CullMode_FrontAndBack,
} Visuals_CullMode;

typedef enum Visuals_DepthFunction {
  Visuals_DepthFunction_LessThan,
  Visuals_DepthFunction_LessThanOrEqualTo,
  Visuals_DepthFunction_EqualTo,
  Visuals_DepthFunction_GreaterThan,
  Visuals_DepthFunction_GreaterThanOrEqualTo,
  Visuals_DepthFunction_NotEqualTo,
  Visuals_DepthFunction_Always,
  Visuals_DepthFunction_Never,
} Visuals_DepthFunction;

/// @since 1.0
/// @brief A visuals context.
Shizu_declareType(Visuals_Context);

struct Visuals_Context_Dispatch {
  Shizu_Object_Dispatch _parent;
  Visuals_Program* (*createProgram)(Shizu_State*, Visuals_Context*, Shizu_String* vertexSource, Shizu_String* fragmentSource);
  Visuals_RenderBuffer* (*createRenderBuffer)(Shizu_State*, Visuals_Context*);
  Visuals_VertexBuffer* (*createVertexBuffer)(Shizu_State*, Visuals_Context*);
  void (*setClearColor)(Shizu_State*, Visuals_Context*, Shizu_Float32 r, Shizu_Float32 g, Shizu_Float32 b, Shizu_Float32 a);
  void (*setClearDepth)(Shizu_State*, Visuals_Context*, Shizu_Float32 z);
  void (*setBlendFactors)(Shizu_State*, Visuals_Context*, Visuals_BlendFactor, Visuals_BlendFactor);
  void (*setCullMode)(Shizu_State*, Visuals_Context*, Visuals_CullMode);
  void (*setDepthFunction)(Shizu_State*, Visuals_Context*, Visuals_DepthFunction);
  void (*setViewport)(Shizu_State*, Visuals_Context*, Shizu_Float32 left, Shizu_Float32 bottom, Shizu_Float32 width, Shizu_Float32 height);
  void (*clear)(Shizu_State*, Visuals_Context*, bool colorBuffer, bool depthBuffer);
  void (*render)(Shizu_State*, Visuals_Context*, Visuals_VertexBuffer* vertexBuffer, Visuals_Program* program);
};

struct Visuals_Context {
  Shizu_Object _parent;
};

void
Visuals_Context_construct
  (
    Shizu_State* state,
    Visuals_Context* self
  );

static inline Visuals_Program*
Visuals_Context_createProgram
  (
    Shizu_State* state,
    Visuals_Context* self,
    Shizu_String* vertexSource,
    Shizu_String* fragmentSource
  )
{ Shizu_VirtualCallWithReturn(Visuals_Context, createProgram, self, vertexSource, fragmentSource); }

static inline Visuals_RenderBuffer*
Visuals_Context_createRenderBuffer
  (
    Shizu_State* state,
    Visuals_Context* self
  )
{ Shizu_VirtualCallWithReturn(Visuals_Context, createRenderBuffer, self); }

static inline Visuals_VertexBuffer*
Visuals_Context_createVertexBuffer
  (
    Shizu_State* state,
    Visuals_Context* self
  )
{ Shizu_VirtualCallWithReturn(Visuals_Context, createVertexBuffer, self); }

/// default value (0,0,0,0)
static inline void
Visuals_Context_setClearColor
  (
    Shizu_State* state,
    Visuals_Context* self,
    Shizu_Float32 r,
    Shizu_Float32 g,
    Shizu_Float32 b,
    Shizu_Float32 a
  )
{ Shizu_VirtualCall(Visuals_Context, setClearColor, self, r, g, b, a); }

/// default value is 1
static inline void
Visuals_Context_setClearDepth
  (
    Shizu_State* state,
    Visuals_Context* self,
    Shizu_Float32 z
  )
{ Shizu_VirtualCall(Visuals_Context, setClearDepth, self, z); }

// default value is (Visuals_BlendFactor_SourceAlpha, Visuals_BlendFactor_OneMinusSourceAlpha)
static inline void
Visuals_Context_setBlendFactors
  (
    Shizu_State* state,
    Visuals_Context* self,
    Visuals_BlendFactor source,
    Visuals_BlendFactor target
  )
{ Shizu_VirtualCall(Visuals_Context, setBlendFactors, self, source, target); }

/// default value is Visuals_CullMode_None
static inline void
Visuals_Context_setCullMode
  (
    Shizu_State* state,
    Visuals_Context* self,
    Visuals_CullMode cullMode
  )
{ Shizu_VirtualCall(Visuals_Context, setCullMode, self, cullMode); }

/// default value is Visuals_DepthFunction_Always
static inline void
Visuals_Context_setDepthFunction
  (
    Shizu_State* state,
    Visuals_Context* self,
    Visuals_DepthFunction depthFunction
  )
{ Shizu_VirtualCall(Visuals_Context, setDepthFunction, self, depthFunction); }

/// default value is (0,0,1,1)
static inline void
Visuals_Context_setViewport
  (
    Shizu_State* state,
    Visuals_Context* self,
    Shizu_Float32 left,
    Shizu_Float32 bottom,
    Shizu_Float32 width,
    Shizu_Float32 height
  )
{ Shizu_VirtualCall(Visuals_Context, setViewport, self, left, bottom, width, height); }

static inline void
Visuals_Context_clear
  (
    Shizu_State* state,
    Visuals_Context* self,
    bool colorBuffer,
    bool depthBuffer
  )
{ Shizu_VirtualCall(Visuals_Context, clear, self, colorBuffer, depthBuffer); }

static inline void
Visuals_Context_render
  (
    Shizu_State* state,
    Visuals_Context* self,
    Visuals_VertexBuffer* vertexBuffer,
    Visuals_Program* program
  )
{ Shizu_VirtualCall(Visuals_Context, render, self, vertexBuffer, program); }

#endif // VISUALS_CONTEXT_H_INCLUDED
