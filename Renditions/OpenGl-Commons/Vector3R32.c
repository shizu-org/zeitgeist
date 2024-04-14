#include "Vector3R32.h"

Zeitgeist_ObjectType const g_Vector3R32_Type = {
	.name = "Vector3R32",
	.parentType = &g_Zeitgeist_Object_Type,
	.destruct = NULL,
	.visit = NULL,
};

Vector3R32*
Vector3R32_create
	(
		Zeitgeist_State* state,
		Zeitgeist_Real32 x,
		Zeitgeist_Real32 y,
		Zeitgeist_Real32 z
	)
{
	Vector3R32* self = Zeitgeist_allocateObject(state, sizeof(Vector3R32), NULL, NULL);
	idlib_vector_3_f32_set(&self->v, x, y, z);
	((Zeitgeist_Object*)self)->type = &g_Vector3R32_Type;
	return self;
}

Vector3R32*
Vector3R32_add
	(
		Zeitgeist_State* state,
		Vector3R32* left,
		Vector3R32* right
	)
{
	Vector3R32* self = Zeitgeist_allocateObject(state, sizeof(Vector3R32), NULL, NULL);
	idlib_vector_3_f32_add(&self->v, &left->v, &right->v);
	return self;
}

Vector3R32*
Vector3R32_subtract
	(
		Zeitgeist_State* state,
		Vector3R32* left,
		Vector3R32* right
	)
{
	Vector3R32* self = Zeitgeist_allocateObject(state, sizeof(Vector3R32), NULL, NULL);
	idlib_vector_3_f32_subtract(&self->v, &left->v, &right->v);
	return self;
}
