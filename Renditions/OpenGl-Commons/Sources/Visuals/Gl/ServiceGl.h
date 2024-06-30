/*
  Shizu Visuals
  Copyright (C) 2024 Michael Heilmann. All rights reserved.

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/


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

/// @since 0.1
/// @brief Initialize the GL service.
/// @remarks The GL service is shared between renditions.
/// The service is reference counted.
/// Each time a rendition calls ServiceGL_startup successfully,
/// the reference count of the service is incremented by @a 1.
/// Each time a rendition calls ServiceGL_shutdown successfully,
/// the reference count of the service is decremented by @a 1.
/// If the reference count increments from zero to one, the service is created.
/// If the reference count decrements from one to zero, the service is destroyed.
void
Visuals_Gl_Service_startup
  (
    Shizu_State2* state
  );

void
Visuals_Gl_Service_shutdown
  (
    Shizu_State2* state
  );

void
Visuals_Gl_Service_setTitle
  (
    Shizu_State2* state,
    Shizu_String* title
  );

void
Visuals_Gl_Service_getClientSize
  (
    Shizu_State2* state,
    Shizu_Integer32* width,
    Shizu_Integer32* height
  );

void
Visuals_Gl_Service_beginFrame
  (
    Shizu_State2* state
  );

void
Visuals_Gl_Service_endFrame
  (
    Shizu_State2* state
  );

void
Visuals_Gl_Service_update
  (
    Shizu_State2* state
  );

Shizu_Boolean
Visuals_Gl_Service_quitRequested
  (
    Shizu_State2* state
  );

GLuint
Visuals_Gl_Service_compileShader
  (
    Shizu_State2* state,
    GLenum type,
    const GLchar *source
  );

GLuint
Visuals_Gl_Service_linkProgram
  (
    Shizu_State2* state,
    GLuint vert,
    GLuint frag
  );

void
Visuals_Gl_Service_registerVisualsObject
  (
    Shizu_State2* state,
    Visuals_Object* object
  );

Shizu_String*
Visuals_Gl_Service_getBackendVendorName
  (
    Shizu_State2* state
  );

Shizu_String*
Visuals_Gl_Service_getBackendRendererName
  (
    Shizu_State2* state
  );

Shizu_Integer32
Visuals_Gl_Service_getBackendMajorVersion
  (
    Shizu_State2* state
  );

Shizu_Integer32
Visuals_Gl_Service_getBackendMinorVersion
  (
    Shizu_State2* state
  );

#endif // SERVICEGL_H_INCLUDED
