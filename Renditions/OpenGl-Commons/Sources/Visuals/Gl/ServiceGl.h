// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#if !defined(SERVICEGL_H_INCLUDED)
#define SERVICEGL_H_INCLUDED

#include "Zeitgeist.h"

/* Forward declaration. */
typedef struct KeyboardKeyMessage KeyboardKeyMessage;

/* Forward declaration. */
typedef struct Visuals_Object Visuals_Object;

#if Shizu_Configuration_OperatingSystem_Windows == Shizu_Configuration_OperatingSystem
  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>
  #include <GL/gl.h>
#elif Shizu_Configuration_OperatingSystem_Linux == Shizu_Configuration_OperatingSystem
  #include <GL/gl.h>
#endif

#include <GL/glext.h>

#define Define(Type, Name) \
  extern Type Name;
#include "ServiceGl_Functions.i"
#undef Define

/**
 * @since 0.1
 * @brief Initialize the GL service.
 * @remarks The GL service is shared between renditions.
 * The service is reference counted.
 * Each time a rendition calls ServiceGL_startup successfully,
 * the reference count of the service is incremented by @a 1.
 * Each time a rendition calls ServiceGL_shutdown successfully,
 * the reference count of the service is decremented by @a 1.
 * If the reference count increments from zero to one, the service is created.
 * If the reference count decrements from one to zero, the service is destroyed.
 */
void
Visuals_ServiceGl_startup
  (
    Shizu_State* state
  );

void
Visuals_ServiceGl_shutdown
  (
    Shizu_State* state
  );

void
Visuals_ServiceGl_setTitle
  (
    Shizu_State* state,
    Shizu_String* title
  );

void
Visuals_ServiceGl_getClientSize
  (
    Shizu_State* state,
    Shizu_Integer32* width,
    Shizu_Integer32* height
  );

void
Visuals_ServiceGl_beginFrame
  (
    Shizu_State* state
  );

void
Visuals_ServiceGl_endFrame
  (
    Shizu_State* state
  );

void
Visuals_ServiceGl_update
  (
    Shizu_State* state
  );

Shizu_Boolean
Visuals_ServiceGl_quitRequested
  (
    Shizu_State* state
  );

GLuint
Visuals_ServiceGl_compileShader
  (
    Shizu_State* state,
    GLenum type,
    const GLchar *source
  );

GLuint
Visuals_ServiceGl_linkProgram
  (
    Shizu_State* state,
    GLuint vert,
    GLuint frag
  );

void
Visuals_ServiceGl_registerVisualsObject
  (
    Shizu_State* state,
    Visuals_Object* object
  );

void
Visuals_ServiceGl_emitKeyboardKeyMessage
  (
    Shizu_State* state,
    KeyboardKeyMessage* message
  );

void
Visuals_ServiceGl_addKeyboardKeyCallback
  (
    Shizu_State* state,
    Shizu_Value* value
  );

#endif // SERVICEGL_H_INCLUDED
