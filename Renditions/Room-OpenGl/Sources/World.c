#include "World.h"

#include "Zeitgeist/UpstreamRequests.h"

#include "KeyboardKeyMessage.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static void
Player_visit
	(
		Shizu_State* state,
		Player* self
	);

static Shizu_TypeDescriptor const Player_Type = {
	.staticInitialize = NULL,
	.staticFinalize = NULL,
	.staticVisit = NULL,
	.size = sizeof(Player),
	.finalize = NULL,
	.visit = (Shizu_OnVisitCallback*) & Player_visit,
	.dispatchSize = sizeof(Player_Dispatch),
	.dispatchInitialize = NULL,
	.dispatchUninitialize = NULL,
};

Shizu_defineType(Player, Shizu_Object);

static void
Player_visit
	(
		Shizu_State* state,
		Player* self
	)
{
	if (self->position) {
		Shizu_Gc_visitObject(state, (Shizu_Object*)self->position);
	}
	if (self->positionSpeed) {
		Shizu_Gc_visitObject(state, (Shizu_Object*)self->positionSpeed);
	}
}

Player*
Player_create
	(
		Shizu_State* state
	)
{
	Player* self = (Player*)Shizu_Gc_allocate(state, sizeof(Player));
	self->position = Vector3R32_create(state, 0.f, 0.f, 0.f);
	self->positionSpeed = Vector3R32_create(state, 0.f, 0.f, 0.f);
	self->rotationY = 0.f;
	self->rotationYSpeed = 0.f;

	self->strafeLeftDown = false;
	self->strafeRightDown = false;
	self->moveForwardDown = false;
	self->moveBackwardDown = false;
	self->turnLeftDown = false ;
	self->turnRightDown = false;

	((Shizu_Object*)self)->type = Player_getType(state);// visit = (void(*)(Shizu_State*, Shizu_Object*)) & Player_visit;
	return self;
}

void
Player_onKeyboardKeyMessage
	(
		Shizu_State* state,
		Player* self,
		KeyboardKeyMessage* message
	)
{ 
	switch (KeyboardKeyMessage_getKey(state, message)) {
		case KeyboardKey_Q: {
			self->turnLeftDown = KeyboardKey_Action_Pressed == KeyboardKeyMessage_getAction(state, message);
		} break;
		case KeyboardKey_E: {
			self->turnRightDown = KeyboardKey_Action_Pressed == KeyboardKeyMessage_getAction(state, message);
		} break;
		case KeyboardKey_W:
		case KeyboardKey_Up: {
			self->moveForwardDown = KeyboardKey_Action_Pressed == KeyboardKeyMessage_getAction(state, message);
		} break;
		case KeyboardKey_S:
		case KeyboardKey_Down: {
			self->moveBackwardDown = KeyboardKey_Action_Pressed == KeyboardKeyMessage_getAction(state, message);
		} break;
		case KeyboardKey_A:
		case KeyboardKey_Left: {
			self->strafeLeftDown = KeyboardKey_Action_Pressed == KeyboardKeyMessage_getAction(state, message);
		} break;
		case KeyboardKey_D:
		case KeyboardKey_Right: {
			self->strafeRightDown = KeyboardKey_Action_Pressed == KeyboardKeyMessage_getAction(state, message);
		} break;
	};
}

void
Player_update
	(
		Shizu_State* state,
		Player* self,
		Shizu_Float32 tick
	)
{
	self->positionSpeed = Vector3R32_create(state, 0.f, 0.f, 0.f);
	if (self->strafeLeftDown != self->strafeRightDown) {
		if (self->strafeLeftDown) {
			Vector3R32* speed = Vector3R32_create(state, -1.f, 0.f, 0.f);
			self->positionSpeed = Vector3R32_add(state, self->positionSpeed, speed);
		} else {
			Vector3R32* speed = Vector3R32_create(state, +1.f, 0.f, 0.f);
			self->positionSpeed = Vector3R32_add(state, self->positionSpeed, speed);
		}
	}
	if (self->moveForwardDown != self->moveBackwardDown) {
		if (self->moveForwardDown) {
			Vector3R32* speed = Vector3R32_create(state, 0.f, 0.f, -1.f);
			self->positionSpeed = Vector3R32_add(state, self->positionSpeed, speed);
		} else {
			Vector3R32* speed = Vector3R32_create(state, 0.f, 0.f, +1.f);
			self->positionSpeed = Vector3R32_add(state, self->positionSpeed, speed);
		}
	}
	self->rotationYSpeed = 0.f;
	if (self->turnLeftDown != self->turnRightDown) {
		if (self->turnLeftDown) {
			self->rotationYSpeed += +1.f;
		} else {
			self->rotationYSpeed += -1.f;
		}
	}

	// update rotation.
	self->rotationY += 0.005f * self->rotationYSpeed * tick;

	// update position.
	idlib_vector_3_f32 v = self->positionSpeed->v;
	if (idlib_vector_3_f32_normalize(&v, &v)) {
		Matrix4R32* rotationY = Matrix4R32_createRotateY(state, self->rotationY);
		idlib_matrix_4x4_3f_transform_direction(&v, &rotationY->m, &v);
		Shizu_Float32 speed = 0.0001f * tick;
		v.e[0] *= speed;
		v.e[1] *= speed;
		v.e[2] *= speed;
		idlib_vector_3_f32_add(&self->position->v, &self->position->v, &v);
	}
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static void
StaticGeometryGl_finalize
	(
		Shizu_State* state,
		StaticGeometryGl* self
	);

static Shizu_TypeDescriptor const StaticGeometryGl_Type = {
	.staticInitialize = NULL,
	.staticFinalize = NULL,
	.staticVisit = NULL,
	.size = sizeof(StaticGeometryGl),
	.finalize = (Shizu_OnFinalizeCallback*)&StaticGeometryGl_finalize,
	.visit = NULL,
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
{ StaticGeometryGl_unmaterialize(state, self); }

void
StaticGeometryGl_unmaterialize
	(
		Shizu_State* state,
		StaticGeometryGl* self
	)
{
	if (self->colorTextureId) {
		glDeleteTextures(1, &self->colorTextureId);
		self->colorTextureId = 0;
	}
	if (self->vertexArrayId) {
		glDeleteVertexArrays(1, &self->vertexArrayId);
		self->vertexArrayId = 0;
	}
	if (self->bufferId) {
		glDeleteBuffers(1, &self->bufferId);
		self->bufferId = 0;
	}
}

StaticGeometryGl*
StaticGeometryGl_create
	(
		Shizu_State* state
	)
{
	StaticGeometryGl* self = (StaticGeometryGl*)Shizu_Gc_allocate(state, sizeof(StaticGeometryGl));

	self->numberOfVertices = 0;
	self->bufferId = 0;
	self->vertexArrayId = 0;

	while (glGetError()) { }
	glGenBuffers(1, &self->bufferId);
	if (glGetError()) {
		fprintf(stderr, "%s:%d: %s failed\n", __FILE__, __LINE__, "glGenBuffers");
		Shizu_State_setError(state, 1);
		Shizu_State_jump(state);
	}
	glGenVertexArrays(1, &self->vertexArrayId);
	if (glGetError()) {
		glDeleteBuffers(1, &self->bufferId);
		self->bufferId = 0;
		fprintf(stderr, "%s:%d: %s failed\n", __FILE__, __LINE__, "glGenVertexArrays");
		Shizu_State_setError(state, 1);
		Shizu_State_jump(state);
	}
	glGenTextures(1, &self->colorTextureId);
	if (glGetError()) {
		glDeleteVertexArrays(1, &self->vertexArrayId);
		self->vertexArrayId = 0;
		glDeleteBuffers(1, &self->bufferId);
		self->bufferId = 0;
		fprintf(stderr, "%s:%d: %s failed\n", __FILE__, __LINE__, "glGenVertexArrays");
		Shizu_State_setError(state, 1);
		Shizu_State_jump(state);
	}

	((Shizu_Object*)self)->type = StaticGeometryGl_getType(state);//(void(*)(Shizu_State*, Shizu_Object*))& StaticGeometryGl_finalize;
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
	static const GLint POSITION_INDEX = 0;
	
	static const GLint NORMAL_INDEX = 1;

	static const GLint AMBIENT_COLOR_INDEX = 2;

	// Compute vertex size.
	size_t vertexSize = numberOfBytes / numberOfVertices;
	
	// Warn if this does not hold. 
	if (numberOfBytes % vertexSize) {
		fprintf(stderr, "%s:%d: warning: additional %zu Bytes at the end of the data\n", __FILE__, __LINE__, numberOfBytes % vertexSize);
	}

	// Store the data in the buffer.
	glBindBuffer(GL_ARRAY_BUFFER, self->bufferId);
	self->numberOfVertices = numberOfVertices;
	self->numberOfBytes = numberOfBytes;
	glBufferData(GL_ARRAY_BUFFER, numberOfBytes, bytes, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(self->vertexArrayId);
	glBindBuffer(GL_ARRAY_BUFFER, self->bufferId);

	glEnableVertexAttribArray(POSITION_INDEX);
	glVertexAttribPointer(POSITION_INDEX,
		                    3,
		                    GL_FLOAT,
		                    GL_FALSE,
		                    vertexSize,
		                    (void*)(uintptr_t)0);

	glEnableVertexAttribArray(NORMAL_INDEX);
	glVertexAttribPointer(NORMAL_INDEX,
												3,
												GL_FLOAT,
												GL_FALSE,
												vertexSize,
												(void*)(uintptr_t)(sizeof(float) * 3));

	glEnableVertexAttribArray(AMBIENT_COLOR_INDEX);
	glVertexAttribPointer(AMBIENT_COLOR_INDEX,
		                    3,
		                    GL_FLOAT,
		                    GL_TRUE,
		                    vertexSize,
		                    (void*)(uintptr_t)(sizeof(float)*6));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
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
			Shizu_State_setError(state, 1);
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
			Shizu_State_setError(state, 1);
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
		Vector3R32* translation,
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
		Vector3R32* translation,
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
		Vector3R32* translation,
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
		Vector3R32* translation,
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
		Vector3R32* translation,
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
		Vector3R32* translation,
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
	.staticInitialize = NULL,
	.staticFinalize = NULL,
	.staticVisit = NULL,
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
		Shizu_Gc_visitObject(state, (Shizu_Object*)self->geometries);
	}
	if (self->player) {
		Shizu_Gc_visitObject(state, (Shizu_Object*)self->player);
	}
}

World*
World_create
	(
		Shizu_State* state
	)
{
	World* self = (World*)Shizu_Gc_allocate(state, sizeof(World));
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
	StaticGeometryGl_setDataFloor(state, geometry, Vector3R32_create(state, 0.f, -height / 2.f, 0.f), breadth, length);
	Shizu_List_appendObject(state, self->geometries, (Shizu_Object*)geometry);

	geometry = StaticGeometryGl_create(state);
	StaticGeometryGl_setDataCeiling(state, geometry, Vector3R32_create(state, 0.f, +height / 2.f, 0.f), breadth, length);
	Shizu_List_appendObject(state, self->geometries, (Shizu_Object*)geometry);

	geometry = StaticGeometryGl_create(state);
	StaticGeometryGl_setDataWestWall(state, geometry, Vector3R32_create(state, -breadth / 2.f, 0.f, 0.f), length, height);
	Shizu_List_appendObject(state, self->geometries, (Shizu_Object*)geometry);

	geometry = StaticGeometryGl_create(state);
	StaticGeometryGl_setDataNorthWall(state, geometry, Vector3R32_create(state, 0.f, 0.f, -length / 2.f), breadth, height);
	Shizu_List_appendObject(state, self->geometries, (Shizu_Object*)geometry);

	geometry = StaticGeometryGl_create(state);
	StaticGeometryGl_setDataEastWall(state, geometry, Vector3R32_create(state, +breadth / 2.f, 0.f, 0.f), length, height);
	Shizu_List_appendObject(state, self->geometries, (Shizu_Object*)geometry);

	geometry = StaticGeometryGl_create(state);
	StaticGeometryGl_setDataSouthWall(state, geometry, Vector3R32_create(state, 0.f, 0.f, +length / 2.f), breadth, height);
	Shizu_List_appendObject(state, self->geometries, (Shizu_Object*)geometry);

	self->player = Player_create(state);

	((Shizu_Object*)self)->type = World_getType(state);// (void(*)(Shizu_State*, Shizu_Object*))& World_visit;
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
