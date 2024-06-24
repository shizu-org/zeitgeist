// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#include "Zeitgeist.h"


#include "Zeitgeist/UpstreamRequests.h"

// strlen
#include <string.h>

// fprintf, stdout
#include <stdio.h>

#include "KeyboardKeyMessage.h"
#include "Visuals/Service.h"
#include "World.h"

#include "Visuals/DefaultPrograms.h"
#include "Visuals/Program.h"
#include "Visuals/RenderBuffer.h"
#include "Visuals/VertexBuffer.h"
#include "Visuals/Material.h"
#include "Visuals/PhongMaterial.h"
#include "Visuals/BlinnPhongMaterial.h"
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
  static char const* NAME = "<Dl: Room (OpenGL)>";
  return NAME;
}

Shizu_Rendition_Export Shizu_String*
Zeitgeist_Rendition_getName
  (
    Shizu_State2* state
  )
{
  return Shizu_String_create(state, "Room (OpenGL)", strlen("Room (OpenGL)"));
}

static Visuals_Program* g_program = NULL;
static Visuals_RenderBuffer* g_renderBuffer = NULL;
static World* g_world = NULL;

#define LightModel_Phong (1)
#define LightModel_BlinnPhong (2)
static Shizu_Integer32 g_lightModel = LightModel_Phong;

static void bindPhongMaterial(Shizu_State2* state, Visuals_Context* context, Visuals_Program* program, Visuals_PhongMaterial* material) {
  Visuals_Program_bindVector3F32(state, g_program, "phongMaterial.ambient", Vector3F32_create(state, ((Shizu_Float32)material->ambientR) / 255.f,
                                                                                                     ((Shizu_Float32)material->ambientG) / 255.f,
                                                                                                     ((Shizu_Float32)material->ambientB) / 255.f));
  Visuals_Program_bindVector3F32(state, g_program, "phongMaterial.diffuse", Vector3F32_create(state, ((Shizu_Float32)material->diffuseR) / 255.f,
                                                                                                     ((Shizu_Float32)material->diffuseG) / 255.f,
                                                                                                     ((Shizu_Float32)material->diffuseB) / 255.f));
  Visuals_Program_bindVector3F32(state, g_program, "phongMaterial.specular", Vector3F32_create(state, ((Shizu_Float32)material->specularR) / 255.f,
                                                                                                      ((Shizu_Float32)material->specularG) / 255.f,
                                                                                                      ((Shizu_Float32)material->specularB) / 255.f));
  Visuals_Program_bindFloat32(state, g_program, "phongMaterial.shininess", ((Shizu_Float32)material->shininess) / 255.f);
}

static void bindBlinnPhongMaterial(Shizu_State2* state, Visuals_Context* context, Visuals_Program* program, Visuals_BlinnPhongMaterial* material) {
  Visuals_Program_bindVector3F32(state, g_program, "blinnPhongMaterial.ambient", Vector3F32_create(state, ((Shizu_Float32)material->ambientR) / 255.f,
                                                                                                          ((Shizu_Float32)material->ambientG) / 255.f,
                                                                                                          ((Shizu_Float32)material->ambientB) / 255.f));
  Visuals_Program_bindVector3F32(state, g_program, "blinnPhongMaterial.diffuse", Vector3F32_create(state, ((Shizu_Float32)material->diffuseR) / 255.f,
                                                                                                          ((Shizu_Float32)material->diffuseG) / 255.f,
                                                                                                          ((Shizu_Float32)material->diffuseB) / 255.f));
  Visuals_Program_bindVector3F32(state, g_program, "blinnPhongMaterial.specular", Vector3F32_create(state, ((Shizu_Float32)material->specularR) / 255.f,
                                                                                                           ((Shizu_Float32)material->specularG) / 255.f,
                                                                                                           ((Shizu_Float32)material->specularB) / 255.f));
  Visuals_Program_bindFloat32(state, g_program, "blinnPhongMaterial.shininess", ((Shizu_Float32)material->shininess) / 255.f);
}

static void bindMaterial(Shizu_State2* state, Visuals_Context* context, Visuals_Program* program, Visuals_Material* material) {
  if (Shizu_Types_isSubTypeOf(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), ((Shizu_Object*)material)->type, Visuals_PhongMaterial_getType(state))) {
    bindPhongMaterial(state, context, program, (Visuals_PhongMaterial*)material);
  }
  if (Shizu_Types_isSubTypeOf(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), ((Shizu_Object*)material)->type, Visuals_BlinnPhongMaterial_getType(state))) {
    bindBlinnPhongMaterial(state, context, program, (Visuals_BlinnPhongMaterial*)material);
  }
}

Shizu_Rendition_Export void
Zeitgeist_Rendition_update
  (
    Shizu_State2* state
  )
{
  Visuals_Service_update(state);
  World_update(state, g_world, 200); /* TODO: Pass delta time. */

  if (Visuals_Service_quitRequested(state)) {
    Zeitgeist_UpstreamRequest* request = Zeitgeist_UpstreamRequest_createExitProcessRequest(state);
    Zeitgeist_sendUpstreamRequest(state, request);
  }

  Visuals_Context* visualsContext = (Visuals_Context*)Visuals_Gl_Context_create(state);

  Shizu_Integer32 canvasWidth, canvasHeight;
  Visuals_Service_getClientSize(state, &canvasWidth, &canvasHeight);
  Visuals_Service_beginFrame(state);

  Visuals_Context_clear(state, visualsContext, true, true);

  Matrix4F32* world = NULL;
  world = Matrix4F32_createScale(state, Vector3F32_create(state, 0.75f, 0.75f, 1.f));
  Visuals_Program_bindMatrix4F32(state, g_program, "matrices.world", world);
  // (viewRotateY^-1 * viewTranslate^-1)
  // is equivalent to
  // (viewTranslate * viewRotateY)^-1
  // as in general
  // (AB)^-1 = (B^-1) * (A^-1)
  // for two square matrices A and B holds.
  Matrix4F32* viewTranslate = NULL;
  viewTranslate = Matrix4F32_createTranslate(state, Vector3F32_create(state, -g_world->player->position->v.e[0], -g_world->player->position->v.e[1], -g_world->player->position->v.e[2]));
  Matrix4F32* viewRotateY = NULL;
  viewRotateY = Matrix4F32_createRotateY(state, -g_world->player->rotationY);
  Matrix4F32* view = NULL;
  view = Matrix4F32_multiply(state, viewRotateY, viewTranslate);
  Visuals_Program_bindMatrix4F32(state, g_program, "matrices.view", view);

  Visuals_Program_bindVector3F32(state, g_program, "viewer.position",
                                 Vector3F32_create(state, g_world->player->position->v.e[0], g_world->player->position->v.e[1], g_world->player->position->v.e[2]));
  
  Matrix4F32* projection = NULL;
  //projection = Matrix4R32_createOrthographic(state, -1.f, +1.f, -1.f, +1.f, -100.f, +100.f);
  projection = Matrix4F32_createPerspective(state, 90.f, canvasHeight > 0.f ? canvasWidth / canvasHeight : 16.f/9.f, 0.1f, 100.f);
  Visuals_Program_bindMatrix4F32(state, g_program, "matrices.projection", projection);
 
  Visuals_Program_bindInteger32(state, g_program, "currentNumberOfLights", 3);
  // Define an ambient light source.
  Visuals_Program_bindInteger32(state, g_program, "g_lights[0].type", 4);
  Visuals_Program_bindVector3F32(state, g_program, "g_lights[0].color", Vector3F32_create(state, 0.2f, 0.2f, 0.2f));
  // Define a directional diffuse light source.
  Visuals_Program_bindInteger32(state, g_program, "g_lights[1].type", 8);
  Visuals_Program_bindVector3F32(state, g_program, "g_lights[1].direction", Vector3F32_create(state, -1.f, -1.f, -1.f));
  Visuals_Program_bindVector3F32(state, g_program, "g_lights[1].color", Vector3F32_create(state, 0.4f, 0.4f, 0.4f));
  // Define a positional specular light source.
  Visuals_Program_bindInteger32(state, g_program, "g_lights[2].type", 64);
  Visuals_Program_bindVector3F32(state, g_program, "g_lights[2].direction", Vector3F32_create(state, -1.f, -1.f, -1.f));
  Visuals_Program_bindVector3F32(state, g_program, "g_lights[2].color", Vector3F32_create(state, 0.8f, 0.8f, 0.8f));
  Visuals_Program_bindVector3F32(state, g_program, "g_lights[2].position", Vector3F32_create(state, 0.f, 0.f, 0.f));

  for (size_t i = 0, n = Shizu_List_getSize(state, g_world->geometries); i < n; ++i) {
    Shizu_Value elementValue = Shizu_List_getValue(state, g_world->geometries, i);
    StaticGeometry *element = (StaticGeometry*)Shizu_Value_getObject(&elementValue);
    // Bind materials.
    for (size_t i = 0, n = Shizu_List_getSize(state, element->materials); i < n; ++i) {
      Shizu_Value elementValue = Shizu_List_getValue(state, element->materials, i);
      Visuals_Material* element = (Visuals_Material*)Shizu_Value_getObject(&elementValue);
      bindMaterial(state, visualsContext, g_program, element);
    }
    // Bind vertices.
    switch (element->vertexBuffer->flags) {
      case (Visuals_VertexSemantics_PositionXyz | Visuals_VertexSyntactics_Float3): {
        Visuals_Program_bindInteger32(state, g_program, "vertexDescriptor", Visuals_VertexSemantics_PositionXyz);
      } break;
      case (Visuals_VertexSemantics_PositionXyz_NormalXyz_AmbientRgb | Visuals_VertexSyntactics_Float3_Float3_Float3): {
        Visuals_Program_bindInteger32(state, g_program, "vertexDescriptor", Visuals_VertexSemantics_PositionXyz_NormalXyz_AmbientRgb);
      } break;
      case (Visuals_VertexSemantics_PositionXyz_NormalXyz_AmbientRgb_DiffuseRgb_SpecularRgb_Shininess | Visuals_VertexSyntactics_Float3_Float3_Float3_Float3_Float3_Float): {
        Visuals_Program_bindInteger32(state, g_program, "vertexDescriptor", Visuals_VertexSemantics_PositionXyz_NormalXyz_AmbientRgb_DiffuseRgb_SpecularRgb_Shininess);
      } break;
      default: {
        fprintf(stderr, "%s:%d: unreachable code reached\n", __FILE__, __LINE__);
        Shizu_State2_setStatus(state, Shizu_Status_ArgumentInvalid);
        Shizu_State2_jump(state);
      } break;
    };
    Visuals_Context_render(state, visualsContext, element->vertexBuffer, g_program);
  }

  Visuals_Service_endFrame(state);
}

static void
onKeyboardKeyMessage
  (
    Shizu_State2* state
  )
{
  if (Shizu_Stack_getSize(Shizu_State2_getState1(state), Shizu_State2_getStack(state)) < 2) {
    fprintf(stderr, "%s:%d: too few arguments\n", __FILE__, __LINE__);
    Shizu_State2_setStatus(state, 1);
    Shizu_State2_jump(state);
  }
  if (!Shizu_Stack_isInteger32(Shizu_State2_getState1(state), Shizu_State2_getStack(state), 0)) {
    fprintf(stderr, "%s:%d: invalid argument type\n", __FILE__, __LINE__);
    Shizu_State2_setStatus(state, 1);
    Shizu_State2_jump(state);
  }
  if (1 != Shizu_Stack_getInteger32(Shizu_State2_getState1(state), Shizu_State2_getStack(state), 0)) {
    fprintf(stderr, "%s:%d: invalid number of arguments\n", __FILE__, __LINE__);
    Shizu_State2_setStatus(state, 1);
    Shizu_State2_jump(state);
  }
  if (!Shizu_Stack_isObject(Shizu_State2_getState1(state), Shizu_State2_getStack(state), 1)) {
    fprintf(stderr, "%s:%d: invalid argument type\n", __FILE__, __LINE__);
    Shizu_State2_setStatus(state, 1);
    Shizu_State2_jump(state);
  }
  fprintf(stdout, "%s:%d: keyboard key message received\n", __FILE__, __LINE__);
  KeyboardKeyMessage* message = (KeyboardKeyMessage*)Shizu_Stack_getObject(Shizu_State2_getState1(state), Shizu_State2_getStack(state), 1);
  if (KeyboardKey_Escape == KeyboardKeyMessage_getKey(state, message)) {
    if (KeyboardKey_Action_Released == KeyboardKeyMessage_getAction(state, message)) {
      Zeitgeist_UpstreamRequest* request = Zeitgeist_UpstreamRequest_createExitProcessRequest(state);
      Zeitgeist_sendUpstreamRequest(state, request);
    }
  } else if (KeyboardKey_L == KeyboardKeyMessage_getKey(state, message)) {
    if (KeyboardKey_Action_Released == KeyboardKeyMessage_getAction(state, message)) {
      switch (g_lightModel) {
        case LightModel_Phong: {
          g_lightModel = LightModel_BlinnPhong;
        } break;
        default: {
          g_lightModel = LightModel_Phong;
        } break;
      }
    }
  } else {
    Player_onKeyboardKeyMessage(state, g_world->player, message);
  }
  Shizu_Stack_pop(Shizu_State2_getState1(state), Shizu_State2_getStack(state));
  Shizu_Stack_pop(Shizu_State2_getState1(state), Shizu_State2_getStack(state));
}

Shizu_Rendition_Export void
Zeitgeist_Rendition_load
  (
    Shizu_State2* state
  )
{
  Visuals_Service_startup(state);
  Visuals_Service_setTitle(state, Shizu_String_create(state, "Room (OpenGL)", strlen("Room (OpenGL)")));

  Shizu_Value temporary;
  Shizu_Value_setCxxFunction(&temporary, &onKeyboardKeyMessage);
  Visuals_Service_addKeyboardKeyCallback(state, &temporary);

  Shizu_JumpTarget jumpTarget;
  Shizu_State2_pushJumpTarget(state, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    Visuals_Context* visualsContext = (Visuals_Context*)Visuals_Gl_Context_create(state);
    Visuals_Program* program = Visuals_getProgram(state, "pbr1");
    Visuals_Object_materialize(state, (Visuals_Object*)program);
    Shizu_Object_lock(Shizu_State2_getState1(state), Shizu_State2_getLocks(state), (Shizu_Object*)program);
    g_program = program;
    World* world = World_create(state, visualsContext);
    Shizu_Object_lock(Shizu_State2_getState1(state), Shizu_State2_getLocks(state), (Shizu_Object*)world);
    g_world = world;
    Visuals_RenderBuffer* renderBuffer = Visuals_Context_createRenderBuffer(state, visualsContext);
    Shizu_Object_lock(Shizu_State2_getState1(state), Shizu_State2_getLocks(state), (Shizu_Object*)renderBuffer);
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

    Visuals_Context_setCullMode(state, visualsContext, Visuals_CullMode_Back);
    Visuals_Context_setDepthFunction(state, visualsContext, Visuals_DepthFunction_LessThanOrEqualTo);

    Shizu_State2_popJumpTarget(state);
  } else {
    Shizu_State2_popJumpTarget(state);
    if (g_renderBuffer) {
      Visuals_Object_unmaterialize(state, (Visuals_Object*)g_renderBuffer);
      Shizu_Object_unlock(Shizu_State2_getState1(state), Shizu_State2_getLocks(state), (Shizu_Object*)g_renderBuffer);
      g_renderBuffer = NULL;
    }
    if (g_world) {
      Shizu_Object_unlock(Shizu_State2_getState1(state), Shizu_State2_getLocks(state), (Shizu_Object*)g_world);
      g_world = NULL;
    }
    if (g_program) {
      Visuals_Object_unmaterialize(state, (Visuals_Object*)g_program);
      Shizu_Object_unlock(Shizu_State2_getState1(state), Shizu_State2_getLocks(state), (Shizu_Object*)g_program);
      g_program = NULL;
    }
    Shizu_State2_jump(state);
  }
}

Shizu_Rendition_Export void
Zeitgeist_Rendition_unload
  (
    Shizu_State2* state
  )
{
  for (size_t i = 0, n = Shizu_List_getSize(state, g_world->geometries); i < n; ++i) {
    Shizu_Value elementValue = Shizu_List_getValue(state, g_world->geometries, i);
    StaticGeometry* element = (StaticGeometry*)Shizu_Value_getObject(&elementValue);
    Shizu_JumpTarget jumpTarget;
    Shizu_State2_pushJumpTarget(state, &jumpTarget);
    if (!setjmp(jumpTarget.environment)) {
      StaticGeometry_unmaterialize(state, element);
      Shizu_State2_popJumpTarget(state);
    } else {
      Shizu_State2_popJumpTarget(state);
    }
  }
  if (g_renderBuffer) {
    Visuals_Object_unmaterialize(state, (Visuals_Object*)g_renderBuffer);
    Shizu_Object_unlock(Shizu_State2_getState1(state), Shizu_State2_getLocks(state), (Shizu_Object*)g_renderBuffer);
    g_renderBuffer = NULL;
  }
  if (g_world) {
    Shizu_Object_unlock(Shizu_State2_getState1(state), Shizu_State2_getLocks(state), (Shizu_Object*)g_world);
    g_world = NULL;
  }
  if (g_program) {
    Visuals_Object_unmaterialize(state, (Visuals_Object*)g_program);
    Shizu_Object_unlock(Shizu_State2_getState1(state), Shizu_State2_getLocks(state), (Shizu_Object*)g_program);
    g_program = NULL;
  }
  Visuals_Service_shutdown(state);
}
