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

#include "MouseButtonMessage.h"

#include "Shizu/Runtime/debugAssert.h"

Shizu_TypeDescriptor const MouseButtonMessage_Type = {
	.postCreateType = NULL,
	.preDestroyType = NULL,
	.visitType = NULL,
	.size = sizeof(MouseButtonMessage),
	.finalize = NULL,
	.visit = NULL,
	.dispatchSize = sizeof(MouseButtonMessage_Dispatch),
	.dispatchInitialize = NULL,
	.dispatchUninitialize = NULL,
};

Shizu_defineType(MouseButtonMessage, Shizu_Object);

MouseButtonMessage*
MouseButtonMessage_create
	(
		Shizu_State* state,
		Shizu_Integer32 action,
		Shizu_Integer32 button
	)
{
	Shizu_Type* type = MouseButtonMessage_getType(state);
	MouseButtonMessage* self = (MouseButtonMessage*)Shizu_Gc_allocateObject(state, sizeof(MouseButtonMessage));
 	Shizu_Object_construct(state, (Shizu_Object*)self);
	self->action = action;
	self->button = button;
	((Shizu_Object*)self)->type = type;
	return self;
}

Shizu_Integer32
MouseButtonMessage_getAction
	(
		Shizu_State* state,
		MouseButtonMessage* self
	)
{
	Shizu_debugAssert(NULL != state);
	Shizu_debugAssert(NULL != self);
	return self->action;
}

Shizu_Integer32
MouseButtonMessage_getButton
	(
		Shizu_State* state,
		MouseButtonMessage* self
	)
{
	Shizu_debugAssert(NULL != state);
	Shizu_debugAssert(NULL != self);
	return self->button;
}
