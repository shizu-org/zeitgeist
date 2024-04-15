#include "Vector3R32.h"

Shizu_TypeDescriptor const Vector3R32_Type = {
	.staticInitialize = NULL,
	.staticFinalize = NULL,
	.staticVisit = NULL,
	.size = sizeof(Vector3R32),
	.finalize = NULL,
	.visit = NULL,
	.dispatchSize = sizeof(Vector3R32_Dispatch),
	.dispatchInitialize = NULL,
	.dispatchUninitialize = NULL,
};

Shizu_defineType(Vector3R32, Shizu_Object);

Vector3R32*
Vector3R32_create
	(
		Shizu_State* state,
		Shizu_Float32 x,
		Shizu_Float32 y,
		Shizu_Float32 z
	)
{
	Vector3R32* self = (Vector3R32*)Shizu_Gc_allocate(state, sizeof(Vector3R32));
	idlib_vector_3_f32_set(&self->v, x, y, z);
	((Shizu_Object*)self)->type = Vector3R32_getType(state);
	return self;
}

Vector3R32*
Vector3R32_add
	(
		Shizu_State* state,
		Vector3R32* left,
		Vector3R32* right
	)
{
	Vector3R32* self = (Vector3R32*)Shizu_Gc_allocate(state, sizeof(Vector3R32));
	idlib_vector_3_f32_add(&self->v, &left->v, &right->v);
	((Shizu_Object*)self)->type = Vector3R32_getType(state);
	return self;
}

Vector3R32*
Vector3R32_subtract
	(
		Shizu_State* state,
		Vector3R32* left,
		Vector3R32* right
	)
{
	Vector3R32* self = (Vector3R32*)Shizu_Gc_allocate(state, sizeof(Vector3R32));
	idlib_vector_3_f32_subtract(&self->v, &left->v, &right->v);
	((Shizu_Object*)self)->type = Vector3R32_getType(state);
	return self;
}
