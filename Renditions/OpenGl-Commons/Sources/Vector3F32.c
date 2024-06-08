#include "Vector3F32.h"

static Shizu_TypeDescriptor const Vector3F32_Type = {
	.postCreateType = NULL,
	.preDestroyType = NULL,
	.visitType = NULL,
	.size = sizeof(Vector3F32),
	.finalize = NULL,
	.visit = NULL,
	.dispatchSize = sizeof(Vector3F32_Dispatch),
	.dispatchInitialize = NULL,
	.dispatchUninitialize = NULL,
};

Shizu_defineType(Vector3F32, Shizu_Object);

Vector3F32*
Vector3F32_create
	(
		Shizu_State* state,
		Shizu_Float32 x,
		Shizu_Float32 y,
		Shizu_Float32 z
	)
{
	Shizu_Type* type = Vector3F32_getType(state);
	Vector3F32* self = (Vector3F32*)Shizu_Gc_allocateObject(state, sizeof(Vector3F32));
	Shizu_Object_construct(state, (Shizu_Object*)self);
	idlib_vector_3_f32_set(&self->v, x, y, z);
	((Shizu_Object*)self)->type = type;
	return self;
}

Vector3F32*
Vector3F32_add
	(
		Shizu_State* state,
		Vector3F32* left,
		Vector3F32* right
	)
{
	Shizu_Type* type = Vector3F32_getType(state);
	Vector3F32* self = (Vector3F32*)Shizu_Gc_allocateObject(state, sizeof(Vector3F32));
	Shizu_Object_construct(state, (Shizu_Object*)self);
	idlib_vector_3_f32_add(&self->v, &left->v, &right->v);
	((Shizu_Object*)self)->type = type;
	return self;
}

Vector3F32*
Vector3F32_subtract
	(
		Shizu_State* state,
		Vector3F32* left,
		Vector3F32* right
	)
{
	Shizu_Type* type = Vector3F32_getType(state);
	Vector3F32* self = (Vector3F32*)Shizu_Gc_allocateObject(state, sizeof(Vector3F32));
	Shizu_Object_construct(state, (Shizu_Object*)self);
	idlib_vector_3_f32_subtract(&self->v, &left->v, &right->v);
	((Shizu_Object*)self)->type = type;
	return self;
}
