#include "Vector4F32.h"

static Shizu_TypeDescriptor const Vector4F32_Type = {
	.postCreateType = NULL,
	.preDestroyType = NULL,
	.visitType = NULL,
	.size = sizeof(Vector4F32),
	.finalize = NULL,
	.visit = NULL,
	.dispatchSize = sizeof(Vector4F32_Dispatch),
	.dispatchInitialize = NULL,
	.dispatchUninitialize = NULL,
};

Shizu_defineType(Vector4F32, Shizu_Object);

Vector4F32*
Vector4F32_create
	(
		Shizu_State* state,
		Shizu_Float32 x,
		Shizu_Float32 y,
		Shizu_Float32 z,
		Shizu_Float32 w
	)
{
	Shizu_Type* type = Vector4F32_getType(state);
	Vector4F32* self = (Vector4F32*)Shizu_Gc_allocateObject(state, sizeof(Vector4F32));
	Shizu_Object_construct(state, (Shizu_Object*)self);
	idlib_vector_4_f32_set(&self->v, x, y, z, w);
	((Shizu_Object*)self)->type = type;
	return self;
}

Vector4F32*
Vector4F32_add
	(
		Shizu_State* state,
		Vector4F32* left,
		Vector4F32* right
	)
{
	Shizu_Type* type = Vector4F32_getType(state);
	Vector4F32* self = (Vector4F32*)Shizu_Gc_allocateObject(state, sizeof(Vector4F32));
	Shizu_Object_construct(state, (Shizu_Object*)self);
	idlib_vector_4_f32_add(&self->v, &left->v, &right->v);
	((Shizu_Object*)self)->type = type;
	return self;
}

Vector4F32*
Vector4F32_subtract
	(
		Shizu_State* state,
		Vector4F32* left,
		Vector4F32* right
	)
{
	Shizu_Type* type = Vector4F32_getType(state);
	Vector4F32* self = (Vector4F32*)Shizu_Gc_allocateObject(state, sizeof(Vector4F32));
	Shizu_Object_construct(state, (Shizu_Object*)self);
	idlib_vector_4_f32_subtract(&self->v, &left->v, &right->v);
	((Shizu_Object*)self)->type = type;
	return self;
}
