#include "KeyboardKeyMessage.h"

Zeitgeist_ObjectType const g_KeyboardKeyMessage_Type = {
	.name = "KeyboardKeyMessage",
	.parentType = &g_Zeitgeist_Object_Type,
	.destruct = NULL,
};

KeyboardKeyMessage*
KeyboardKeyMessage_create
	(
		Zeitgeist_State* state,
		Zeitgeist_Integer action,
		Zeitgeist_Integer key
	)
{
	KeyboardKeyMessage* self = Zeitgeist_allocateObject(state, sizeof(KeyboardKeyMessage), NULL, NULL);

	self->action = action;
	self->key = key;

	return self;
}

Zeitgeist_Integer
KeyboardKeyMessage_getAction
	(
		Zeitgeist_State* state,
		KeyboardKeyMessage* self
	)
{
	cDebugAssert(NULL != state);
	cDebugAssert(NULL != self);
	return self->action;
}

Zeitgeist_Integer
KeyboardKeyMessage_getKey
	(
		Zeitgeist_State* state,
		KeyboardKeyMessage* self
	)
{
	cDebugAssert(NULL != state);
	cDebugAssert(NULL != self);
	return self->key;
}
