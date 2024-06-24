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

#define KeyboardKey_Action_Released (0)
#define KeyboardKey_Action_Pressed (1)

#define KeyboardKey_Up (1)
#define KeyboardKey_Down (2)
#define KeyboardKey_Left (3)
#define KeyboardKey_Right (4)
#define KeyboardKey_Escape (5)
#define KeyboardKey_Q (6)
#define KeyboardKey_E (7)
#define KeyboardKey_W (8)
#define KeyboardKey_A (9)
#define KeyboardKey_S (10)
#define KeyboardKey_D (11)

#define KeyboardKey_L (12)

Shizu_declareType(KeyboardKeyMessage)

struct KeyboardKeyMessage_Dispatch {
	Shizu_Object_Dispatch _parent;
};

struct KeyboardKeyMessage {
	Shizu_Object _parent;
	Shizu_Integer32 action;
	Shizu_Integer32 key;
};

/// @code
/// construct(Shizu.Integer32 action, Shizu.Integer32 key)
/// @endcode
void
KeyboardKeyMessage_construct
	(
		Shizu_State2* state,
		KeyboardKeyMessage* self,
		Shizu_Integer32 action,
		Shizu_Integer32 key
	);

KeyboardKeyMessage*
KeyboardKeyMessage_create
	(
		Shizu_State2* state,
		Shizu_Integer32 action,
		Shizu_Integer32 key
	);

Shizu_Integer32
KeyboardKeyMessage_getAction
	(
		Shizu_State2* state,
		KeyboardKeyMessage* self
	);

Shizu_Integer32
KeyboardKeyMessage_getKey
	(
		Shizu_State2* state,
		KeyboardKeyMessage* self
	);

#endif // KEYBOARDKEYMESSAGE_H_INCLUDED
