#include "KeyboardKeyMessage.h"

Shizu_TypeDescriptor const KeyboardKeyMessage_Type = {
	.staticInitialize = NULL,
	.staticFinalize = NULL,
	.staticVisit = NULL,
	.finalize = NULL,
	.visit = NULL,
};

Shizu_defineType(KeyboardKeyMessage, Shizu_Object);

KeyboardKeyMessage*
KeyboardKeyMessage_create
	(
		Shizu_State* state,
		Shizu_Integer32 action,
		Shizu_Integer32 key
	)
{
	KeyboardKeyMessage* self = (KeyboardKeyMessage*)Shizu_Gc_allocate(state, sizeof(KeyboardKeyMessage));

	self->action = action;
	self->key = key;

	return self;
}

Shizu_Integer32
KeyboardKeyMessage_getAction
	(
		Shizu_State* state,
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
		Shizu_State* state,
		KeyboardKeyMessage* self
	)
{
	Shizu_debugAssert(NULL != state);
	Shizu_debugAssert(NULL != self);
	return self->key;
}
