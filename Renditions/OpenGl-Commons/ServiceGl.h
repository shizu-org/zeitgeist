// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#if !defined(SERVICEGL_H_INCLUDED)
#define SERVICEGL_H_INCLUDED

#include "Zeitgeist.h"

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
ServiceGl_startup
  (
    Shizu_State* state
  );

void
ServiceGl_shutdown
  (
    Shizu_State* state
  );

void
ServiceGl_setTitle
  (
    Shizu_State* state,
    Shizu_String* title
  );

void
ServiceGl_getClientSize
  (
    Shizu_State* state,
    Shizu_Integer32* width,
    Shizu_Integer32* height
  );

void
ServiceGl_beginFrame
  (
    Shizu_State* state
  );

void
ServiceGl_endFrame
  (
    Shizu_State* state
  );

void
ServiceGl_update
  (
    Shizu_State* state
  );

Shizu_Boolean
ServiceGl_quitRequested
  (
    Shizu_State* state
  );

GLuint
ServiceGl_compileShader
  (
    Shizu_State* state,
    GLenum type,
    const GLchar *source
  );

GLuint
ServiceGl_linkProgram
  (
    Shizu_State* state,
    GLuint vert,
    GLuint frag
  );

typedef struct KeyboardKeyMessage KeyboardKeyMessage;

/**
 * @since 0.1
 * @brief Emit a keyboard key message.
 * @param message A pointer to the KeyboardKeyMessage object.
 */
void
ServiceGl_emitKeyboardKeyMessage
  (
    Shizu_State* state,
    KeyboardKeyMessage* message
  );

/**
 * @since 0.1
 * @brief Add a keyboard key callback.
 * @param state A pointer to the Shizu_State.
 * @param value The value to be used as a callback.
 * @remarks
 * - If the value stores a Shizu_CxxFunction* value,
 *   then this Shizu_CxxFunction* is added to the callback list.
 *   If Zeitgeist_KeyboardKeyMessage is dispatched,
 *   then the Shizu_CxxFunction* is invoked with the KeyboardKeyMessage* value as its single argument.
 * - If the value stores a Shizu_Object value,
 *   then this object is wrapped into a weak reference and the weak reference is added to the callback list.
 *   If a Zeitgeist_KeyboardKeyMessage is dispatched,
 *   then the the Shizu_Object value in the weak reference is retrieved.
 *   If it expired, then the weak reference is removed from the callback list.
 *   Otherwise the Shizu_Object's "call" metamethod is invoked with
 *   the Shizu_Object as the first argument
 *   and
 *   the KeyboardKeyMessage object as the second argument.
 * - If the value stores a Shizu_Void value,
 *   then nothing is added to the callback list.
 * - If value stores any other value than the above,
 *   an error is raised.
 */
void
ServiceGl_addKeyboardKeyCallback
  (
    Shizu_State* state,
    Shizu_Value* value
  );

#endif // SERVICEGL_H_INCLUDED
