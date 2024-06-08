#if !defined(VECTOR4F32_H_INCLUDED)
#define VECTOR4F32_H_INCLUDED

#include "Zeitgeist.h"
#include "idlib/math.h"

Shizu_declareType(Vector4F32)

struct Vector4F32_Dispatch {
	Shizu_Object_Dispatch _parent;
};

struct Vector4F32 {
	Shizu_Object _parent;
	idlib_vector_4_f32 v;
};

Vector4F32*
Vector4F32_create
	(
		Shizu_State* state,
		Shizu_Float32 x,
		Shizu_Float32 y,
		Shizu_Float32 z,
		Shizu_Float32 w
	);

Vector4F32*
Vector4F32_add
	(
		Shizu_State* state,
		Vector4F32* left,
		Vector4F32* right
	);

Vector4F32*
Vector4F32_subtract
	(
		Shizu_State* state,
		Vector4F32* left,
		Vector4F32* right
	);

#endif // VECTOR3R32_H_INCLUDED
