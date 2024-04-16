#if !defined(WORLD_H_INCLUDED)
#define WORLD_H_INCLUDED

#include "Zeitgeist.h"

#include "Matrix4R32.h"
#include "Vector3R32.h"
#include "ServiceGl.h"
#include "Visuals/VertexBuffer.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Shizu_declareType(Player);

struct Player_Dispatch {
	Shizu_Object_Dispatch _parent;
};

/**
 * @brief The state of the player.
 */
struct Player {
	Shizu_Object parent;
	/**
	 * @brief A position vector indicating the position of the player in world coordinates.
	 * @remarks The default position is (0,0,0).
	 */
	Vector3R32* position;
	Vector3R32* positionSpeed;
	/**
	 * @brief A scalar indicating the rotation of the player around the Y axis.
	 * @remarks The default direction is (0,0,-1), the default rotation is 0.
	 */
	Shizu_Float32 rotationY;
	Shizu_Float32 rotationYSpeed;

	/**
	 * @brief If "strate left" is down.
	 */
	bool strafeLeftDown;
	/**
	 * @brief If "strafe right" is down.
	 */
	bool strafeRightDown;

	/**
	 * @brief If "move forward" is down.
	 */
	bool moveForwardDown;

	/**
   * @brief If "move backward" is down.
	 */
	bool moveBackwardDown;

	/**
   * @brief If "turn left" is down.
	 */
	bool turnLeftDown;

	/**
	 * @brief If "turn right is down.
	 */
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

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/**
 * @since 0.2
 * Indicates the geometry of the west side (9 o'clock) of a box.
 * The west side has the normal (-1,0,0).
 */
#define BOX_WEST_SIDE (1)

/**
 * @since 0.2
 * Indicates the geometry of the north side (12 o'clock) of a room.
 * The north side has the normal (0,0,-1).
 */
#define BOX_NORTH_SIDE (2)

/**
 * @since 0.2
 * Indicates the geometry of the east side (3 o'clock) of a box.
 * The east side has the the normal (+1,0,0).
 */
#define BOX_EAST_SIDE (3)

/**
 * @since 0.2
 * Indicates the geometry of the south side (6 o'clock) of a box.
 * The south side has the normal (0,0,+1).
 */
#define BOX_SOUTH_SIDE (4)

/**
 * @since 0.2
 * Indicates the geometry of the west wall (9 o'clock) of a room.
 * The west wall has the normal (+1,0,0).
 */
#define WEST_WALL (5)

/**
 * @since 0.2
 * Indicates the geometry of the north wall (12 o'clock) of a room.
 * The north wall has the normal (0,0,+1).
 */
#define NORTH_WALL (6)

/**
 * @since 0.2
 * Indicates the geometry of the east wall (3 o'clock) of a room.
 * The east wall has the the normal (-1,0,0).
 */
#define EAST_WALL (7)

/**
 * @since 0.2
 * Indicates the geometry of the south wall (6 o'clock) of a room.
 * The south wall has the normal (0,0,-1).
 */
#define SOUTH_WALL (8)

/**
 * @since 2.0
 * Indicates the geometry of the floor of a room.
 * The floor has the normal (0,+1,0).
 */
#define FLOOR (9)

/**
 * @since 2.0
 * Indicates geometry of the ceiling of a room.
 * The ceiling has the normal (0,-1,0).
 */
#define CEILING (10)

/**
 * @since 0.1
 * @brief
 * The retained mode representation of a part of a building's geometry.
 * A StaticGeometryGl object represents one of the following:
 * - the top, bottom, west, north, east, or south side of a box
 * - a floor of a room
 * - a ceiling of a room
 * - a west, north, east, or south walls of a room
 */
Shizu_declareType(StaticGeometryGl)

struct StaticGeometryGl_Dispatch {
	Shizu_Object_Dispatch _parent;
};

/**
 * @since 0.1
 * @brief
 * The retained mode representation of a part of a building's geometry.
 * A StaticGeometryGl object represents one of the following:
 * - the top, bottom, west, north, east, or south side of a box
 * - a floor of a room
 * - a ceiling of a room
 * - a west, north, east, or south walls of a room
 */
struct StaticGeometryGl {
	Shizu_Object parent;

	/**
	 * @brief The OpenGL ID of the color texture.
	 */
	GLuint colorTextureId;
	
	/**
	 * @brief The vertex buffer.
	 */
	Visuals_VertexBuffer* vertexBuffer;

	/** 
   * @brief The number of vertices of this wall.
	 */
	size_t numberOfVertices;
	
	/** 
   * @brief The number of Bytes of this wall.
	 */
	size_t numberOfBytes;
	
	/** 
	 * @brief Either FLOOR or CEILING, WEST_WALL, NORTH_WALL, EAST_WALL, or SOUT_WALL.
	 */
	uint8_t flags;

};

void
StaticGeometryGl_unmaterialize
	(
		Shizu_State* state,
		StaticGeometryGl* self
	);

/**
 * @brief Set the vertex data.
 * @param state A pointer to the state.
 * @param self A pointer to to this StaticGeometryGl object.
 * @param numberOfVertices The size, in vertices, of the data.
 * @param numberOfBytes The size, in Bytes, of the data.
 * @param bytes A pointer to the data.
 */
void
StaticGeometryGl_setData
	(
		Shizu_State* state,
		StaticGeometryGl* self,
		size_t numberOfVertices,
		size_t numberOfBytes,
		void const* bytes
	);

/**
 * @brief Set the vertex data for the geometry of the north wall (12 o'clock).
 * The north wall has the normal (0,0,+1).
 * @param translation The translation of the wall along the axes.
 * Initially, the wall is centered on the origin.
 * @param breadth The extend along the x-axis. Must be positive.
 * @param height The extend along the y-axis. Must be positive.
 */
void
StaticGeometryGl_setDataNorthWall
	(
		Shizu_State* state,
		StaticGeometryGl* self,
		Vector3R32* translation,
		Shizu_Float32 breadth,
		Shizu_Float32 height
	);

/**
 * @brief Set the vertex data for the geometry of the south wall (6 o'clock).
 * The south wall has the normal (0,0,-1).
 * @param translation The translation of the wall along the axes.
 * Initially, the wall is centered on the origin.
 * @param breadth The extend along the x-axis. Must be positive.
 * @param height The extend along the y-axis. Must be positive.
 */
void
StaticGeometryGl_setDataSouthWall
	(
		Shizu_State* state,
		StaticGeometryGl* self,
		Vector3R32* translation,
		Shizu_Float32 breadth,
		Shizu_Float32 height
	);

/**
 * @brief Set the vertex data for the geometry of the east wall (3 o'clock).
 * The east wall has the normal (-1,0,0).
 * @param translation The translation of the wall along the axes.
 * Initially, the wall is centered on the origin.
 * @param breadth The extend along the z-axis. Must be positive.
 * @param height The extend along the y-axis. Must be positive.
 */
void
StaticGeometryGl_setDataEastWall
	(
		Shizu_State* state,
		StaticGeometryGl* self,
		Vector3R32* translation,
		Shizu_Float32 breadth,
		Shizu_Float32 height
	);

/**
 * @brief Set the vertex data for the geometry of the west wall (9 o'clock).
 * The west wall has the normal (+1,0,0).
 * @param translation The translation of the wall along the axes.
 * Initially, the wall is centered on the origin.
 * @param breadth The extend along the z-axis. Must be positive.
 * @param height The extend along the y-axis. Must be positive.
 */
void
StaticGeometryGl_setDataWestWall
	(
		Shizu_State* state,
		StaticGeometryGl* self,
		Vector3R32* translation,
		Shizu_Float32 breadth,
		Shizu_Float32 height
	);

/**
 * @brief Set the vertex data for the geometry of the floor.
 * The floor has the normal (0,+1,0).
 * @param translation The translation of the wall along the axes.
 * Initially, the wall is centered on the origin.
 * @param breadth The breadth (the extend along the x-axis). Must be positive.
 * @param length The length (the extend along the z-axis). Must be positive.
 */
void
StaticGeometryGl_setDataFloor
	(
		Shizu_State* state,
		StaticGeometryGl* self,
		Vector3R32* translation,
		Shizu_Float32 breadth,
		Shizu_Float32 length
	);

/**
 * @brief Set the vertex data for the geometry of the ceiling.
 * The ceiling has the normal (0,+1,0).
 * @param translation The translation of the wall along the axes.
 * Initially, the wall is centered on the origin.
 * @param breadth The breadth (the extend along the x-axis). Must be positive.
 * @param length The length (the extend along the z-axis). Must be positive.
 */
void
StaticGeometryGl_setDataCeiling
	(
		Shizu_State* state,
		StaticGeometryGl* self,
		Vector3R32* translation,
		Shizu_Float32 breadth,
		Shizu_Float32 length
	);

StaticGeometryGl*
StaticGeometryGl_create
	(
		Shizu_State* state
	);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Shizu_declareType(World)

struct World_Dispatch {
	Shizu_Object_Dispatch _parent;
};

struct World {
	Shizu_Object _parent;
	/// @brief Pointer to the list of StaticGeometryGl objects.
	Shizu_List* geometries;
	/// @brief Information on the player.
	Player* player;
};

World*
World_create
	(
		Shizu_State* state
	);

void
World_update
	(
		Shizu_State* state,
		World* self,
		Shizu_Float32 tick
	);

#endif // WORLD_H_INCLUDED
