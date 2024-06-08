#if !defined(PLAYER_H_INCLUDED)
#define PLAYER_H_INCLUDED

#include "Zeitgeist.h"
#include "KeyboardKeyMessage.h"
#include "Vector3F32.h"

Shizu_declareType(Player);

struct Player_Dispatch {
	Shizu_Object_Dispatch _parent;
};

/**
 * @brief The state of the player.
 */
struct Player {
	Shizu_Object parent;
	/// @brief A position vector indicating the position of the player in world coordinates.
	/// @remarks The default position is (0,0,0).
	Vector3F32* position;
	Vector3F32* positionSpeed;
	/// @brief A scalar indicating the rotation of the player around the Y axis.
	/// @remarks The default direction is (0,0,-1), the default rotation is 0.
	Shizu_Float32 rotationY;
	Shizu_Float32 rotationYSpeed;

	/// @brief If "strate left" is down.
	bool strafeLeftDown;
	/// @brief If "strafe right" is down.
	bool strafeRightDown;
	/// @brief If "move forward" is down.
	bool moveForwardDown;
	/// @brief If "move backward" is down.
	bool moveBackwardDown;
	/// @brief If "turn left" is down.
	bool turnLeftDown;
	/// @brief If "turn right is down.
	bool turnRightDown;
};

Player*
Player_create
	(
		Shizu_State* state
	);

void
Player_onKeyboardKeyMessage
	(
		Shizu_State* state,
		Player* self,
		KeyboardKeyMessage* message
	);

void
Player_update
	(
		Shizu_State* state,
		Player* self,
		Shizu_Float32 tick
	);

#endif // PLAYER_H_INCLUDED
