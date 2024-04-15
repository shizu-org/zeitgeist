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

Shizu_declareType(KeyboardKeyMessage)

struct KeyboardKeyMessage_Dispatch {
	Shizu_Object_Dispatch _parent;
};

struct KeyboardKeyMessage {
	Shizu_Object _parent;
	Shizu_Integer32 action;
	Shizu_Integer32 key;
};

KeyboardKeyMessage*
KeyboardKeyMessage_create
	(
		Shizu_State* state,
		Shizu_Integer32 action,
		Shizu_Integer32 key
	);

Shizu_Integer32
KeyboardKeyMessage_getAction
	(
		Shizu_State* state,
		KeyboardKeyMessage* self
	);

Shizu_Integer32
KeyboardKeyMessage_getKey
	(
		Shizu_State* state,
		KeyboardKeyMessage* self
	);

#endif // KEYBOARDKEYMESSAGE_H_INCLUDED
