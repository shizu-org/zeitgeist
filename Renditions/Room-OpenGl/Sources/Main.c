// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#include "Zeitgeist.h"


#include "Zeitgeist/UpstreamRequests.h"

// strlen
#include <string.h>

// fprintf, stdout
#include <stdio.h>

#include "KeyboardKeyMessage.h"
#include "ServiceGl.h"
#include "World.h"

#include "Visuals/DefaultPrograms.h"
#include "Visuals/Program.h"
#include "Visuals/RenderBuffer.h"
#include "Visuals/VertexBuffer.h"

#if Shizu_Configuration_OperatingSystem_Windows == Shizu_Configuration_OperatingSystem
  #define Shizu_Rendition_Export _declspec(dllexport)
#elif Shizu_Configuration_OperatingSystem_Linux == Shizu_Configuration_OperatingSystem
  #define Shizu_Rendition_Export
#else
  #error("operating system not (yet) supported")
#endif

Shizu_Rendition_Export char const* 
Shizu_Module_getName
  (
  )
{
  static char const* NAME = "<Dl: Room (OpenGL)>";
  return NAME;
}

Shizu_Rendition_Export Shizu_String*
Zeitgeist_Rendition_getName
  (
    Shizu_State* state
  )
{
  return Shizu_String_create(state, "Room (OpenGL)", strlen("Room (OpenGL)"));
}

static Visuals_Program* g_program = NULL;
static Visuals_RenderBuffer* g_renderBuffer = NULL;
static World* g_world = NULL;

static void
bindMatrix4Uniform
  (
    Shizu_State* state,
    GLuint programId,
    char const* name,
    Matrix4R32* value
  )
{
  GLint location = glGetUniformLocation(programId, name);
  if (-1 == location) {
    fprintf(stderr, "%s:%d: unable to get uniform location of uniform `%s`\n", __FILE__, __LINE__, name);
  } else {
    glUniformMatrix4fv(location, 1, GL_TRUE, idlib_matrix_4x4_f32_get_data(&value->m));
  }
}

static void
bindVector3Uniform
  (
    Shizu_State* state,
    GLuint programId,
    char const* name,
    Vector3R32* value
  )
{
  GLint location = glGetUniformLocation(programId, name);
  if (-1 == location) {
    fprintf(stderr, "%s:%d: unable to get uniform location of uniform `%s`\n", __FILE__, __LINE__, name);
  } else {
    glUniform3fv(location, 1, &value->v.e[0]);
  }
}

static void
bindIntegerUniform
  (
    Shizu_State* state,
    GLuint programId,
    char const* name,
    Shizu_Integer32 value
  )
{
  GLint location = glGetUniformLocation(programId, name);
  if (-1 == location) {
    fprintf(stderr, "%s:%d: unable to get uniform location of uniform `%s`\n", __FILE__, __LINE__, name);
  } else {
    /// @todo Check bounds.
    glUniform1i(location, value);
  }
}

static void
bindBooleanUniform
  ( 
    Shizu_State* state,
    GLuint programId,
    char const* name,
    Shizu_Boolean value
  )
{
  GLint location = glGetUniformLocation(programId, name);
  if (-1 == location) {
    fprintf(stderr, "%s:%d: unable to get uniform location of uniform `%s`\n", __FILE__, __LINE__, name);
  } else {
    glUniform1i(location, value ? 1 : 0);
  }
}

static void
bindInteger32Uniform
  (
    Shizu_State* state,
    GLuint programId,
    char const* name,
    Shizu_Integer32 value
  ) {
  GLint location = glGetUniformLocation(programId, name);
  if (-1 == location) {
    fprintf(stderr, "%s:%d: unable to get uniform location of uniform `%s`\n", __FILE__, __LINE__, name);
  } else {
    // Both GLint and Shizu_Integer32 are two's complements integer hence should have the same maximal/minimal values.
    glUniform1i(location, value);
  }
}

Shizu_Rendition_Export void
Zeitgeist_Rendition_update
  (
    Shizu_State* state
  )
{
  ServiceGl_update(state);
  World_update(state, g_world, 200); /* TODO: Pass delta time. */

  if (ServiceGl_quitRequested(state)) {
    Zeitgeist_UpstreamRequest* request = Zeitgeist_UpstreamRequest_createExitProcessRequest(state);
    Zeitgeist_sendUpstreamRequest(state, request);
  }

  Shizu_Integer32 viewportWidth, viewportHeight;
  ServiceGl_getClientSize(state, &viewportWidth, &viewportHeight);
  ServiceGl_beginFrame(state);


  glViewport(0, 0, viewportWidth, viewportHeight);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(((Visuals_GlProgram*)g_program)->programId);

  Matrix4R32* world = NULL;
  world = Matrix4R32_createScale(state, Vector3R32_create(state, 0.75f, 0.75f, 1.f));
  bindMatrix4Uniform(state, ((Visuals_GlProgram*)g_program)->programId, "matrices.world", world);
  // (viewRotateY^-1 * viewTranslate^-1)
  // is equivalent to
  // (viewTranslate * viewRotateY)^-1
  // as in general
  // (AB)^-1 = (B^-1) * (A^-1)
  // for two square matrices A and B holds.
  Matrix4R32* viewTranslate = NULL;
  viewTranslate = Matrix4R32_createTranslate(state, Vector3R32_create(state, -g_world->player->position->v.e[0], -g_world->player->position->v.e[1], -g_world->player->position->v.e[2]));
  Matrix4R32* viewRotateY = NULL;
  viewRotateY = Matrix4R32_createRotateY(state, -g_world->player->rotationY);
  Matrix4R32* view = NULL;
  view = Matrix4R32_multiply(state, viewRotateY, viewTranslate);
  bindMatrix4Uniform(state, ((Visuals_GlProgram*)g_program)->programId, "matrices.view", view);

  bindVector3Uniform(state, ((Visuals_GlProgram*)g_program)->programId, "viewer.position",
                     Vector3R32_create(state, g_world->player->position->v.e[0], g_world->player->position->v.e[1], g_world->player->position->v.e[2]));
  
  //
  Matrix4R32* projection = NULL;
  //projection = Matrix4R32_createOrthographic(state, -1.f, +1.f, -1.f, +1.f, -100.f, +100.f);
  projection = Matrix4R32_createPerspective(state, 90.f, viewportHeight > 0.f ? viewportWidth / viewportHeight : 16.f/9.f, 0.1f, 100.f);
  bindMatrix4Uniform(state, ((Visuals_GlProgram*)g_program)->programId, "matrices.projection", projection);
  
  // The color (255, 204, 51) is the websafe color "sunglow".
  bindVector3Uniform(state, ((Visuals_GlProgram*)g_program)->programId, "meshColor", Vector3R32_create(state, 1.0f, 0.8f, 0.2f));

  bindIntegerUniform(state, ((Visuals_GlProgram*)g_program)->programId, "inputFragmentColorType", 1);

  bindVector3Uniform(state, ((Visuals_GlProgram*)g_program)->programId, "ambientLightInfo.color", Vector3R32_create(state, 0.3f, 0.3f, 0.3f));
  bindVector3Uniform(state, ((Visuals_GlProgram*)g_program)->programId, "diffuseLightInfo.direction", Vector3R32_create(state, -1.f, -1.f, -1.f));
  bindVector3Uniform(state, ((Visuals_GlProgram*)g_program)->programId, "diffuseLightInfo.color", Vector3R32_create(state, 1.f, 1.f, 1.f));

  bindVector3Uniform(state, ((Visuals_GlProgram*)g_program)->programId, "specularLightInfo.direction", Vector3R32_create(state, -1.f, -1.f, -1.f));
  bindVector3Uniform(state, ((Visuals_GlProgram*)g_program)->programId, "specularLightInfo.color", Vector3R32_create(state, 0.8f, 0.8f, 0.8f));
  bindVector3Uniform(state, ((Visuals_GlProgram*)g_program)->programId, "specularLightInfo.position", Vector3R32_create(state, 0.f, 0.f, 0.f));
  static const Shizu_Integer32 LightType_Directional = 1;
  static const Shizu_Integer32 LightType_Point = 2;
  bindInteger32Uniform(state, ((Visuals_GlProgram*)g_program)->programId, "specularLightInfo.lightType", LightType_Point);
  static const Shizu_Integer32 LightModel_Phong = 1;
  static const Shizu_Integer32 LightModel_BlinnPhong = 2;
  bindInteger32Uniform(state, ((Visuals_GlProgram*)g_program)->programId, "specularLightInfo.lightModel", LightModel_BlinnPhong);

  Shizu_Value sizeValue = Shizu_List_getSize(state, g_world->geometries);
  for (Shizu_Integer32 i = 0, n = Shizu_Value_getInteger32(&sizeValue); i < n; ++i) {
    Shizu_Value elementValue = Shizu_List_getValue(state, g_world->geometries, i);
    StaticGeometryGl *element = (StaticGeometryGl*)Shizu_Value_getObject(&elementValue);
    glBindVertexArray(((Visuals_GlVertexBuffer*)element->vertexBuffer)->vertexArrayId);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, element->numberOfVertices);
  }

  glBindVertexArray(0);
  glUseProgram(0);

  ServiceGl_endFrame(state);
}

static void
onKeyboardKeyMessage
  (
    Shizu_State* state
  )
{
  if (Shizu_Stack_getSize(state) < 2) {
    fprintf(stderr, "%s:%d: too few arguments\n", __FILE__, __LINE__);
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  if (!Shizu_Stack_isInteger32(state, 0)) {
    fprintf(stderr, "%s:%d: invalid argument type\n", __FILE__, __LINE__);
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  if (1 != Shizu_Stack_getInteger32(state, 0)) {
    fprintf(stderr, "%s:%d: invalid number of arguments\n", __FILE__, __LINE__);
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  if (!Shizu_Stack_isObject(state, 1)) {
    fprintf(stderr, "%s:%d: invalid argument type\n", __FILE__, __LINE__);
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  fprintf(stdout, "%s:%d: keyboard key message received\n", __FILE__, __LINE__);
  KeyboardKeyMessage* message = (KeyboardKeyMessage*)Shizu_Stack_getObject(state, 1);
  if (KeyboardKey_Escape == KeyboardKeyMessage_getKey(state, message)) {
    if (KeyboardKey_Action_Released == KeyboardKeyMessage_getAction(state, message)) {
      Zeitgeist_UpstreamRequest* request = Zeitgeist_UpstreamRequest_createExitProcessRequest(state);
      Zeitgeist_sendUpstreamRequest(state, request);
    }
  } else {
    Player_onKeyboardKeyMessage(state, g_world->player, message);
  }
  Shizu_Stack_pop(state);
  Shizu_Stack_pop(state);
}

Shizu_Rendition_Export void
Zeitgeist_Rendition_load
  (
    Shizu_State* state
  )
{
  ServiceGl_startup(state);
  ServiceGl_setTitle(state, Shizu_String_create(state, "Room (OpenGL)", strlen("Room (OpenGL)")));

  Shizu_Value temporary;
  Shizu_Value_setCxxFunction(&temporary, &onKeyboardKeyMessage);
  ServiceGl_addKeyboardKeyCallback(state, &temporary);

  Shizu_JumpTarget jumpTarget;
  Shizu_State_pushJumpTarget(state, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    Visuals_Program* program = Visuals_getProgram(state, "pbr1");
    Visuals_Object_materialize(state, (Visuals_Object*)program);
    Shizu_Object_lock(state, (Shizu_Object*)program);
    g_program = program;
    World* world = World_create(state);
    Shizu_Object_lock(state, (Shizu_Object*)world);
    g_world = world;
    Visuals_RenderBuffer* renderBuffer = (Visuals_RenderBuffer*)Visuals_GlRenderBuffer_create(state);
    Shizu_Object_lock(state, (Shizu_Object*)renderBuffer);
    g_renderBuffer = renderBuffer;
    Shizu_State_popJumpTarget(state);
  } else {
    Shizu_State_popJumpTarget(state);
    if (g_renderBuffer) {
      Visuals_Object_unmaterialize(state, (Visuals_Object*)g_renderBuffer);
      Shizu_Object_unlock(state, (Shizu_Object*)g_renderBuffer);
      g_renderBuffer = NULL;
    }
    if (g_world) {
      Shizu_Object_unlock(state, (Shizu_Object*)g_world);
      g_world = NULL;
    }
    if (g_program) {
      Visuals_Object_unmaterialize(state, (Visuals_Object*)g_program);
      Shizu_Object_unlock(state, (Shizu_Object*)g_program);
      g_program = NULL;
    }
    Shizu_State_jump(state);
  }

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glClearColor(0.0f, 0.0f, 0.0f, 0.f);

  glDepthFunc(GL_LEQUAL);
  glClearDepth(1.f);

  fprintf(stdout, "OpenGL renderer: %s\n", glGetString(GL_RENDERER));
  fprintf(stdout, "OpenGL vendor:   %s\n", glGetString(GL_VENDOR));
  fprintf(stdout, "OpenGL version:  %s\n", glGetString(GL_VERSION));
}

Shizu_Rendition_Export void
Zeitgeist_Rendition_unload
  (
    Shizu_State* state
  )
{
  Shizu_Value sizeValue = Shizu_List_getSize(state, g_world->geometries);
  for (Shizu_Integer32 i = 0, n = Shizu_Value_getInteger32(&sizeValue); i < n; ++i) {
    Shizu_Value elementValue = Shizu_List_getValue(state, g_world->geometries, i);
    StaticGeometryGl* element = (StaticGeometryGl*)Shizu_Value_getObject(&elementValue);
    StaticGeometryGl_unmaterialize(state, element);
  }
  if (g_renderBuffer) {
    Visuals_Object_unmaterialize(state, (Visuals_Object*)g_renderBuffer);
    Shizu_Object_unlock(state, (Shizu_Object*)g_renderBuffer);
    g_renderBuffer = NULL;
  }
  if (g_world) {
    Shizu_Object_unlock(state, (Shizu_Object*)g_world);
    g_world = NULL;
  }
  if (g_program) {
    Visuals_Object_unmaterialize(state, (Visuals_Object*)g_program);
    Shizu_Object_unlock(state, (Shizu_Object*)g_program);
    g_program = NULL;
  }
  ServiceGl_shutdown(state);
}
