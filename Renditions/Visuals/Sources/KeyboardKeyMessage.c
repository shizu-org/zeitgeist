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

#include "KeyboardKeyMessage.h"

#include "Shizu/Runtime/CxxUtilities.h"

static Shizu_TypeDescriptor const KeyboardKeyMessage_Type = {
	.postCreateType = NULL,
	.preDestroyType = NULL,
	.visitType = NULL,
	.size = sizeof(KeyboardKeyMessage),
	.finalize = NULL,
	.visit = NULL,
	.dispatchSize = sizeof(KeyboardKeyMessage_Dispatch),
	.dispatchInitialize = NULL,
	.dispatchUninitialize = NULL,
};

Shizu_defineType(KeyboardKeyMessage, Shizu_Object);

void
KeyboardKeyMessage_construct
	(
		Shizu_State2* state,
		KeyboardKeyMessage* self,
		Shizu_Integer32 action,
		Shizu_Integer32 key
	)
{
	Shizu_Type* TYPE = KeyboardKeyMessage_getType(state);
	Shizu_Object_construct(state, (Shizu_Object*)self);
	self->action = action;
	self->key = key;
	((Shizu_Object*)self)->type = TYPE;
}

KeyboardKeyMessage*
KeyboardKeyMessage_create
	(
		Shizu_State2* state,
		Shizu_Integer32 action,
		Shizu_Integer32 key
	)
{
	Shizu_Type* type = KeyboardKeyMessage_getType(state);
	KeyboardKeyMessage* self = (KeyboardKeyMessage*)Shizu_Gc_allocateObject(state, sizeof(KeyboardKeyMessage));
	KeyboardKeyMessage_construct(state, self, action, key);
	return self;
}

Shizu_Integer32
KeyboardKeyMessage_getAction
	(
		Shizu_State2* state,
		KeyboardKeyMessage* self
	)
{
	Shizu_debugAssert(NULL != state);
	Shizu_debugAssert(NULL != self);
	return self->action;
}

Shizu_Integer32
KeyboardKeyMessage_getKey
	(
		Shizu_State2* state,
		KeyboardKeyMessage* self
	)
{
	Shizu_debugAssert(NULL != state);
	Shizu_debugAssert(NULL != self);
	return self->key;
}
