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

#if !defined(KEYBOARDKEYMESSAGE_H_INCLUDED)
#define KEYBOARDKEYMESSAGE_H_INCLUDED

#include "Zeitgeist.h"

#define MousePointer_Action_Moved (0)

Shizu_declareObjectType(MousePointerMessage)

struct MousePointerMessage_Dispatch {
  Shizu_Object_Dispatch _parent;
};

struct MousePointerMessage {
  Shizu_Object _parent;
  Shizu_Integer32 action;
  Shizu_Integer32 x;
  Shizu_Integer32 y;
};

void
MousePointerMessage_construct
  (
    Shizu_State2* state,
    MousePointerMessage* self,
    Shizu_Integer32 action,
    Shizu_Integer32 x,
    Shizu_Integer32 y
  );

MousePointerMessage*
MousePointerMessage_create
  (
    Shizu_State2* state,
    Shizu_Integer32 action,
    Shizu_Integer32 x,
    Shizu_Integer32 y
  );

Shizu_Integer32
MousePointerMessage_getAction
  (
    Shizu_State2* state,
    MousePointerMessage* self
  );

Shizu_Integer32
MousePointerMessage_getX
  (
    Shizu_State2* state,
    MousePointerMessage* self
  );

Shizu_Integer32
MousePointerMessage_getY
  (
    Shizu_State2* state,
    MousePointerMessage* self
  );

#endif // KEYBOARDKEYMESSAGE_H_INCLUDED
