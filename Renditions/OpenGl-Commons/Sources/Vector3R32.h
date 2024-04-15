#if !defined(VECTOR3R32_H_INCLUDED)
#define VECTOR3R32_H_INCLUDED

#include "Zeitgeist.h"
#include "idlib-math.h"

Shizu_declareType(Vector3R32)

struct Vector3R32_Dispatch {
	Shizu_Object_Dispatch _parent;
};

struct Vector3R32 {
	Shizu_Object _parent;
	idlib_vector_3_f32 v;
};

Vector3R32*
Vector3R32_create
	(
		Shizu_State* state,
		Shizu_Float32 x,
		Shizu_Float32 y,
		Shizu_Float32 z
	);

Vector3R32*
Vector3R32_add
	(
		Shizu_State* state,
		Vector3R32* left,
		Vector3R32* right
	);

Vector3R32*
Vector3R32_subtract
	(
		Shizu_State* state,
		Vector3R32* left,
		Vector3R32* right
	);

#endif // VECTOR3R32_H_INCLUDED
