// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#include "ServiceGl.h"

// malloc, free
#include <malloc.h>

// fprintf, stdio
#include <stdio.h>

// exit, EXIT_FAILURE
#include <stdlib.h>

// strlen
#include <string.h>

#if Shizu_Configuration_OperatingSystem_Windows == Shizu_Configuration_OperatingSystem
  #include "Visuals/Gl/Wgl/Service.h"
  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>
  #include <GL/gl.h>
#elif Shizu_Configuration_OperatingSystem_Linux == Shizu_Configuration_OperatingSystem
  #include "Visuals/Gl/Glx/ServiceGlx.h"
  #include <GL/gl.h>
#else
  #error("operating system not (yet) supported")
#endif

#include <GL/glext.h>

#define Define(Type, Name) \
  Type Name = NULL;
#include "ServiceGl_Functions.i"
#undef Define

typedef struct Visuals_Gl_Service {
  /// The reference count.
  Shizu_Integer32 referenceCount;
  /// List of weak references to Visuals.Object values.
  /// Used to notify the Visuals.Object values to release their resources before this service shuts down.
  Shizu_List* objects;
} Visuals_Gl_Service;

static Visuals_Gl_Service g_service = {
    .referenceCount = 0,
    .objects = NULL,
  };

static void*
link
  (
    Shizu_State2* state,
    char const* functionName,
    char const* extensionName
  )
{
#if Shizu_Configuration_OperatingSystem_Windows == Shizu_Configuration_OperatingSystem
  return Visuals_Gl_Wgl_Service_link(state, functionName, extensionName);
#elif Shizu_Configuration_OperatingSystem_Linux == Shizu_Configuration_OperatingSystem
  return ServiceGlx_link(state, functionName, extensionName);
#else
  #error("operating system not (yet) supported")
#endif
}

void
Visuals_Gl_Service_startup
  (
    Shizu_State2* state
  )
{
  if (g_service.referenceCount == 0) {
  #if Shizu_Configuration_OperatingSystem_Windows == Shizu_Configuration_OperatingSystem
    Visuals_Gl_Wgl_Service_startup(state);
  #elif Shizu_Configuration_OperatingSystem_Linux == Shizu_Configuration_OperatingSystem
    ServiceGlx_startup(state);
  #else
    #error("operating system not (yet) supported")
  #endif
  #define Define(Type, Name) \
    Name = (Type)link(state, #Name, NULL);
  #include "ServiceGl_Functions.i"
  #undef Define
  }
  g_service.referenceCount++;
}

void
Visuals_Gl_Service_shutdown
  (
    Shizu_State2* state
  )
{
  if (0 == --g_service.referenceCount) {
    if (g_service.objects) {
      Shizu_Object_unlock(Shizu_State2_getState1(state), Shizu_State2_getLocks(state), (Shizu_Object*)g_service.objects);
      g_service.objects = NULL;
    }
  #if Shizu_Configuration_OperatingSystem_Windows == Shizu_Configuration_OperatingSystem
    Visuals_Gl_Wgl_Service_shutdown(state);
  #elif Shizu_Configuration_OperatingSystem_Linux == Shizu_Configuration_OperatingSystem
    ServiceGlx_shutdown(state);
  #else
    #error("operating system not (yet) supported")
  #endif
  }
}

void
Visuals_Gl_Service_setTitle
  (
    Shizu_State2* state,
    Shizu_String* title
  )
{
#if Shizu_Configuration_OperatingSystem_Windows == Shizu_Configuration_OperatingSystem
  Visuals_Gl_Wgl_Service_setTitle(state, title);
#elif Shizu_Configuration_OperatingSystem_Linux == Shizu_Configuration_OperatingSystem
  ServiceGlx_setTitle(state, title);
#else
  #error("operating system not (yet) supported")
#endif
}

void
Visuals_Gl_Service_getClientSize
  (
    Shizu_State2* state,
    Shizu_Integer32* width,
    Shizu_Integer32* height
  )
{
#if Shizu_Configuration_OperatingSystem_Windows == Shizu_Configuration_OperatingSystem
  Visuals_Gl_Wgl_Service_getClientSize(state, width, height);
#elif Shizu_Configuration_OperatingSystem_Linux == Shizu_Configuration_OperatingSystem
  ServiceGlx_getClientSize(state, width, height);
#else
  #error("operating system not (yet) supported")
#endif
}

void
Visuals_Gl_Service_beginFrame
  (
    Shizu_State2* state
  )
{
#if Shizu_Configuration_OperatingSystem_Windows == Shizu_Configuration_OperatingSystem
  Visuals_Gl_Wgl_Service_beginFrame(state);
#elif Shizu_Configuration_OperatingSystem_Linux == Shizu_Configuration_OperatingSystem
  ServiceGlx_beginFrame(state);
#else
  #error("operating system not (yet) supported")
#endif
}

void
Visuals_Gl_Service_endFrame
  (
    Shizu_State2* state
  )
{
#if Shizu_Configuration_OperatingSystem_Windows == Shizu_Configuration_OperatingSystem
  Visuals_Gl_Wgl_Service_endFrame(state);
#elif Shizu_Configuration_OperatingSystem_Linux == Shizu_Configuration_OperatingSystem
  ServiceGlx_endFrame(state);
#else
  #error("operating system not (yet) supported")
#endif
}

void
Visuals_Gl_Service_update
  (
    Shizu_State2* state
  )
{
#if Shizu_Configuration_OperatingSystem_Windows == Shizu_Configuration_OperatingSystem
  Visuals_Gl_Wgl_Service_update(state);
#elif Shizu_Configuration_OperatingSystem_Linux == Shizu_Configuration_OperatingSystem
  ServiceGlx_update(state);
#else
  #error("operating system not (yet) supported")
#endif
}

Shizu_Boolean
Visuals_Gl_Service_quitRequested
  (
    Shizu_State2* state
  )
{
#if Shizu_Configuration_OperatingSystem_Windows == Shizu_Configuration_OperatingSystem
  return Visuals_Gl_Wgl_Service_quitRequested(state);
#elif Shizu_Configuration_OperatingSystem_Linux == Shizu_Configuration_OperatingSystem
  return ServiceGlx_quitRequested(state);
#else
  #error("operating system not (yet) supported")
#endif
}

GLuint
Visuals_Gl_Service_compileShader
  (
    Shizu_State2* state,
    GLenum type,
    const GLchar *source
  )
{
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);
  GLint param;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &param);
  if (!param) {
    GLchar log[4096];
    glGetShaderInfoLog(shader, sizeof(log), NULL, log);
    fprintf(stderr, "error: %s: %s\n",
            type == GL_FRAGMENT_SHADER ? "frag" : "vert", (char *) log);
    Shizu_State2_setStatus(state, 1);
    Shizu_State2_jump(state);
  }
  return shader;
}

GLuint
Visuals_Gl_Service_linkProgram
  (
    Shizu_State2* state,
    GLuint vert,
    GLuint frag
  )
{
  GLuint program = glCreateProgram();
  glAttachShader(program, vert);
  glAttachShader(program, frag);
  glLinkProgram(program);
  GLint param;
  glGetProgramiv(program, GL_LINK_STATUS, &param);
  if (!param) {
    GLchar log[4096];
    glGetProgramInfoLog(program, sizeof(log), NULL, log);
    fprintf(stderr, "error: link: %s\n", (char *) log);
    Shizu_State2_setStatus(state, 1);
    Shizu_State2_jump(state);
  }
  return program;
}

void
Visuals_Gl_Service_registerVisualsObject
  (
    Shizu_State2* state,
    Visuals_Object* object
  )
{
  if (!g_service.objects) {
    g_service.objects = Shizu_List_create(state);
    Shizu_JumpTarget jumpTarget;
    Shizu_State2_pushJumpTarget(state, &jumpTarget);
    if (!setjmp(jumpTarget.environment)) {
      Shizu_Object_lock(Shizu_State2_getState1(state), Shizu_State2_getLocks(state), (Shizu_Object*)g_service.objects);
      Shizu_State2_popJumpTarget(state);
    } else {
      Shizu_State2_popJumpTarget(state);
      g_service.objects = NULL;
      Shizu_State2_jump(state);
    }   
  }
  Shizu_Value temporary;
  Shizu_Value_setObject(&temporary, (Shizu_Object*)Shizu_WeakReference_create(state, (Shizu_Object*)object));
  Shizu_List_appendValue(state, g_service.objects, &temporary);
}


Shizu_String*
Visuals_Gl_Service_getBackendVendorName
  (
    Shizu_State2* state
  )
{
  GLubyte const* p = glGetString(GL_VENDOR);
  return Shizu_String_create(state, p, strlen(p));
}

Shizu_String*
Visuals_Gl_Service_getBackendRendererName
  (
    Shizu_State2* state
  )
{
  GLubyte const* p = glGetString(GL_RENDERER);
  return Shizu_String_create(state, p, strlen(p));
}

Shizu_Integer32
Visuals_Gl_Service_getBackendMajorVersion
  (
    Shizu_State2* state
  )
{
  GLint v;
  glGetIntegerv(GL_MAJOR_VERSION, &v);
  if (v < Shizu_Integer32_Minimum || v > Shizu_Integer32_Maximum) {
    Shizu_State2_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State2_jump(state);
  }
  return v;
}

Shizu_Integer32
Visuals_Gl_Service_getBackendMinorVersion
  (
    Shizu_State2* state
  )
{
  GLint v;
  glGetIntegerv(GL_MINOR_VERSION, &v);
  if (v < Shizu_Integer32_Minimum || v > Shizu_Integer32_Maximum) {
    Shizu_State2_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State2_jump(state);
  }
  return v;
}