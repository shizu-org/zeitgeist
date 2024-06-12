/*
  Zeitgeist
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

#if !defined(VISUALS_SERVICE_H_INCLUDED)
#define VISUALS_SERVICE_H_INCLUDED

#include "Zeitgeist.h"
// Forward declaration.
typedef struct KeyboardKeyMessage KeyboardKeyMessage;
// Forward declaration.
typedef struct MouseButtonMessage MouseButtonMessage;
// Forward declaration.
typedef struct MousePointerMessage MousePointerMessage;

/// @since 0.1
/// @brief Initialize the "Visuals" service.
/// @param state A pointer to a Shizu_State object.
/// @remarks The "Visuals" service is shared between renditions.
/// The service is reference counted.
/// Each time a rendition calls Visuals_Service_startup successfully, the reference count of the service is incremented by @a 1.
/// Each time a rendition calls Visuals_Service_shutdown successfully, the reference count of the service is decremented by @a 1.
/// If the reference count increments from zero to one, the service is created.
/// If the reference count decrements from one to zero, the service is destroyed.
void
Visuals_Service_startup
  (
    Shizu_State* state
  );

/// @since 1.0
/// @brief Uninitialize the "Visuals" service.
/// @param state A pointer to a Shizu_State object.
/// @see Visuals_Service_startup for more information.
void
Visuals_Service_shutdown
  (
    Shizu_State* state
  );
  
/// @since 1.0
/// @brief Set the title of the window.
/// @param state A pointer to a Shizu_State object.
/// @param title A pointer to a Shizu_String object, the title to be set.
void
Visuals_Service_setTitle
  (
    Shizu_State* state,
    Shizu_String* title
  );

/// @since 1.0
/// @brief Get the client size.
/// @param state A pointer to a Shizu_State object.
/// @param [out] width A pointer to a <code>Shizu_Integer32</code> variable.
/// @param [out] height A pointer to a <code>Shizu_Integer32</code> variable.
/// @success <code>*width</code> was assigned the client width. <code>*height</code> was assigned the client height.
void
Visuals_Service_getClientSize
  (
    Shizu_State* state,
    Shizu_Integer32* width,
    Shizu_Integer32* height
  );

/// @brief Must be invoked to begin rendering a frame.
/// @param state A pointer to a Shizu_State object.
void
Visuals_Service_beginFrame
  (
    Shizu_State* state
  );

/// @brief Must be invoked to end rendering a frame.
/// @param state A pointer to a Shizu_State object.
void
Visuals_Service_endFrame
  (
    Shizu_State* state
  );

/// @brief Must be invoked in intervals.
/// @param state A pointer to a Shizu_State object.
void
Visuals_Service_update
  (
    Shizu_State* state
  );

/// @param state A pointer to a Shizu_State object.
Shizu_Boolean
Visuals_Service_quitRequested
  (
    Shizu_State* state
  );

/// @since 0.1
/// @brief Emit a keyboard key message.
/// @param state A pointer to a Shizu_State object.
/// @param message A pointer to the KeyboardKeyMessage object.
void
Visuals_Service_emitKeyboardKeyMessage
  (
    Shizu_State* state,
    KeyboardKeyMessage* message
  );

/// @since 0.1
/// @brief Add a keyboard key callback.
/// @param state A pointer to a Shizu_State object.
/// @param value The value to be used as a callback.
/// @remarks
/// - If the value stores a Shizu_CxxFunction* value, then this Shizu_CxxFunction* is added to the callback list.
///   If a KeyboardKeyMessage/MouseButtonMessage/MousePointerMessage is dispatched, then the Shizu_CxxFunction* is invoked with the message object as its single argument.
/// - If the value stores a Shizu_Object value, then this object is wrapped into a weak reference and the weak reference is added to the callback list.
///   If a KeyboardKeyMessage/MouseButtonMessage/MousePointerMessage is dispatched, then the the Shizu_Object value in the weak reference is retrieved.
///   If it expired, then the weak reference is removed from the callback list.  Otherwise the Shizu_Object's "call" metamethod is invoked with the object as the first argument and the message object as the second argument.
/// - If the value stores a Shizu_Void value, then nothing is added to the callback list.
/// - If value stores any other value than the above, an error is raised.
void
Visuals_Service_addKeyboardKeyCallback
  (
    Shizu_State* state,
    Shizu_Value* value
  );

/// @since 0.1
/// @brief Add a mouse button callback.
/// @param state A pointer to a Shizu_State object.
/// @param value The value to be used as a callback.
/// @remarks
/// - If the value stores a Shizu_CxxFunction* value, then this Shizu_CxxFunction* is added to the callback list.
///   If a KeyboardKeyMessage/MouseButtonMessage/MousePointerMessage is dispatched, then the Shizu_CxxFunction* is invoked with the message object as its single argument.
/// - If the value stores a Shizu_Object value, then this object is wrapped into a weak reference and the weak reference is added to the callback list.
///   If a KeyboardKeyMessage/MouseButtonMessage/MousePointerMessage is dispatched, then the the Shizu_Object value in the weak reference is retrieved.
///   If it expired, then the weak reference is removed from the callback list.  Otherwise the Shizu_Object's "call" metamethod is invoked with the object as the first argument and the message object as the second argument.
/// - If the value stores a Shizu_Void value, then nothing is added to the callback list.
/// - If value stores any other value than the above, an error is raised.
void
Visuals_Service_addMouseButtonCallback
  (
    Shizu_State* state,
    Shizu_Value* value
  );

/// @since 0.1
/// @brief Add a mouse pointer callback.
/// @param state A pointer to a Shizu_State object.
/// @param value The value to be used as a callback.
/// @remarks
/// - If the value stores a Shizu_CxxFunction* value, then this Shizu_CxxFunction* is added to the callback list.
///   If a KeyboardKeyMessage/MouseButtonMessage/MousePointerMessage is dispatched, then the Shizu_CxxFunction* is invoked with the message object as its single argument.
/// - If the value stores a Shizu_Object value, then this object is wrapped into a weak reference and the weak reference is added to the callback list.
///   If a KeyboardKeyMessage/MouseButtonMessage/MousePointerMessage is dispatched, then the the Shizu_Object value in the weak reference is retrieved.
///   If it expired, then the weak reference is removed from the callback list.  Otherwise the Shizu_Object's "call" metamethod is invoked with the object as the first argument and the message object as the second argument.
/// - If the value stores a Shizu_Void value, then nothing is added to the callback list.
/// - If value stores any other value than the above, an error is raised.
void
Visuals_Service_addMousePointerCallback
  (
    Shizu_State* state,
    Shizu_Value* value
  );

Shizu_String*
Visuals_Service_getBackendVendorName
  (
    Shizu_State* state
  );

Shizu_String*
Visuals_Service_getBackendRendererName
  (
    Shizu_State* state
  );

Shizu_Integer32
Visuals_Service_getBackendMajorVersion
  (
    Shizu_State* state
  );

Shizu_Integer32
Visuals_Service_getBackendMinorVersion
  (
    Shizu_State* state
  );

#endif // VISUALS_SERVICE_H_INCLUDED
