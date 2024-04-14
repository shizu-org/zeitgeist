#include "World.h"

#include "Zeitgeist/List.h"
#include "Zeitgeist/UpstreamRequests.h"

#include "KeyboardKeyMessage.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Zeitgeist_ObjectType const g_Player_Type = {
	.name = "Player",
	.parentType = &g_Zeitgeist_Object_Type,
	.destruct = NULL,
};

static void
Player_visit
	(
		Zeitgeist_State* state,
		Player* self
	)
{
	if (self->position) {
		Zeitgeist_Gc_visitObject(state, (Zeitgeist_Object*)self->position);
	}
	if (self->positionSpeed) {
		Zeitgeist_Gc_visitObject(state, (Zeitgeist_Object*)self->positionSpeed);
	}
}

Player*
Player_create
	(
		Zeitgeist_State* state
	)
{
	Player* self = Zeitgeist_allocateObject(state, sizeof(Player), NULL, NULL);
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

	((Zeitgeist_Object*)self)->visit = (void(*)(Zeitgeist_State*, Zeitgeist_Object*)) & Player_visit;
	return self;
}

void
Player_onKeyboardKeyMessage
	(
		Zeitgeist_State* state,
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
		Zeitgeist_State* state,
		Player* self,
		Zeitgeist_Real32 tick
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
		Zeitgeist_Real32 speed = 0.0001f * tick;
		v.e[0] *= speed;
		v.e[1] *= speed;
		v.e[2] *= speed;
		idlib_vector_3_f32_add(&self->position->v, &self->position->v, &v);
	}
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static void
StaticGeometryGl_destruct
	(
		Zeitgeist_State* state,
		StaticGeometryGl* self
	);

Zeitgeist_ObjectType const g_StaticGeometryGl_Type = {
	.name = "StaticGeometryGl",
	.parentType = &g_Zeitgeist_Object_Type,
	.destruct = (void(*)(Zeitgeist_State*,Zeitgeist_Object*))&StaticGeometryGl_destruct,
	.visit = NULL,
};

static void
StaticGeometryGl_destruct
	(
		Zeitgeist_State* state,
		StaticGeometryGl* self
	)
{ StaticGeometryGl_unmaterialize(state, self); }


static void
StaticGeometryGl_finalize
	(
		Zeitgeist_State* state,
		StaticGeometryGl* self
	)
{
	StaticGeometryGl_unmaterialize(state, self);
}

void
StaticGeometryGl_unmaterialize
	(
		Zeitgeist_State* state,
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
		Zeitgeist_State* state
	)
{
	StaticGeometryGl* self = Zeitgeist_allocateObject(state, sizeof(StaticGeometryGl), NULL, NULL);

	self->numberOfVertices = 0;
	self->bufferId = 0;
	self->vertexArrayId = 0;

	while (glGetError()) { }
	glGenBuffers(1, &self->bufferId);
	if (glGetError()) {
		fprintf(stderr, "%s:%d: %s failed\n", __FILE__, __LINE__, "glGenBuffers");
		Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
	}
	glGenVertexArrays(1, &self->vertexArrayId);
	if (glGetError()) {
		glDeleteBuffers(1, &self->bufferId);
		self->bufferId = 0;
		fprintf(stderr, "%s:%d: %s failed\n", __FILE__, __LINE__, "glGenVertexArrays");
		Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
	}
	glGenTextures(1, &self->colorTextureId);
	if (glGetError()) {
		glDeleteVertexArrays(1, &self->vertexArrayId);
		self->vertexArrayId = 0;
		glDeleteBuffers(1, &self->bufferId);
		self->bufferId = 0;
		fprintf(stderr, "%s:%d: %s failed\n", __FILE__, __LINE__, "glGenVertexArrays");
		Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
	}

	((Zeitgeist_Object*)self)->finalize = (void(*)(Zeitgeist_State*, Zeitgeist_Object*)) & StaticGeometryGl_finalize;
	return self;
}

void
StaticGeometryGl_setData
	(
		Zeitgeist_State* state,
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
			Zeitgeist_State* state,
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
			Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
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
			Zeitgeist_State_raiseError(state, __FILE__, __LINE__, 1);
		}
	}

#else
	
	static void
	debugCheckNormal
		(
			Zeitgeist_State* state,
			struct VERTEX const* vertices,
			idlib_vector_3_f32 const* n
		)
	{ }

#endif

void
StaticGeometryGl_setDataNorthWall
	(
		Zeitgeist_State* state,
		StaticGeometryGl* self,
		Vector3R32* translation,
		Zeitgeist_Real32 breadth,
		Zeitgeist_Real32 height
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
		Zeitgeist_State* state,
		StaticGeometryGl* self,
		Vector3R32* translation,
		Zeitgeist_Real32 breadth,
		Zeitgeist_Real32 height
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
		Zeitgeist_State* state,
		StaticGeometryGl* self,
		Vector3R32* translation,
		Zeitgeist_Real32 breadth,
		Zeitgeist_Real32 height
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
		Zeitgeist_State* state,
		StaticGeometryGl* self,
		Vector3R32* translation,
		Zeitgeist_Real32 breadth,
		Zeitgeist_Real32 height
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
		Zeitgeist_State* state,
		StaticGeometryGl* self,
		Vector3R32* translation,
		Zeitgeist_Real32 breadth,
		Zeitgeist_Real32 length
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
		Zeitgeist_State* state,
		StaticGeometryGl* self,
		Vector3R32* translation,
		Zeitgeist_Real32 breadth,
		Zeitgeist_Real32 length
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
		Zeitgeist_State* state,
		World* self
	)
{
	if (self->geometries) {
		Zeitgeist_Gc_visitObject(state, (Zeitgeist_Object*)self->geometries);
	}
	if (self->player) {
		Zeitgeist_Gc_visitObject(state, (Zeitgeist_Object*)self->player);
	}
}

World*
World_create
	(
		Zeitgeist_State* state
	)
{
	World* self = Zeitgeist_allocateObject(state, sizeof(World), NULL, NULL);
	self->player = NULL;
	self->geometries = NULL;

	self->geometries = Zeitgeist_List_create(state);
	
	StaticGeometryGl* geometry = NULL;

	// Extend along the x-axis in metres.
	static const Zeitgeist_Real32 breadth = 5.f;
	// Extend along the z-axis in metres.
	static const Zeitgeist_Real32 length = 5.f;
	// Extend along the y-axis in metres.
	static const Zeitgeist_Real32 height = 4.f;

	geometry = StaticGeometryGl_create(state);
	StaticGeometryGl_setDataFloor(state, geometry, Vector3R32_create(state, 0.f, -height / 2.f, 0.f), breadth, length);
	Zeitgeist_List_appendObject(state, self->geometries, (Zeitgeist_Object*)geometry);

	geometry = StaticGeometryGl_create(state);
	StaticGeometryGl_setDataCeiling(state, geometry, Vector3R32_create(state, 0.f, +height / 2.f, 0.f), breadth, length);
	Zeitgeist_List_appendObject(state, self->geometries, (Zeitgeist_Object*)geometry);

	geometry = StaticGeometryGl_create(state);
	StaticGeometryGl_setDataWestWall(state, geometry, Vector3R32_create(state, -breadth / 2.f, 0.f, 0.f), length, height);
	Zeitgeist_List_appendObject(state, self->geometries, (Zeitgeist_Object*)geometry);

	geometry = StaticGeometryGl_create(state);
	StaticGeometryGl_setDataNorthWall(state, geometry, Vector3R32_create(state, 0.f, 0.f, -length / 2.f), breadth, height);
	Zeitgeist_List_appendObject(state, self->geometries, (Zeitgeist_Object*)geometry);

	geometry = StaticGeometryGl_create(state);
	StaticGeometryGl_setDataEastWall(state, geometry, Vector3R32_create(state, +breadth / 2.f, 0.f, 0.f), length, height);
	Zeitgeist_List_appendObject(state, self->geometries, (Zeitgeist_Object*)geometry);

	geometry = StaticGeometryGl_create(state);
	StaticGeometryGl_setDataSouthWall(state, geometry, Vector3R32_create(state, 0.f, 0.f, +length / 2.f), breadth, height);
	Zeitgeist_List_appendObject(state, self->geometries, (Zeitgeist_Object*)geometry);

	self->player = Player_create(state);

	((Zeitgeist_Object*)self)->visit = (void(*)(Zeitgeist_State*, Zeitgeist_Object*)) & World_visit;
	return self;
}

void
World_update
	(
		Zeitgeist_State* state,
		World* self,
		Zeitgeist_Real32 tick
	)
{
	Player_update(state, self->player, tick);
}
