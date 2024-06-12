#if !defined(WORLD_H_INCLUDED)
#define WORLD_H_INCLUDED

#include "Player.h"
#include "Vector3F32.h"
#include "Visuals/Context.h"
#include "Visuals/Texture.h"
#include "Visuals/VertexBuffer.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @since 0.2
/// Indicates the geometry of the west side (9 o'clock) of a box.
/// The west side has the normal (-1,0,0).
#define BOX_WEST_SIDE (1)

/// @since 0.2
/// Indicates the geometry of the north side (12 o'clock) of a room.
/// The north side has the normal (0,0,-1).
#define BOX_NORTH_SIDE (2)

/// @since 0.2
/// Indicates the geometry of the east side (3 o'clock) of a box.
/// The east side has the the normal (+1,0,0).
#define BOX_EAST_SIDE (3)

/// @since 0.2
/// Indicates the geometry of the south side (6 o'clock) of a box.
/// The south side has the normal (0,0,+1).
#define BOX_SOUTH_SIDE (4)

/// @since 0.2
/// Indicates the geometry of the west wall (9 o'clock) of a room.
/// The west wall has the normal (+1,0,0).
#define WEST_WALL (5)

/// @since 0.2
/// Indicates the geometry of the north wall (12 o'clock) of a room.
/// The north wall has the normal (0,0,+1).
#define NORTH_WALL (6)

/// @since 0.2
/// Indicates the geometry of the east wall (3 o'clock) of a room.
/// The east wall has the the normal (-1,0,0).
#define EAST_WALL (7)

/// @since 0.2
/// Indicates the geometry of the south wall (6 o'clock) of a room.
/// The south wall has the normal (0,0,-1).
#define SOUTH_WALL (8)

/// @since 2.0
/// Indicates the geometry of the floor of a room.
/// The floor has the normal (0,+1,0).
#define FLOOR (9)

/// @since 2.0
/// Indicates geometry of the ceiling of a room.
/// The ceiling has the normal (0,-1,0).
#define CEILING (10)

/// @since 0.1
/// @brief
/// The retained mode representation of a part of a building's geometry.
/// A StaticGeometry object represents one of the following:
/// - the top, bottom, west, north, east, or south side of a box
/// - a floor of a room
/// - a ceiling of a room
/// - a west, north, east, or south walls of a room
Shizu_declareType(StaticGeometry)

struct StaticGeometry_Dispatch {
	Shizu_Object_Dispatch _parent;
};

struct StaticGeometry {
	Shizu_Object parent;

	/// @brief
	/// A pointer to a list of materials. Must not be null.
	/// @remarks
	/// The supported materials are Visuals_PhongMaterial and Visuals_BlinnPhongMaterial.
	/// The first material in the list is used.
	Shizu_List* materials;
	
	/// @brief The vertex buffer.
	Visuals_VertexBuffer* vertexBuffer;

	/// @brief The number of vertices of this wall.
	size_t numberOfVertices;
	
	/// @brief The number of Bytes of this wall.
	size_t numberOfBytes;
	
	/// @brief Either FLOOR or CEILING, WEST_WALL, NORTH_WALL, EAST_WALL, or SOUTH_WALL.
	uint8_t flags;

};

void
StaticGeometry_unmaterialize
	(
		Shizu_State* state,
		StaticGeometry* self
	);

/// @brief Set the vertex data.
/// @param state A pointer to the state.
/// @param self A pointer to to this StaticGeometryGl object.
/// @param numberOfVertices The size, in vertices, of the data.
/// @param numberOfBytes The size, in Bytes, of the data.
/// @param bytes A pointer to the data.
void
StaticGeometry_setData
	(
		Shizu_State* state,
		StaticGeometry* self,
		uint8_t flags,
		size_t numberOfVertices,
		size_t numberOfBytes,
		void const* bytes
	);

/// @brief Set the vertex data for the geometry of the north wall (12 o'clock).
/// The north wall has the normal (0,0,+1).
/// @param translation The translation of the wall along the axes.
/// Initially, the wall is centered on the origin.
/// @param breadth The extend along the x-axis. Must be positive.
/// @param height The extend along the y-axis. Must be positive.
void
StaticGeometry_setDataNorthWall
	(
		Shizu_State* state,
		StaticGeometry* self,
		Vector3F32* translation,
		Shizu_Float32 breadth,
		Shizu_Float32 height
	);

/// @brief Set the vertex data for the geometry of the south wall (6 o'clock).
/// The south wall has the normal (0,0,-1).
/// @param translation The translation of the wall along the axes.
/// Initially, the wall is centered on the origin.
/// @param breadth The extend along the x-axis. Must be positive.
/// @param height The extend along the y-axis. Must be positive.
void
StaticGeometry_setDataSouthWall
	(
		Shizu_State* state,
		StaticGeometry* self,
		Vector3F32* translation,
		Shizu_Float32 breadth,
		Shizu_Float32 height
	);

/// @brief Set the vertex data for the geometry of the east wall (3 o'clock).
/// The east wall has the normal (-1,0,0).
/// @param translation The translation of the wall along the axes.
/// Initially, the wall is centered on the origin.
/// @param breadth The extend along the z-axis. Must be positive.
/// @param height The extend along the y-axis. Must be positive.
void
StaticGeometry_setDataEastWall
	(
		Shizu_State* state,
		StaticGeometry* self,
		Vector3F32* translation,
		Shizu_Float32 breadth,
		Shizu_Float32 height
	);

/// @brief Set the vertex data for the geometry of the west wall (9 o'clock).
/// The west wall has the normal (+1,0,0).
/// @param translation The translation of the wall along the axes.
/// Initially, the wall is centered on the origin.
/// @param breadth The extend along the z-axis. Must be positive.
/// @param height The extend along the y-axis. Must be positive.
void
StaticGeometry_setDataWestWall
	(
		Shizu_State* state,
		StaticGeometry* self,
		Vector3F32* translation,
		Shizu_Float32 breadth,
		Shizu_Float32 height
	);

/// @brief Set the vertex data for the geometry of the floor.
/// The floor has the normal (0,+1,0).
/// @param translation The translation of the wall along the axes.
/// Initially, the wall is centered on the origin.
/// @param breadth The breadth (the extend along the x-axis). Must be positive.
/// @param length The length (the extend along the z-axis). Must be positive.
void
StaticGeometry_setDataFloor
	(
		Shizu_State* state,
		StaticGeometry* self,
		Vector3F32* translation,
		Shizu_Float32 breadth,
		Shizu_Float32 length
	);

/// @brief Set the vertex data for the geometry of the ceiling.
/// The ceiling has the normal (0,+1,0).
/// @param translation The translation of the wall along the axes.
/// Initially, the wall is centered on the origin.
/// @param breadth The breadth (the extend along the x-axis). Must be positive.
/// @param length The length (the extend along the z-axis). Must be positive.
void
StaticGeometry_setDataCeiling
	(
		Shizu_State* state,
		StaticGeometry* self,
		Vector3F32* translation,
		Shizu_Float32 breadth,
		Shizu_Float32 length
	);

StaticGeometry*
StaticGeometry_create
	(
		Shizu_State* state,
		Visuals_Context* visualsContext
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
		Shizu_State* state,
		Visuals_Context* visualsContext
	);

void
World_update
	(
		Shizu_State* state,
		World* self,
		Shizu_Float32 tick
	);

#endif // WORLD_H_INCLUDED
