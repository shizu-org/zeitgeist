// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#if !defined(SERVICEWGL_H_INCLUDED)
#define SERVICEWGL_H_INCLUDED

#include "Zeitgeist.h"

/// @since 0.1
/// @brief Startup the WGL service.
/// @param state A pointer to the Shizu_State object.
void
ServiceWgl_startup
	(
		Shizu_State2* state
	);

/// @since 0.1
/// @brief Shutdown the WGL service.
///@param state A pointer to the Shizu_State object.
void
ServiceWgl_shutdown
	(
		Shizu_State2* state
	);

/// @since 0.1
/// @brief Set the window title.
/// @param state A pointer to the Shizu_State object.
/// @param title A pointer to the title.
void
ServiceWgl_setTitle
	(
		Shizu_State2* state,
		Shizu_String* title
	);

/// @since 0.1
/// @brief Must be called in intervals to update.
/// @params tate A pointer to the Shizu_State object.
void
ServiceWgl_update
	(
		Shizu_State2* state
	);

/// @since 1.0
/// @brief Get if "quit" was requested.
/// @param state A pointer to the Shizu_State object.
/// @return @a true if "quit" was requested. @a false otherwise.
Shizu_Boolean
ServiceWgl_quitRequested
	(
		Shizu_State2* state
	);

/// @since 0.1
/// @brief Get the size, in pixels, of the client rectangle.
/// @param state A pointer to the Shizu_State object.
/// @param width A pointer to a Shizu_Integer32 variable.
/// If this function succeeds, this variable is assigned the client width.
/// @param height A pointer to a Shizu_Integer32 variable.
/// If this function succeeds, this variable is assigned the client height.
void
ServiceWgl_getClientSize
	(
		Shizu_State2* state,
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
ServiceWgl_link
	(
		Shizu_State2* state,
		char const* functionName,
		char const* extensionName
	);
	
void
ServiceWgl_beginFrame
	(
		Shizu_State2* state
	);

void
ServiceWgl_endFrame
	(
		Shizu_State2* state
	);

#endif // SERVICEWGL_H_INCLUDED
