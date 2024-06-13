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

#include "MousePointerMessage.h"

#include "Shizu/Runtime/debugAssert.h"

Shizu_TypeDescriptor const MousePointerMessage_Type = {
	.postCreateType = NULL,
	.preDestroyType = NULL,
	.visitType = NULL,
	.size = sizeof(MousePointerMessage),
	.finalize = NULL,
	.visit = NULL,
	.dispatchSize = sizeof(MousePointerMessage_Dispatch),
	.dispatchInitialize = NULL,
	.dispatchUninitialize = NULL,
};

Shizu_defineType(MousePointerMessage, Shizu_Object);

MousePointerMessage*
MousePointerMessage_create
	(
		Shizu_State* state,
		Shizu_Integer32 action,
		Shizu_Integer32 x,
    Shizu_Integer32 y
	)
{
	Shizu_Type* type = MousePointerMessage_getType(state);
	MousePointerMessage* self = (MousePointerMessage*)Shizu_Gc_allocateObject(state, sizeof(MousePointerMessage));
 	Shizu_Object_construct(state, (Shizu_Object*)self);
	self->action = action;
	self->x = x;
  self->y = y;
	((Shizu_Object*)self)->type = type;
	return self;
}

Shizu_Integer32
MousePointerMessage_getAction
	(
		Shizu_State* state,
		MousePointerMessage* self
	)
{
	Shizu_debugAssert(NULL != state);
	Shizu_debugAssert(NULL != self);
	return self->action;
}

Shizu_Integer32
MousePointerMessage_getX
	(
		Shizu_State* state,
		MousePointerMessage* self
	)
{
	Shizu_debugAssert(NULL != state);
	Shizu_debugAssert(NULL != self);
	return self->x;
}

Shizu_Integer32
MousePointerMessage_getY
	(
		Shizu_State* state,
		MousePointerMessage* self
	)
{
	Shizu_debugAssert(NULL != state);
	Shizu_debugAssert(NULL != self);
	return self->y;
}
