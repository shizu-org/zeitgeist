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

#if !defined(VISUALS_GL_GLX_SERVICE_H_INCLUDED)
#define VISUALS_GL_GLX_SERVICE_H_INCLUDED

#include "Zeitgeist.h"

/// @since 0.1
/// @brief Startup the GLX service.
/// @param state A pointer to the Shizu_State2 value.
void
Visuals_Gl_Glx_Service_startup
  (
    Shizu_State2* state
  );

/// @since 0.1
/// @brief Shutdown the GLX service.
/// @param state A pointer to the Shizu_State2 value.
void
Visuals_Gl_Glx_Service_shutdown
  (
    Shizu_State2* state
  );

/// @since 0.1
/// @brief Set the window title.
/// @param state A pointer to the Shizu_State2 value.
/// @param title A pointer to the title.
void
Visuals_Gl_Glx_Service_setTitle
  (
    Shizu_State2* state,
    Shizu_String* title
  );

void
Visuals_Gl_Glx_Service_update
  (
    Shizu_State2* state
  );

Shizu_Boolean
Visuals_Gl_Glx_Service_quitRequested
  (
    Shizu_State2* state
  );

/**
 * @since 0.1
 * @brief Get the size, in pixels, of the client rectangle.
 * @param state A pointer to the Shizu_State2 value.
 * @param width A pointer to a Shizu_Integer32 variable.
 * If this function succeeds, this variable is assigned the client width.
 * @param height A pointer to a Shizu_Integer32 variable.
 * If this function succeeds, this variable is assigned the client height.
 */
void
Visuals_Gl_Glx_Service_getClientSize
  (
    Shizu_State2* state,
    Shizu_Integer32 *width,
    Shizu_Integer32 *height
  );

/**
 * @since 0.1
 * @brief Try to link the an OpenGL function.
 * @param state A pointer to the Shizu_State2 value.
 * @param functionName A pointer to the function name.
 * @param extensionName A pointer to the extension name or the null pointer.
 * @return A poiner to the function on success.
 */
void*
Visuals_Gl_Glx_Service_link
  (
    Shizu_State2* state,
    char const* functionName,
    char const* extensionName
  );

void
Visuals_Gl_Glx_Service_beginFrame
  (
    Shizu_State2* state
  );

void
Visuals_Gl_Glx_Service_endFrame
  (
    Shizu_State2* state
  );

void
Visuals_Gl_Glx_Service_addKeyboardKeyCallback
  (
    Shizu_State2* state
  );

#endif // VISUALS_GL_GLX_SERVCE_H_INCLUDED
