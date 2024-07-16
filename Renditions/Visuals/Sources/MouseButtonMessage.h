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

#if !defined(MOUSEBUTTONMESSAGE_H_INCLUDED)
#define MOUSEBUTTONMESSAGE_H_INCLUDED

#include "Zeitgeist.h"

#define MouseButton_Action_Released (0)
#define MouseButton_Action_Pressed (1)

#define MouseButton_One (1)
#define MouseButton_Two (2)
#define MouseButton_Three (3)
#define MouseButton_Four (4)
#define MouseButton_Five (5)
#define MouseButton_Six (6)
#define MouseButton_Seven (7)
#define MouseButton_Eight (8)
#define MouseButton_Nine (9)
#define MouseButton_Ten (10)
#define MouseButton_Eleven (11)
#define MouseButton_Twelve (12)

/// The type
/// @code
/// class MouseButtonMessage
/// @endcode
/// Its constructor is
/// @code
/// MouseButtonMessage.construct(Integer32 action, Integer32 button)
/// @endcode
Shizu_declareObjectType(MouseButtonMessage)

struct MouseButtonMessage_Dispatch {
  Shizu_Object_Dispatch _parent;
};

struct MouseButtonMessage {
  Shizu_Object _parent;
  Shizu_Integer32 action;
  Shizu_Integer32 button;
  Shizu_Integer32 x;
  Shizu_Integer32 y;
};

MouseButtonMessage*
MouseButtonMessage_create
  (
    Shizu_State2* state,
    Shizu_Integer32 action,
    Shizu_Integer32 button,
    Shizu_Integer32 x,
    Shizu_Integer32 y
  );

Shizu_Integer32
MouseButtonMessage_getAction
  (
    Shizu_State2* state,
    MouseButtonMessage* self
  );

Shizu_Integer32
MouseButtonMessage_getButton
  (
    Shizu_State2* state,
    MouseButtonMessage* self
  );

#endif // MOUSEBUTTONMESSAGE_H_INCLUDED
