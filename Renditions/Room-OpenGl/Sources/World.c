#include "World.h"

#include "Visuals/Gl/VertexBuffer.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static void
StaticGeometryGl_finalize
	(
		Shizu_State* state,
		StaticGeometryGl* self
	);

static void
StaticGeometryGl_visit
	(
		Shizu_State* state,
		StaticGeometryGl* self
	);

static Shizu_TypeDescriptor const StaticGeometryGl_Type = {
	.postCreateType = NULL,
	.preDestroyType = NULL,
	.visitType = NULL,
	.size = sizeof(StaticGeometryGl),
	.finalize = (Shizu_OnFinalizeCallback*)&StaticGeometryGl_finalize,
	.visit = (Shizu_OnVisitCallback*)&StaticGeometryGl_visit,
	.dispatchSize = sizeof(StaticGeometryGl_Dispatch),
	.dispatchInitialize = NULL,
	.dispatchUninitialize = NULL,
};

Shizu_defineType(StaticGeometryGl, Shizu_Object);

static void
StaticGeometryGl_finalize
	(
		Shizu_State* state,
		StaticGeometryGl* self
	)
{
	self->vertexBuffer = NULL; 
	StaticGeometryGl_unmaterialize(state, self);
}

static void
StaticGeometryGl_visit
	(
		Shizu_State* state,
		StaticGeometryGl* self
	)
{
	if (self->vertexBuffer) {
		Shizu_Gc_visitObject(Shizu_State_getState1(state), Shizu_State_getGc(state), (Shizu_Object*)self->vertexBuffer);
	}
}

void
StaticGeometryGl_unmaterialize
	(
		Shizu_State* state,
		StaticGeometryGl* self
	)
{
	if (self->vertexBuffer) {
		Visuals_Object_unmaterialize(state, (Visuals_Object*)self->vertexBuffer);
	}
	if (self->colorTextureId) {
		glDeleteTextures(1, &self->colorTextureId);
		self->colorTextureId = 0;
	}
}

StaticGeometryGl*
StaticGeometryGl_create
	(
		Shizu_State* state
	)
{
	Shizu_Type* type = StaticGeometryGl_getType(state);
	StaticGeometryGl* self = (StaticGeometryGl*)Shizu_Gc_allocateObject(state, sizeof(StaticGeometryGl));

	self->vertexBuffer = (Visuals_VertexBuffer*)Visuals_GlVertexBuffer_create(state);
	Visuals_Object_materialize(state, (Visuals_Object*)self->vertexBuffer);
	self->numberOfVertices = 0;
	glGenTextures(1, &self->colorTextureId);
	if (glGetError()) {
		Visuals_Object_unmaterialize(state, (Visuals_Object*)self->vertexBuffer);
		self->vertexBuffer = NULL;
		fprintf(stderr, "%s:%d: %s failed\n", __FILE__, __LINE__, "glGenVertexArrays");
		Shizu_State_setStatus(state, 1);
		Shizu_State_jump(state);
	}
	((Shizu_Object*)self)->type = type;
	return self;
}

void
StaticGeometryGl_setData
	(
		Shizu_State* state,
		StaticGeometryGl* self,
		size_t numberOfVertices,
		size_t numberOfBytes,
		void const* bytes
	)
{
	Visuals_VertexBuffer_setData(state, self->vertexBuffer, Visuals_VertexSemantics_PositionXyz_NormalXyz_AmbientRgb|Visuals_VertexSyntactics_Float3_Float3_Float3, bytes, numberOfBytes);
	self->numberOfVertices = numberOfVertices;
}

struct VERTEX {
	idlib_vector_3_f32 position;
	idlib_vector_3_f32 normal;
	idlib_color_3_f32 ambientColor;
};

#if defined(_DEBUG)

	static void
	debugCheckNormal
		(
			Shizu_State* state,
			struct VERTEX const* vertices,
			idlib_vector_3_f32 const* n
		)
	{
		idlib_vector_3_f32 x, y, z;

		idlib_vector_3_f32_subtract(&x, &vertices[1].position, &vertices[0].position);
		idlib_vector_3_f32_subtract(&y, &vertices[2].position, &vertices[0].position);
		idlib_vector_3_f32_cross(&z, &x, &y);
		idlib_vector_3_f32_normalize(&z, &z);
		if (!idlib_vector_3_f32_are_equal(n, &z)) {
			Shizu_State_setStatus(state, 1);
			Shizu_State_jump(state);
		}

		idlib_vector_3_f32 nn;
		idlib_vector_3_f32_negate(&nn, n);

		// the second vertex' normal must be the normal of the first vertex inverted. 
		// However, the specification of triangle strips state, the normal of subsequent triangles is the normal of the first triangle.
		idlib_vector_3_f32_subtract(&x, &vertices[2].position, &vertices[1].position);
		idlib_vector_3_f32_subtract(&y, &vertices[3].position, &vertices[1].position);
		idlib_vector_3_f32_cross(&z, &x, &y);
		idlib_vector_3_f32_normalize(&z, &z);
		if (!idlib_vector_3_f32_are_equal(&nn, &z)) {
			Shizu_State_setStatus(state, 1);
			Shizu_State_jump(state);
		}
	}

#else
	
	static void
	debugCheckNormal
		(
			Shizu_State* state,
			struct VERTEX const* vertices,
			idlib_vector_3_f32 const* n
		)
	{ }

#endif

void
StaticGeometryGl_setDataNorthWall
	(
		Shizu_State* state,
		StaticGeometryGl* self,
		Vector3F32* translation,
		Shizu_Float32 breadth,
		Shizu_Float32 height
	)
{
	idlib_color_3_f32 ambientColor;
	idlib_color_convert_3_u8_to_3_f32(&ambientColor, &idlib_colors_lightgray_3_u8);
	struct VERTEX vertices[] = {
		{.position = { -0.5f,  0.5f, 0.f, }, .normal = { 0.f, 0.f, 1.f, }, .ambientColor = ambientColor },
		{.position = { -0.5f, -0.5f, 0.f, }, .normal = { 0.f, 0.f, 1.f, }, .ambientColor = ambientColor, },
		{.position = {  0.5f,  0.5f, 0.f, }, .normal = { 0.f, 0.f, 1.f, }, .ambientColor = ambientColor, },
		{.position = {  0.5f, -0.5f, 0.f, }, .normal = { 0.f, 0.f, 1.f, }, .ambientColor = ambientColor, },
	};

	for (size_t i = 0; i < 4; ++i) {
		vertices[i].position.e[1] *= height;
		vertices[i].position.e[0] *= breadth;

		idlib_vector_3_f32_add(&vertices[i].position, &vertices[i].position, &translation->v);
	}

	idlib_vector_3_f32 n;
	idlib_vector_3_f32_set(&n, 0.f, 0.f, 1.f);
	debugCheckNormal(state, &vertices[0], &n);

	size_t numberOfVertices = sizeof(vertices) / sizeof(struct VERTEX);
	StaticGeometryGl_setData(state, self, numberOfVertices, sizeof(vertices), vertices);
	self->flags = NORTH_WALL;
}

void
StaticGeometryGl_setDataSouthWall
	(
		Shizu_State* state,
		StaticGeometryGl* self,
		Vector3F32* translation,
		Shizu_Float32 breadth,
		Shizu_Float32 height
	)
{
	idlib_color_3_f32 ambientColor;
	idlib_color_convert_3_u8_to_3_f32(&ambientColor, &idlib_colors_lightgray_3_u8);
	struct VERTEX vertices[] = {
		{.position = {  0.5f,  0.5f, 0.f, }, .normal = { 0.f, 0.f, -1.f, }, .ambientColor = ambientColor, },
		{.position = {  0.5f, -0.5f, 0.f, }, .normal = { 0.f, 0.f, -1.f, }, .ambientColor = ambientColor, },
		{.position = { -0.5f,  0.5f, 0.f, }, .normal = { 0.f, 0.f, -1.f, }, .ambientColor = ambientColor, },
		{.position = { -0.5f, -0.5f, 0.f, }, .normal = { 0.f, 0.f, -1.f, }, .ambientColor = ambientColor, },
	};

	for (size_t i = 0; i < 4; ++i) {
		vertices[i].position.e[1] *= height;
		vertices[i].position.e[0] *= breadth;

		idlib_vector_3_f32_add(&vertices[i].position, &vertices[i].position, &translation->v);
	}

	idlib_vector_3_f32 n;
	idlib_vector_3_f32_set(&n, 0.f, 0.f, -1.f);
	debugCheckNormal(state, &vertices[0], &n);

	size_t numberOfVertices = sizeof(vertices) / sizeof(struct VERTEX);
	StaticGeometryGl_setData(state, self, numberOfVertices, sizeof(vertices), vertices);
	self->flags = SOUTH_WALL;
}

void
StaticGeometryGl_setDataEastWall
	(
		Shizu_State* state,
		StaticGeometryGl* self,
		Vector3F32* translation,
		Shizu_Float32 breadth,
		Shizu_Float32 height
	)
{
	idlib_color_3_f32 ambientColor;
	idlib_color_convert_3_u8_to_3_f32(&ambientColor, &idlib_colors_lightgray_3_u8);
	struct VERTEX vertices[] = {
		{.position = { 0.f,  0.5f, -0.5f, }, .normal = { -1.f, 0.f, 0.f, }, .ambientColor = ambientColor, },
		{.position = { 0.f, -0.5f, -0.5f, }, .normal = { -1.f, 0.f, 0.f, }, .ambientColor = ambientColor, },
		{.position = { 0.f,  0.5f, +0.5f, }, .normal = { -1.f, 0.f, 0.f, }, .ambientColor = ambientColor, },
		{.position = { 0.f, -0.5f, +0.5f, }, .normal = { -1.f, 0.f, 0.f, }, .ambientColor = ambientColor, },
	};

	for (size_t i = 0; i < 4; ++i) {
		vertices[i].position.e[1] *= height;
		vertices[i].position.e[2] *= breadth;

		idlib_vector_3_f32_add(&vertices[i].position, &vertices[i].position, &translation->v);
	}

	idlib_vector_3_f32 n;
	idlib_vector_3_f32_set(&n, -1.f, 0.f, 0.f);
	debugCheckNormal(state, &vertices[0], &n);

	size_t numberOfVertices = sizeof(vertices) / sizeof(struct VERTEX);
	StaticGeometryGl_setData(state, self, numberOfVertices, sizeof(vertices), vertices);
	self->flags = EAST_WALL;
}

void
StaticGeometryGl_setDataWestWall
	(
		Shizu_State* state,
		StaticGeometryGl* self,
		Vector3F32* translation,
		Shizu_Float32 breadth,
		Shizu_Float32 height
	)
{
	idlib_color_3_f32 ambientColor;
	idlib_color_convert_3_u8_to_3_f32(&ambientColor, &idlib_colors_lightgray_3_u8);
	struct VERTEX vertices[] = {
		{.position = { 0.f,  0.5f, +0.5f, }, .normal = { +1.f, 0.f, 0.f, }, .ambientColor = ambientColor },
		{.position = { 0.f, -0.5f, +0.5f, }, .normal = { +1.f, 0.f, 0.f, }, .ambientColor = ambientColor, },
		{.position = { 0.f,  0.5f, -0.5f, }, .normal = { +1.f, 0.f, 0.f, }, .ambientColor = ambientColor, },
		{.position = { 0.f, -0.5f, -0.5f, }, .normal = { +1.f, 0.f, 0.f, }, .ambientColor = ambientColor, },
	};

	for (size_t i = 0; i < 4; ++i) {
		vertices[i].position.e[1] *= height;
		vertices[i].position.e[2] *= breadth;

		idlib_vector_3_f32_add(&vertices[i].position, &vertices[i].position, &translation->v);
	}

	idlib_vector_3_f32 n;
	idlib_vector_3_f32_set(&n, +1.f, 0.f, 0.f);
	debugCheckNormal(state, &vertices[0], &n);

	size_t numberOfVertices = sizeof(vertices) / sizeof(struct VERTEX);
	StaticGeometryGl_setData(state, self, numberOfVertices, sizeof(vertices), vertices);
	self->flags = WEST_WALL;
}

void
StaticGeometryGl_setDataFloor
	(
		Shizu_State* state,
		StaticGeometryGl* self,
		Vector3F32* translation,
		Shizu_Float32 breadth,
		Shizu_Float32 length
	)
{
	idlib_color_3_f32 ambientColor;
	idlib_color_convert_3_u8_to_3_f32(&ambientColor, &idlib_colors_lightgray_3_u8);
	struct VERTEX vertices[] = {
		{.position = { -0.5f, 0.f, -0.5f, }, .normal = { 0.f, 1.f, 0.f, }, .ambientColor = ambientColor, },
		{.position = { -0.5f, 0.f,  0.5f, }, .normal = { 0.f, 1.f, 0.f, }, .ambientColor = ambientColor, },
		{.position = {  0.5f, 0.f, -0.5f, }, .normal = { 0.f, 1.f, 0.f, }, .ambientColor = ambientColor, },
		{.position = {  0.5f, 0.f,  0.5f, }, .normal = { 0.f, 1.f, 0.f, }, .ambientColor = ambientColor, },
	};

	for (size_t i = 0; i < 4; ++i) {
		vertices[i].position.e[0] *= breadth;
		vertices[i].position.e[2] *= length;

		idlib_vector_3_f32_add(&vertices[i].position, &vertices[i].position, &translation->v);
	}

	idlib_vector_3_f32 n;
	idlib_vector_3_f32_set(&n, 0.f, +1.f, 0.f);
	debugCheckNormal(state, &vertices[0], &n);

	size_t numberOfVertices = sizeof(vertices) / sizeof(struct VERTEX);
	StaticGeometryGl_setData(state, self, numberOfVertices, sizeof(vertices), vertices);
	self->flags = FLOOR;
}

void
StaticGeometryGl_setDataCeiling
	(
		Shizu_State* state,
		StaticGeometryGl* self,
		Vector3F32* translation,
		Shizu_Float32 breadth,
		Shizu_Float32 length
	)
{
	idlib_color_3_f32 ambientColor;
	idlib_color_convert_3_u8_to_3_f32(&ambientColor, &idlib_colors_lightgray_3_u8);
	struct VERTEX vertices[] = {
		{.position = { -0.5f, 0.f,  0.5f, }, .normal = { 0.f, -1.f, 0.f, }, .ambientColor = ambientColor, },
		{.position = { -0.5f, 0.f, -0.5f, }, .normal = { 0.f, -1.f, 0.f, }, .ambientColor = ambientColor, },
		{.position = {  0.5f, 0.f,  0.5f, }, .normal = { 0.f, -1.f, 0.f, }, .ambientColor = ambientColor, },
		{.position = {  0.5f, 0.f, -0.5f, }, .normal = { 0.f, -1.f, 0.f, }, .ambientColor = ambientColor, },
	};

	for (size_t i = 0; i < 4; ++i) {
		vertices[i].position.e[0] *= breadth;
		vertices[i].position.e[2] *= length;

		idlib_vector_3_f32_add(&vertices[i].position, &vertices[i].position, &translation->v);
	}

	idlib_vector_3_f32 n;
	idlib_vector_3_f32_set(&n, 0.f, -1.f, 0.f);
	debugCheckNormal(state, &vertices[0], &n);

	size_t numberOfVertices = sizeof(vertices) / sizeof(struct VERTEX);
	StaticGeometryGl_setData(state, self, numberOfVertices, sizeof(vertices), vertices);
	self->flags = CEILING;
}

static void
World_visit
	(
		Shizu_State* state,
		World* self
	);

static Shizu_TypeDescriptor const World_Type = {
	.postCreateType = NULL,
	.postCreateType = NULL,
	.visitType = NULL,
	.size = sizeof(World),
	.finalize = NULL,
	.visit = (Shizu_OnVisitCallback*)&World_visit,
	.dispatchSize = sizeof(World_Dispatch),
	.dispatchInitialize = NULL,
	.dispatchUninitialize = NULL,
};

Shizu_defineType(World, Shizu_Object);

static void
World_visit
	(
		Shizu_State* state,
		World* self
	)
{
	if (self->geometries) {
		Shizu_Gc_visitObject(Shizu_State_getState1(state), Shizu_State_getGc(state), (Shizu_Object*)self->geometries);
	}
	if (self->player) {
		Shizu_Gc_visitObject(Shizu_State_getState1(state), Shizu_State_getGc(state), (Shizu_Object*)self->player);
	}
}

World*
World_create
	(
		Shizu_State* state
	)
{
	Shizu_Type* type = World_getType(state);
	World* self = (World*)Shizu_Gc_allocateObject(state, sizeof(World));
	self->player = NULL;
	self->geometries = NULL;

	self->geometries = Shizu_List_create(state);
	
	StaticGeometryGl* geometry = NULL;

	// Extend along the x-axis in metres.
	static const Shizu_Float32 breadth = 5.f;
	// Extend along the z-axis in metres.
	static const Shizu_Float32 length = 5.f;
	// Extend along the y-axis in metres.
	static const Shizu_Float32 height = 4.f;

	geometry = StaticGeometryGl_create(state);
	StaticGeometryGl_setDataFloor(state, geometry, Vector3F32_create(state, 0.f, -height / 2.f, 0.f), breadth, length);
	Shizu_List_appendObject(state, self->geometries, (Shizu_Object*)geometry);

	geometry = StaticGeometryGl_create(state);
	StaticGeometryGl_setDataCeiling(state, geometry, Vector3F32_create(state, 0.f, +height / 2.f, 0.f), breadth, length);
	Shizu_List_appendObject(state, self->geometries, (Shizu_Object*)geometry);

	geometry = StaticGeometryGl_create(state);
	StaticGeometryGl_setDataWestWall(state, geometry, Vector3F32_create(state, -breadth / 2.f, 0.f, 0.f), length, height);
	Shizu_List_appendObject(state, self->geometries, (Shizu_Object*)geometry);

	geometry = StaticGeometryGl_create(state);
	StaticGeometryGl_setDataNorthWall(state, geometry, Vector3F32_create(state, 0.f, 0.f, -length / 2.f), breadth, height);
	Shizu_List_appendObject(state, self->geometries, (Shizu_Object*)geometry);

	geometry = StaticGeometryGl_create(state);
	StaticGeometryGl_setDataEastWall(state, geometry, Vector3F32_create(state, +breadth / 2.f, 0.f, 0.f), length, height);
	Shizu_List_appendObject(state, self->geometries, (Shizu_Object*)geometry);

	geometry = StaticGeometryGl_create(state);
	StaticGeometryGl_setDataSouthWall(state, geometry, Vector3F32_create(state, 0.f, 0.f, +length / 2.f), breadth, height);
	Shizu_List_appendObject(state, self->geometries, (Shizu_Object*)geometry);

	self->player = Player_create(state);

	((Shizu_Object*)self)->type = type;
	return self;
}

void
World_update
	(
		Shizu_State* state,
		World* self,
		Shizu_Float32 tick
	)
{
	Player_update(state, self->player, tick);
}
