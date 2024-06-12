// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#if !defined(SERVICEGLX_H_INCLUDED)
#define SERVICEGLX_H_INCLUDED

#include "Zeitgeist.h"

/// @since 0.1
/// @brief Startup the GLX service.
/// @param state A pointer to the Shizu_State object.
void
ServiceGlx_startup
  (
    Shizu_State* state
  );

/// @since 0.1
/// @brief Shutdown the GLX service.
/// @param state A pointer to the Shizu_State object.
void
ServiceGlx_shutdown
  (
    Shizu_State* state
  );

/// @since 0.1
/// @brief Set the window title.
/// @param state A pointer to the Shizu_State object.
/// @param title A pointer to the title.
void
ServiceGlx_setTitle
  (
    Shizu_State* state,
    Shizu_String* title
  );

void
ServiceGlx_update
  (
    Shizu_State* state
  );

Shizu_Boolean
ServiceGlx_quitRequested
  (
    Shizu_State* state
  );

/**
 * @since 0.1
 * @brief Get the size, in pixels, of the client rectangle.
 * @param state A pointer to the Shizu_State object.
 * @param width A pointer to a Shizu_Integer32 variable.
 * If this function succeeds, this variable is assigned the client width.
 * @param height A pointer to a Shizu_Integer32 variable.
 * If this function succeeds, this variable is assigned the client height.
 */
void
ServiceGlx_getClientSize
  (
    Shizu_State* state,
    Shizu_Integer32 *width,
    Shizu_Integer32 *height
  );

/**
 * @since 0.1
 * @brief Try to link the an OpenGL function.
 * @param state A pointer to the Shizu_State object.
 * @param functionName A pointer to the function name.
 * @param extensionName A pointer to the extension name or the null pointer.
 * @return A poiner to the function on success.
 */
void*
ServiceGlx_link
  (
    Shizu_State* state,
    char const* functionName,
    char const* extensionName
  );

void
ServiceGlx_beginFrame
  (
    Shizu_State* state
  );

void
ServiceGlx_endFrame
  (
    Shizu_State* state
  );

void
ServiceGlx_addKeyboardKeyCallback
  (
    Shizu_State* state
  );

#endif // SERVICEGLX_H_INCLUDED
