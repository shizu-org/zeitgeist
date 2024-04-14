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

#if Shizu_Configuration_OperatingSystem_Windows == Shizu_Configuration_OperatingSystem
  #define Shizu_Rendition_Export _declspec(dllexport)
#elif Shizu_Configuration_OperatingSystem_Linux == Shizu_Configuration_OperatingSystem
  #define Shizu_Rendition_Export
#else
  #error("operating system not (yet) supported")
#endif

Shizu_Rendition_Export char const* 
Shizu_getDlName
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

// Vertex shader.
// constant inputs:
//  meshAmbientColor vec4: The ambient color of the mesh in RGBF32.
//  inputFragmentAmbientColorType : Determine if the mesh ambient color or the vertex ambient color
//                                  is used as the input fragment color. This can assume two values:
//                                  0 (default and fallback, the mesh ambient color is used)
//                                  1 (the vertex ambient color is used)
// variable inputs:
//  vertexPosition vec3: The vertex position in model coordinates.
//  vertexAmbientColor vec4: The vertex ambient color in RGBF32.
static const GLchar* g_vertexShader =
  "#version 330 core\n"
  // vertex variables
  "layout(location = 0) in vec3 vertexPosition;\n"
  "layout(location = 1) in vec3 vertexNormal;\n"
  "layout(location = 2) in vec3 vertexColor;\n"
  // decide if mesh color or vertex color is used.
  "uniform int inputFragmentColorType = 0;\n"
  "uniform vec3 meshColor = vec3(1.0, 0.8, 0.2);\n"
  "out vec3 inputFragmentPosition;\n"
  "out vec3 inputFragmentNormal;\n"
  "out vec3 inputFragmentColor;\n"
  "uniform mat4 projection = mat4(1);\n"
  "uniform mat4 view = mat4(1);\n"
  "uniform mat4 world = mat4(1);\n"
  "void main() {\n"
  "  mat4 worldProjection = projection * view * world;\n"
  "  gl_Position = worldProjection * vec4(vertexPosition, 1.);\n"
  "  inputFragmentPosition = (worldProjection * vec4(vertexPosition, 1.)).xyz;\n"
  "  inputFragmentNormal = vertexNormal;\n"
  "  if(inputFragmentColorType == 1) {\n"
  "    inputFragmentColor = vertexColor;\n"
  "  } else {\n"
  "    inputFragmentColor = meshColor;\n"
  "  }\n"
  "}\n"
  ;

// Fragment shader.
// light type 1:
// directionless, locationless light
// 
// light type 2:
// emit light rays from the points on a plane in the direction of the normal vector
// 
// The color (255, 204, 51) is the websafe color "sunglow".
static const GLchar* g_fragmentShader =
  "#version 330 core\n"
  "out vec4 outputFragmentColor;\n"
  "in vec3 inputFragmentPosition;\n"
  "in vec3 inputFragmentNormal;\n"
  "in vec3 inputFragmentColor;\n"

  /* diffuse lighting (color + direction) */
  "uniform vec3 diffuseLightDirection = vec3(1, 1, 1);\n"
  "uniform vec3 diffuseLightColor = vec3(1, 1, 1);\n"

  /* ambient lighting (color) */
  "uniform vec3 ambientLightColor = vec3(1, 1, 1);\n"

  "void main() {\n"
  "  vec3 n = normalize(inputFragmentNormal);\n"
  "  vec3 d = normalize(diffuseLightDirection);\n"
  "  float diffuseIntensity = max(dot(n, d), 0.0);\n"
  "  vec3 diffuse = diffuseIntensity * diffuseLightColor;\n"
  "  vec3 ambient = ambientLightColor;\n"
  "  vec3 color = (ambient + diffuse) * inputFragmentColor;\n"
  "  outputFragmentColor = vec4(color, 1.f);\n"
  "}\n"
  ;

#include "Visuals/Program.h"

static Visuals_GlProgram* g_program = NULL;
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

  glUseProgram(g_program->programId);

  //
  Matrix4R32* world = NULL;
  world = Matrix4R32_createScale(state, Vector3R32_create(state, 0.75f, 0.75f, 1.f));
  bindMatrix4Uniform(state, g_program->programId, "world", world);
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
  bindMatrix4Uniform(state, g_program->programId, "view", view);
  
  //
  Matrix4R32* projection = NULL;
  //projection = Matrix4R32_createOrthographic(state, -1.f, +1.f, -1.f, +1.f, -100.f, +100.f);
  projection = Matrix4R32_createPerspective(state, 90.f, viewportHeight > 0.f ? viewportWidth / viewportHeight : 16.f/9.f, 0.1f, 100.f);
  bindMatrix4Uniform(state, g_program->programId, "projection", projection);
  
  // The color (255, 204, 51) is the websafe color "sunglow".
  bindVector3Uniform(state, g_program->programId, "meshColor", Vector3R32_create(state, 1.0f, 0.8f, 0.2f));

  bindIntegerUniform(state, g_program->programId, "inputFragmentColorType", 1);

  bindVector3Uniform(state, g_program->programId, "ambientLightColor", Vector3R32_create(state, 1.f, 1.f, 1.f));
  bindVector3Uniform(state, g_program->programId, "diffuseLightDirection", Vector3R32_create(state, 1.f, 1.f, 1.f));
  bindVector3Uniform(state, g_program->programId, "diffuseLightColor", Vector3R32_create(state, 1.f, 1.f, 1.f));

  Shizu_Value sizeValue = Shizu_List_getSize(state, g_world->geometries);
  for (Shizu_Integer32 i = 0, n = Shizu_Value_getInteger32(&sizeValue); i < n; ++i) {
    Shizu_Value elementValue = Shizu_List_getValue(state, g_world->geometries, i);
    StaticGeometryGl *element = (StaticGeometryGl*)Shizu_Value_getObject(&elementValue);
    glBindVertexArray(element->vertexArrayId);
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
    Shizu_State_setError(state, 1);
    Shizu_State_jump(state);
  }
  if (!Shizu_Stack_isInteger32(state, 0)) {
    fprintf(stderr, "%s:%d: invalid argument type\n", __FILE__, __LINE__);
    Shizu_State_setError(state, 1);
    Shizu_State_jump(state);
  }
  if (1 != Shizu_Stack_getInteger32(state, 0)) {
    fprintf(stderr, "%s:%d: invalid number of arguments\n", __FILE__, __LINE__);
    Shizu_State_setError(state, 1);
    Shizu_State_jump(state);
  }
  if (!Shizu_Stack_isObject(state, 1)) {
    fprintf(stderr, "%s:%d: invalid argument type\n", __FILE__, __LINE__);
    Shizu_State_setError(state, 1);
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

static void
loadPrograms
  (
    Shizu_State* state
  )
{
  g_program = Visuals_GlProgram_create(state, Shizu_String_create(state, g_vertexShader, strlen(g_vertexShader)),
                                              Shizu_String_create(state, g_fragmentShader, strlen(g_fragmentShader)));
  Shizu_Object_lock(state, (Shizu_Object*)g_program);
  Visuals_GlProgram_materialize(state, g_program);
}

static void
unloadPrograms
  (
    Shizu_State* state
  )
{
  Visuals_GlProgram_unmaterialize(state, g_program);
  Shizu_Object_unlock(state, (Shizu_Object*)g_program);
  g_program = NULL;
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

  loadPrograms(state);

  Shizu_JumpTarget jumpTarget;
  //
  Shizu_State_pushJumpTarget(state, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    g_world = World_create(state);
    Shizu_State_popJumpTarget(state);
  } else {
    Shizu_State_popJumpTarget(state);
    Visuals_GlProgram_unmaterialize(state, g_program);
    g_program = NULL;
    Shizu_State_jump(state);
  }
  //
  Shizu_State_pushJumpTarget(state, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    Shizu_Object_lock(state, (Shizu_Object*)g_world);
    Shizu_State_popJumpTarget(state);
  } else {
    Shizu_State_popJumpTarget(state);
    g_world = NULL;
    Visuals_GlProgram_unmaterialize(state, g_program);
    g_program = NULL;
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
  Shizu_Object_unlock(state, (Shizu_Object*)g_world);
  g_world = NULL;
  unloadPrograms(state);
  ServiceGl_shutdown(state);
}
