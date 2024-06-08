#if !defined(VECTOR3F32_H_INCLUDED)
#define VECTOR3F32_H_INCLUDED

#include "Zeitgeist.h"
#include "idlib/math.h"

Shizu_declareType(Vector3F32)

struct Vector3F32_Dispatch {
	Shizu_Object_Dispatch _parent;
};

struct Vector3F32 {
	Shizu_Object _parent;
	idlib_vector_3_f32 v;
};

Vector3F32*
Vector3F32_create
	(
		Shizu_State* state,
		Shizu_Float32 x,
		Shizu_Float32 y,
		Shizu_Float32 z
	);

Vector3F32*
Vector3F32_add
	(
		Shizu_State* state,
		Vector3F32* left,
		Vector3F32* right
	);

Vector3F32*
Vector3F32_subtract
	(
		Shizu_State* state,
		Vector3F32* left,
		Vector3F32* right
	);

#endif // VECTOR3F32_H_INCLUDED
