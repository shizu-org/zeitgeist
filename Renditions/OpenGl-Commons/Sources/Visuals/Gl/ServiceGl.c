// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#include "ServiceGl.h"

// malloc
#include <malloc.h>

// fprintf, stdio
#include <stdio.h>

// exit, EXIT_FAILURE
#include <stdlib.h>

#if Shizu_Configuration_OperatingSystem_Windows == Shizu_Configuration_OperatingSystem
  #include "Visuals/Gl/Wgl/ServiceWgl.h"
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

/**
 * @brief A list of weak references to Visuals.Object values.
 * Before this Visuals.Service shuts down, it notifies all these objects to release their resources.
 */
static Shizu_List* g_objects = NULL;

/**
 * @brief A list of Shizu_CxxFunction values.
 */
static Shizu_List* g_keyboardKeyListeners = NULL;

static Shizu_Integer32 g_referenceCount = 0;

static void*
link
  (
    Shizu_State* state,
    char const* functionName,
    char const* extensionName
  )
{
#if Shizu_Configuration_OperatingSystem_Windows == Shizu_Configuration_OperatingSystem
  return ServiceWgl_link(state, functionName, extensionName);
#elif Shizu_Configuration_OperatingSystem_Linux == Shizu_Configuration_OperatingSystem
  return ServiceGlx_link(state, functionName, extensionName);
#else
  #error("operating system not (yet) supported")
#endif
}

void
Visuals_ServiceGl_startup
  (
    Shizu_State* state
  )
{
  if (g_referenceCount == 0) {
  #if Shizu_Configuration_OperatingSystem_Windows == Shizu_Configuration_OperatingSystem
    ServiceWgl_startup(state);
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
  g_referenceCount++;
}

void
Visuals_ServiceGl_shutdown
  (
    Shizu_State* state
  )
{
  if (0 == --g_referenceCount) {
    if (g_objects) {
      Shizu_Object_unlock(Shizu_State_getState1(state), Shizu_State_getLocks(state), (Shizu_Object*)g_objects);
      g_objects = NULL;
    }
    if (g_keyboardKeyListeners) {
      Shizu_Object_unlock(Shizu_State_getState1(state), Shizu_State_getLocks(state), (Shizu_Object*)g_keyboardKeyListeners);
      g_keyboardKeyListeners = NULL;
    }
  #if Shizu_Configuration_OperatingSystem_Windows == Shizu_Configuration_OperatingSystem
    ServiceWgl_shutdown(state);
  #elif Shizu_Configuration_OperatingSystem_Linux == Shizu_Configuration_OperatingSystem
    ServiceGlx_shutdown(state);
  #else
    #error("operating system not (yet) supported")
  #endif
  }
}

void
Visuals_ServiceGl_setTitle
  (
    Shizu_State* state,
    Shizu_String* title
  )
{
#if Shizu_Configuration_OperatingSystem_Windows == Shizu_Configuration_OperatingSystem
  ServiceWgl_setTitle(state, title);
#elif Shizu_Configuration_OperatingSystem_Linux == Shizu_Configuration_OperatingSystem
  ServiceGlx_setTitle(state, title);
#else
  #error("operating system not (yet) supported")
#endif
}

void
Visuals_ServiceGl_getClientSize
  (
    Shizu_State* state,
    Shizu_Integer32* width,
    Shizu_Integer32* height
  )
{
#if Shizu_Configuration_OperatingSystem_Windows == Shizu_Configuration_OperatingSystem
  ServiceWgl_getClientSize(state, width, height);
#elif Shizu_Configuration_OperatingSystem_Linux == Shizu_Configuration_OperatingSystem
  ServiceGlx_getClientSize(state, width, height);
#else
  #error("operating system not (yet) supported")
#endif
}

void
Visuals_ServiceGl_beginFrame
  (
    Shizu_State* state
  )
{
#if Shizu_Configuration_OperatingSystem_Windows == Shizu_Configuration_OperatingSystem
  ServiceWgl_beginFrame(state);
#elif Shizu_Configuration_OperatingSystem_Linux == Shizu_Configuration_OperatingSystem
  ServiceGlx_beginFrame(state);
#else
  #error("operating system not (yet) supported")
#endif
}

void
Visuals_ServiceGl_endFrame
  (
    Shizu_State* state
  )
{
#if Shizu_Configuration_OperatingSystem_Windows == Shizu_Configuration_OperatingSystem
  ServiceWgl_endFrame(state);
#elif Shizu_Configuration_OperatingSystem_Linux == Shizu_Configuration_OperatingSystem
  ServiceGlx_endFrame(state);
#else
  #error("operating system not (yet) supported")
#endif
}

void
Visuals_ServiceGl_update
  (
    Shizu_State* state
  )
{
#if Shizu_Configuration_OperatingSystem_Windows == Shizu_Configuration_OperatingSystem
  ServiceWgl_update(state);
#elif Shizu_Configuration_OperatingSystem_Linux == Shizu_Configuration_OperatingSystem
  ServiceGlx_update(state);
#else
  #error("operating system not (yet) supported")
#endif
}

Shizu_Boolean
Visuals_ServiceGl_quitRequested
  (
    Shizu_State* state
  )
{
#if Shizu_Configuration_OperatingSystem_Windows == Shizu_Configuration_OperatingSystem
  return ServiceWgl_quitRequested(state);
#elif Shizu_Configuration_OperatingSystem_Linux == Shizu_Configuration_OperatingSystem
  return ServiceGlx_quitRequested(state);
#else
  #error("operating system not (yet) supported")
#endif
}

GLuint
Visuals_ServiceGl_compileShader
  (
    Shizu_State* state,
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
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  return shader;
}

GLuint
Visuals_ServiceGl_linkProgram
  (
    Shizu_State* state,
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
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  return program;
}

void
Visuals_ServiceGl_registerVisualsObject
  (
    Shizu_State* state,
    Visuals_Object* object
  )
{
  if (!g_objects) {
    g_objects = Shizu_List_create(state);
    Shizu_JumpTarget jumpTarget;
    Shizu_State_pushJumpTarget(state, &jumpTarget);
    if (!setjmp(jumpTarget.environment)) {
      Shizu_Object_lock(Shizu_State_getState1(state), Shizu_State_getLocks(state), (Shizu_Object*)g_objects);
      Shizu_State_popJumpTarget(state);
    } else {
      Shizu_State_popJumpTarget(state);
      g_objects = NULL;
      Shizu_State_jump(state);
    }   
  }
  Shizu_Value temporary;
  Shizu_Value_setObject(&temporary, (Shizu_Object*)Shizu_WeakReference_create(state, (Shizu_Object*)object));
  Shizu_List_appendValue(state, g_objects, &temporary);
}

void
Visuals_ServiceGl_emitKeyboardKeyMessage
  (
    Shizu_State* state,
    KeyboardKeyMessage* message
  )
{
  if (!g_keyboardKeyListeners) {
    return;  
  }
  Shizu_Value temporary = Shizu_List_getSize(state, g_keyboardKeyListeners);
  for (size_t i = 0, n = Shizu_Value_getInteger32(&temporary); i < n; ++i) {
    temporary = Shizu_List_getValue(state, g_keyboardKeyListeners, i);
    if (Shizu_Value_isCxxFunction(&temporary)) {
      Shizu_CxxFunction* cxxFunction = Shizu_Value_getCxxFunction(&temporary);
      Shizu_Stack_pushObject(state, (Shizu_Object*)message);
      Shizu_Stack_pushInteger32(state, 1);
      (*cxxFunction)(state);
    } else {
      fprintf(stderr, "%s:%d: unreachable code reached\n", __FILE__, __LINE__);
      exit(EXIT_FAILURE);
    }
  }
}

void
Visuals_ServiceGl_addKeyboardKeyCallback
  (
    Shizu_State* state,
    Shizu_Value* value
  )
{
  if (!g_keyboardKeyListeners) {
    g_keyboardKeyListeners = Shizu_List_create(state);
    Shizu_JumpTarget jumpTarget;
    Shizu_State_pushJumpTarget(state, &jumpTarget);
    if (!setjmp(jumpTarget.environment)) {
      Shizu_Object_lock(Shizu_State_getState1(state), Shizu_State_getLocks(state), (Shizu_Object*)g_keyboardKeyListeners);
      Shizu_State_popJumpTarget(state);
    } else {
      Shizu_State_popJumpTarget(state);
      g_keyboardKeyListeners = NULL;
      Shizu_State_jump(state);
    }
  }
  if (Shizu_Value_isVoid(value)) {
    return;
  } else if (Shizu_Value_isCxxFunction(value)) {
    Shizu_List_appendValue(state, g_keyboardKeyListeners, value);
  } else if (Shizu_Value_isObject(value)) {
    Shizu_Value temporary;
    Shizu_Value_setObject(&temporary, (Shizu_Object*)Shizu_WeakReference_create(state, (Shizu_Object*)Shizu_Value_getObject(value)));
    Shizu_List_appendValue(state, g_keyboardKeyListeners, &temporary);
  } else {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
}
