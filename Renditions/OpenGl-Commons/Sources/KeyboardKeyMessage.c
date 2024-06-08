#include "KeyboardKeyMessage.h"

#include "Shizu/Runtime/debugAssert.h"

Shizu_TypeDescriptor const KeyboardKeyMessage_Type = {
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

KeyboardKeyMessage*
KeyboardKeyMessage_create
	(
		Shizu_State* state,
		Shizu_Integer32 action,
		Shizu_Integer32 key
	)
{
	Shizu_Type* type = KeyboardKeyMessage_getType(state);
	KeyboardKeyMessage* self = (KeyboardKeyMessage*)Shizu_Gc_allocateObject(state, sizeof(KeyboardKeyMessage));
 	Shizu_Object_construct(state, (Shizu_Object*)self);
	self->action = action;
	self->key = key;
	((Shizu_Object*)self)->type = type;
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
