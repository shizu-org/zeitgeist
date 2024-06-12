// Copyright (c) 2024 Michael Heilmann. All rights reserved.

// strlen
#include <string.h>

// fprintf, stdout
#include <stdio.h>

#include "Zeitgeist/UpstreamRequests.h"
#include "Visuals/Service.h"

#include "Visuals/DefaultPrograms.h"
#include "Visuals/Program.h"
#include "Visuals/RenderBuffer.h"
#include "Visuals/VertexBuffer.h"
#include "Visuals/Gl/Context.h"

#if Shizu_Configuration_OperatingSystem_Windows == Shizu_Configuration_OperatingSystem
  #define Shizu_Rendition_Export _declspec(dllexport)
#elif Shizu_Configuration_OperatingSystem_Linux == Shizu_Configuration_OperatingSystem
  #define Shizu_Rendition_Export
#else
  #error("operating system not (yet) supported")
#endif

Shizu_Rendition_Export char const*
Shizu_ModuleLibrary_getName
  (
  )
{
  static char const* NAME = "<Dl:Hello World (OpenGL)>";
  return NAME;
}

Shizu_Rendition_Export Shizu_String*
Zeitgeist_Rendition_getName
  (
    Shizu_State* state
  )
{
  return Shizu_String_create(state, "Hello World (OpenGL)", strlen("Hello World (OpenGL)"));
}

static idlib_vector_3_f32 const SQUARE[] = {
  { -1.0f,  1.0f, 0.f, },
  { -1.0f, -1.0f, 0.f, },
  {  1.0f,  1.0f, 0.f, },
  {  1.0f, -1.0f, 0.f, },
};

static Visuals_Program* g_program = NULL;
static Visuals_VertexBuffer* g_vertexBuffer = NULL;
static Visuals_RenderBuffer* g_renderBuffer = NULL;

Shizu_Rendition_Export void
Zeitgeist_Rendition_update
  (
    Shizu_State* state
  )
{
  Visuals_Service_update(state);

  if (Visuals_Service_quitRequested(state)) {
    Zeitgeist_UpstreamRequest* request = Zeitgeist_UpstreamRequest_createExitProcessRequest(state);
    Zeitgeist_sendUpstreamRequest(state, request);
  }

  Visuals_Context* visualsContext = (Visuals_Context*)Visuals_Gl_Context_create(state);

  Shizu_Integer32 canvasWidth, canvasHeight;
  Visuals_Service_getClientSize(state, &canvasWidth, &canvasHeight);
  Visuals_Service_beginFrame(state);

  Visuals_Context_clear(state, visualsContext, true, true);
    
  Matrix4F32* m = Matrix4F32_createScale(state, Vector3F32_create(state, 0.75f, 0.75f, 1.f));
  Visuals_Program_bindMatrix4F32(state, g_program, "scale", m);

  // The color (255, 204, 51) is the websafe color "sunglow".
  Vector4F32 *c = Vector4F32_create(state, 1.0, 0.8, 0.2, 1.0);
  Visuals_Program_bindVector4F32(state, g_program, "inputColor", c);

  Visuals_Context_render(state, visualsContext, g_vertexBuffer, g_program);

  Visuals_Service_endFrame(state);
}

Shizu_Rendition_Export void
Zeitgeist_Rendition_load
  (
    Shizu_State* state
  )
{
  Shizu_JumpTarget jumpTarget;

  Visuals_Service_startup(state);

  Shizu_State_pushJumpTarget(state, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    Visuals_Context* visualsContext = (Visuals_Context*)Visuals_Gl_Context_create(state);
    Visuals_Service_setTitle(state, Shizu_String_create(state, "Hello World (OpenGL)", strlen("Hello World (OpenGL)")));
    Visuals_Program* program = Visuals_getProgram(state, "simple");
    Visuals_Object_materialize(state, (Visuals_Object*)program);
    Shizu_Object_lock(Shizu_State_getState1(state), Shizu_State_getLocks(state), (Shizu_Object*)program);
    g_program = program;
    Visuals_VertexBuffer* vertexBuffer = Visuals_Context_createVertexBuffer(state, visualsContext);
    Visuals_VertexBuffer_setData(state, vertexBuffer, Visuals_VertexSemantics_PositionXyz | Visuals_VertexSyntactics_Float3, SQUARE, sizeof(SQUARE));
    Shizu_Object_lock(Shizu_State_getState1(state), Shizu_State_getLocks(state), (Shizu_Object*)vertexBuffer);
    g_vertexBuffer = vertexBuffer;
    Visuals_RenderBuffer* renderBuffer = Visuals_Context_createRenderBuffer(state, visualsContext);
    Shizu_Object_lock(Shizu_State_getState1(state), Shizu_State_getLocks(state), (Shizu_Object*)renderBuffer);
    g_renderBuffer = renderBuffer;

    Shizu_String* p;

    p = Visuals_Service_getBackendVendorName(state);
    fprintf(stdout, "backend vendor: ");
    fwrite(Shizu_String_getBytes(state, p), 1, Shizu_String_getNumberOfBytes(state, p), stdout);
    fprintf(stdout, "\n");

    p = Visuals_Service_getBackendRendererName(state);
    fprintf(stdout, "backend renderer: ");
    fwrite(Shizu_String_getBytes(state, p), 1, Shizu_String_getNumberOfBytes(state, p), stdout);
    fprintf(stdout, "\n");

    fprintf(stdout, "backend version: %d.%d\n", Visuals_Service_getBackendMajorVersion(state), Visuals_Service_getBackendMinorVersion(state));

    Shizu_State_popJumpTarget(state);
  } else {
    Shizu_State_popJumpTarget(state);
    if (g_renderBuffer) {
      Visuals_Object_unmaterialize(state, (Visuals_Object*)g_renderBuffer);
      Shizu_Object_unlock(Shizu_State_getState1(state), Shizu_State_getLocks(state), (Shizu_Object*)g_renderBuffer);
      g_renderBuffer = NULL;
    }
    if (g_vertexBuffer) {
      Visuals_Object_unmaterialize(state, (Visuals_Object*)g_vertexBuffer);
      Shizu_Object_unlock(Shizu_State_getState1(state), Shizu_State_getLocks(state), (Shizu_Object*)g_vertexBuffer);
      g_vertexBuffer = NULL;
    }
    if (g_program) {
      Visuals_Object_unmaterialize(state, (Visuals_Object*)g_program);
      Shizu_Object_unlock(Shizu_State_getState1(state), Shizu_State_getLocks(state), (Shizu_Object*)g_program);
      g_program = NULL;
    }
    Visuals_Service_shutdown(state);
    Shizu_State_jump(state);
  }
}

Shizu_Rendition_Export void
Zeitgeist_Rendition_unload
  (
    Shizu_State* state
  )
{
  if (g_renderBuffer) {
    Visuals_Object_unmaterialize(state, (Visuals_Object*)g_renderBuffer);
    Shizu_Object_unlock(Shizu_State_getState1(state), Shizu_State_getLocks(state), (Shizu_Object*)g_renderBuffer);
    g_renderBuffer = NULL;
  }
  if (g_vertexBuffer) {
    Visuals_Object_unmaterialize(state, (Visuals_Object*)g_vertexBuffer);
    Shizu_Object_unlock(Shizu_State_getState1(state), Shizu_State_getLocks(state), (Shizu_Object*)g_vertexBuffer);
    g_vertexBuffer = NULL;
  }
  if (g_program) {
    Visuals_Object_unmaterialize(state, (Visuals_Object*)g_program);
    Shizu_Object_unlock(Shizu_State_getState1(state), Shizu_State_getLocks(state), (Shizu_Object*)g_program);
    g_program = NULL;
  }
  Visuals_Service_shutdown(state);
}
