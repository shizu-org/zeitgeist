#include "World.h"

#include "Visuals/BlinnPhongMaterial.h"
#include "Visuals/Context.h"
#include "Visuals/PhongMaterial.h"
#include "Visuals/VertexBuffer.h"


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static void
StaticGeometry_finalize
  (
    Shizu_State2* state,
    StaticGeometry* self
  );

static void
StaticGeometry_visit
  (
    Shizu_State2* state,
    StaticGeometry* self
  );

static Shizu_ObjectTypeDescriptor const StaticGeometryGl_Type = {
  .postCreateType = NULL,
  .preDestroyType = NULL,
  .visitType = NULL,
  .size = sizeof(StaticGeometry),
  .finalize = (Shizu_OnFinalizeCallback*)&StaticGeometry_finalize,
  .visit = (Shizu_OnVisitCallback*)&StaticGeometry_visit,
  .dispatchSize = sizeof(StaticGeometry_Dispatch),
  .dispatchInitialize = NULL,
  .dispatchUninitialize = NULL,
};

Shizu_defineObjectType(StaticGeometryGl, Shizu_Object);

static void
StaticGeometry_finalize
  (
    Shizu_State2* state,
    StaticGeometry* self
  )
{
  self->materials = NULL;
  self->vertexBuffer = NULL;
}

static void
StaticGeometry_visit
  (
    Shizu_State2* state,
    StaticGeometry* self
  )
{
  if (self->materials) {
    Shizu_Gc_visitObject(Shizu_State2_getState1(state), Shizu_State2_getGc(state), (Shizu_Object*)self->materials);
  }
  if (self->vertexBuffer) {
    Shizu_Gc_visitObject(Shizu_State2_getState1(state), Shizu_State2_getGc(state), (Shizu_Object*)self->vertexBuffer);
  }
}

void
StaticGeometry_unmaterialize
  (
    Shizu_State2* state,
    StaticGeometry* self
  )
{
  for (size_t i = 0, n = Shizu_List_getSize(state, self->materials); i < n; ++i) {
    Shizu_Value element = Shizu_List_getValue(state, self->materials, i);
    Visuals_Object_unmaterialize(state, (Visuals_Object*)Shizu_Value_getObject(&element));
  }
  if (self->vertexBuffer) {
    Visuals_Object_unmaterialize(state, (Visuals_Object*)self->vertexBuffer);
  }
}

StaticGeometry*
StaticGeometry_create
  (
    Shizu_State2* state,
    Visuals_Context* visualsContext
  )
{
  Shizu_Type* type = StaticGeometryGl_getType(state);
  StaticGeometry* self = (StaticGeometry*)Shizu_Gc_allocateObject(state, sizeof(StaticGeometry));
  self->vertexBuffer = (Visuals_VertexBuffer*)Visuals_Context_createVertexBuffer(state, visualsContext);
  self->materials = Shizu_Runtime_Extensions_createList(state);
  {
    Shizu_List_appendObject(state, self->materials, (Shizu_Object*)Visuals_PhongMaterial_create(state));
    Shizu_List_appendObject(state, self->materials, (Shizu_Object*)Visuals_BlinnPhongMaterial_create(state));
  }
  self->numberOfVertices = 0;
  Visuals_Object_materialize(state, (Visuals_Object*)self->vertexBuffer);
  ((Shizu_Object*)self)->type = type;
  return self;
}

void
StaticGeometry_setData
  (
    Shizu_State2* state,
    StaticGeometry* self,
    uint8_t flags,
    size_t numberOfVertices,
    size_t numberOfBytes,
    void const* bytes
  )
{
  Visuals_VertexBuffer_setData(state, self->vertexBuffer, flags, bytes, numberOfBytes);
  self->numberOfVertices = numberOfVertices;
}

struct VERTEX {
  idlib_vector_3_f32 position;
  idlib_vector_3_f32 normal;
  idlib_color_3_f32 ambient;
  idlib_color_3_f32 diffuse;
  idlib_color_3_f32 specular;
  idlib_f32 shininess;
};

#if defined(_DEBUG)

  static void
  debugCheckNormal
    (
      Shizu_State2* state,
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
      Shizu_State2_setStatus(state, 1);
      Shizu_State2_jump(state);
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
      Shizu_State2_setStatus(state, 1);
      Shizu_State2_jump(state);
    }
  }

#else

  static void
  debugCheckNormal
    (
      Shizu_State2* state,
      struct VERTEX const* vertices,
      idlib_vector_3_f32 const* n
    )
  { }

#endif

void
StaticGeometry_setDataNorthWall
  (
    Shizu_State2* state,
    StaticGeometry* self,
    Vector3F32* translation,
    Shizu_Float32 breadth,
    Shizu_Float32 height
  )
{
  idlib_color_3_f32 ambient, diffuse, specular;
  idlib_color_3_f32_set(&ambient, 0.3f, 0.3f, 0.3f);
  idlib_color_3_f32_set(&diffuse, 0.3f, 0.3f, 0.3f);
  idlib_color_3_f32_set(&specular, 0.3f, 0.3f, 0.3f);
  idlib_f32 shininess = 0.9;
  struct VERTEX vertices[] = {
    {.position = { -0.5f,  0.5f, 0.f, }, .normal = { 0.f, 0.f, 1.f, }, .ambient = ambient, .diffuse = diffuse, .specular = specular, .shininess = shininess, },
    {.position = { -0.5f, -0.5f, 0.f, }, .normal = { 0.f, 0.f, 1.f, }, .ambient = ambient, .diffuse = diffuse, .specular = specular, .shininess = shininess, },
    {.position = {  0.5f,  0.5f, 0.f, }, .normal = { 0.f, 0.f, 1.f, }, .ambient = ambient, .diffuse = diffuse, .specular = specular, .shininess = shininess, },
    {.position = {  0.5f, -0.5f, 0.f, }, .normal = { 0.f, 0.f, 1.f, }, .ambient = ambient, .diffuse = diffuse, .specular = specular, .shininess = shininess, },
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
  StaticGeometry_setData(state, self, Visuals_VertexSemantics_PositionXyz_NormalXyz_AmbientRgb_DiffuseRgb_SpecularRgb_Shininess | Visuals_VertexSyntactics_Float3_Float3_Float3_Float3_Float3_Float, numberOfVertices, sizeof(vertices), vertices);
  self->flags = NORTH_WALL;
}

void
StaticGeometry_setDataSouthWall
  (
    Shizu_State2* state,
    StaticGeometry* self,
    Vector3F32* translation,
    Shizu_Float32 breadth,
    Shizu_Float32 height
  )
{
  idlib_color_3_f32 ambient, diffuse, specular;
  idlib_color_3_f32_set(&ambient, 0.3f, 0.3f, 0.3f);
  idlib_color_3_f32_set(&diffuse, 0.3f, 0.3f, 0.3f);
  idlib_color_3_f32_set(&specular, 0.3f, 0.3f, 0.3f);
  idlib_f32 shininess = 0.9;
  struct VERTEX vertices[] = {
    {.position = {  0.5f,  0.5f, 0.f, }, .normal = { 0.f, 0.f, -1.f, }, .ambient = ambient, .diffuse = diffuse, .specular = specular, .shininess = shininess, },
    {.position = {  0.5f, -0.5f, 0.f, }, .normal = { 0.f, 0.f, -1.f, }, .ambient = ambient, .diffuse = diffuse, .specular = specular, .shininess = shininess, },
    {.position = { -0.5f,  0.5f, 0.f, }, .normal = { 0.f, 0.f, -1.f, }, .ambient = ambient, .diffuse = diffuse, .specular = specular, .shininess = shininess, },
    {.position = { -0.5f, -0.5f, 0.f, }, .normal = { 0.f, 0.f, -1.f, }, .ambient = ambient, .diffuse = diffuse, .specular = specular, .shininess = shininess, },
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
  StaticGeometry_setData(state, self, Visuals_VertexSemantics_PositionXyz_NormalXyz_AmbientRgb_DiffuseRgb_SpecularRgb_Shininess | Visuals_VertexSyntactics_Float3_Float3_Float3_Float3_Float3_Float, numberOfVertices, sizeof(vertices), vertices);
  self->flags = SOUTH_WALL;
}

void
StaticGeometry_setDataEastWall
  (
    Shizu_State2* state,
    StaticGeometry* self,
    Vector3F32* translation,
    Shizu_Float32 breadth,
    Shizu_Float32 height
  )
{
  idlib_color_3_f32 ambient, diffuse, specular;
  idlib_color_3_f32_set(&ambient, 0.3f, 0.3f, 0.3f);
  idlib_color_3_f32_set(&diffuse, 0.3f, 0.3f, 0.3f);
  idlib_color_3_f32_set(&specular, 0.3f, 0.3f, 0.3f);
  idlib_f32 shininess = 0.9;
  struct VERTEX vertices[] = {
    {.position = { 0.f,  0.5f, -0.5f, }, .normal = { -1.f, 0.f, 0.f, }, .ambient = ambient, .diffuse = diffuse, .specular = specular, .shininess = shininess, },
    {.position = { 0.f, -0.5f, -0.5f, }, .normal = { -1.f, 0.f, 0.f, }, .ambient = ambient, .diffuse = diffuse, .specular = specular, .shininess = shininess, },
    {.position = { 0.f,  0.5f, +0.5f, }, .normal = { -1.f, 0.f, 0.f, }, .ambient = ambient, .diffuse = diffuse, .specular = specular, .shininess = shininess, },
    {.position = { 0.f, -0.5f, +0.5f, }, .normal = { -1.f, 0.f, 0.f, }, .ambient = ambient, .diffuse = diffuse, .specular = specular, .shininess = shininess, },
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
  StaticGeometry_setData(state, self, Visuals_VertexSemantics_PositionXyz_NormalXyz_AmbientRgb_DiffuseRgb_SpecularRgb_Shininess | Visuals_VertexSyntactics_Float3_Float3_Float3_Float3_Float3_Float, numberOfVertices, sizeof(vertices), vertices);
  self->flags = EAST_WALL;
}

void
StaticGeometry_setDataWestWall
  (
    Shizu_State2* state,
    StaticGeometry* self,
    Vector3F32* translation,
    Shizu_Float32 breadth,
    Shizu_Float32 height
  )
{
  idlib_color_3_f32 ambient, diffuse, specular;
  idlib_color_3_f32_set(&ambient, 0.3f, 0.3f, 0.3f);
  idlib_color_3_f32_set(&diffuse, 0.3f, 0.3f, 0.3f);
  idlib_color_3_f32_set(&specular, 0.3f, 0.3f, 0.3f);
  idlib_f32 shininess = 0.9;
  struct VERTEX vertices[] = {
    {.position = { 0.f,  0.5f, +0.5f, }, .normal = { +1.f, 0.f, 0.f, }, .ambient = ambient, .diffuse = diffuse, .specular = specular, .shininess = shininess, },
    {.position = { 0.f, -0.5f, +0.5f, }, .normal = { +1.f, 0.f, 0.f, }, .ambient = ambient, .diffuse = diffuse, .specular = specular, .shininess = shininess, },
    {.position = { 0.f,  0.5f, -0.5f, }, .normal = { +1.f, 0.f, 0.f, }, .ambient = ambient, .diffuse = diffuse, .specular = specular, .shininess = shininess, },
    {.position = { 0.f, -0.5f, -0.5f, }, .normal = { +1.f, 0.f, 0.f, }, .ambient = ambient, .diffuse = diffuse, .specular = specular, .shininess = shininess, },
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
  StaticGeometry_setData(state, self, Visuals_VertexSemantics_PositionXyz_NormalXyz_AmbientRgb_DiffuseRgb_SpecularRgb_Shininess | Visuals_VertexSyntactics_Float3_Float3_Float3_Float3_Float3_Float, numberOfVertices, sizeof(vertices), vertices);
  self->flags = WEST_WALL;
}

void
StaticGeometry_setDataFloor
  (
    Shizu_State2* state,
    StaticGeometry* self,
    Vector3F32* translation,
    Shizu_Float32 breadth,
    Shizu_Float32 length
  )
{
  idlib_color_3_f32 ambient, diffuse, specular;
  idlib_color_3_f32_set(&ambient, 0.3f, 0.3f, 0.3f);
  idlib_color_3_f32_set(&diffuse, 0.3f, 0.3f, 0.3f);
  idlib_color_3_f32_set(&specular, 0.3f, 0.3f, 0.3f);
  idlib_f32 shininess = 0.9;
  struct VERTEX vertices[] = {
    {.position = { -0.5f, 0.f, -0.5f, }, .normal = { 0.f, 1.f, 0.f, }, .ambient = ambient, .diffuse = diffuse, .specular = specular, .shininess = shininess, },
    {.position = { -0.5f, 0.f,  0.5f, }, .normal = { 0.f, 1.f, 0.f, }, .ambient = ambient, .diffuse = diffuse, .specular = specular, .shininess = shininess, },
    {.position = {  0.5f, 0.f, -0.5f, }, .normal = { 0.f, 1.f, 0.f, }, .ambient = ambient, .diffuse = diffuse, .specular = specular, .shininess = shininess, },
    {.position = {  0.5f, 0.f,  0.5f, }, .normal = { 0.f, 1.f, 0.f, }, .ambient = ambient, .diffuse = diffuse, .specular = specular, .shininess = shininess, },
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
  StaticGeometry_setData(state, self, Visuals_VertexSemantics_PositionXyz_NormalXyz_AmbientRgb_DiffuseRgb_SpecularRgb_Shininess | Visuals_VertexSyntactics_Float3_Float3_Float3_Float3_Float3_Float, numberOfVertices, sizeof(vertices), vertices);
  self->flags = FLOOR;
}

void
StaticGeometry_setDataCeiling
  (
    Shizu_State2* state,
    StaticGeometry* self,
    Vector3F32* translation,
    Shizu_Float32 breadth,
    Shizu_Float32 length
  )
{
  idlib_color_3_f32 ambient, diffuse, specular;
  idlib_color_3_f32_set(&ambient, 0.3f, 0.3f, 0.3f);
  idlib_color_3_f32_set(&diffuse, 0.3f, 0.3f, 0.3f);
  idlib_color_3_f32_set(&specular, 0.3f, 0.3f, 0.3f);
  idlib_f32 shininess = 0.9;
  struct VERTEX vertices[] = {
    {.position = { -0.5f, 0.f,  0.5f, }, .normal = { 0.f, -1.f, 0.f, }, .ambient = ambient, .diffuse = diffuse, .specular = specular, .shininess = shininess, },
    {.position = { -0.5f, 0.f, -0.5f, }, .normal = { 0.f, -1.f, 0.f, }, .ambient = ambient, .diffuse = diffuse, .specular = specular, .shininess = shininess, },
    {.position = {  0.5f, 0.f,  0.5f, }, .normal = { 0.f, -1.f, 0.f, }, .ambient = ambient, .diffuse = diffuse, .specular = specular, .shininess = shininess, },
    {.position = {  0.5f, 0.f, -0.5f, }, .normal = { 0.f, -1.f, 0.f, }, .ambient = ambient, .diffuse = diffuse, .specular = specular, .shininess = shininess, },
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
  StaticGeometry_setData(state, self, Visuals_VertexSemantics_PositionXyz_NormalXyz_AmbientRgb_DiffuseRgb_SpecularRgb_Shininess | Visuals_VertexSyntactics_Float3_Float3_Float3_Float3_Float3_Float, numberOfVertices, sizeof(vertices), vertices);
  self->flags = CEILING;
}

static void
World_visit
  (
    Shizu_State2* state,
    World* self
  );

static Shizu_ObjectTypeDescriptor const World_Type = {
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

Shizu_defineObjectType(World, Shizu_Object);

static void
World_visit
  (
    Shizu_State2* state,
    World* self
  )
{
  if (self->geometries) {
    Shizu_Gc_visitObject(Shizu_State2_getState1(state), Shizu_State2_getGc(state), (Shizu_Object*)self->geometries);
  }
  if (self->player) {
    Shizu_Gc_visitObject(Shizu_State2_getState1(state), Shizu_State2_getGc(state), (Shizu_Object*)self->player);
  }
}

World*
World_create
  (
    Shizu_State2* state,
    Visuals_Context* visualsContext
  )
{
  Shizu_Type* type = World_getType(state);
  World* self = (World*)Shizu_Gc_allocateObject(state, sizeof(World));
  self->player = NULL;
  self->geometries = NULL;

  self->geometries = Shizu_Runtime_Extensions_createList(state);

  StaticGeometry* geometry = NULL;

  // Extend along the x-axis in metres.
  static const Shizu_Float32 breadth = 5.f;
  // Extend along the z-axis in metres.
  static const Shizu_Float32 length = 5.f;
  // Extend along the y-axis in metres.
  static const Shizu_Float32 height = 4.f;

  geometry = StaticGeometry_create(state, visualsContext);
  StaticGeometry_setDataFloor(state, geometry, Vector3F32_create(state, 0.f, -height / 2.f, 0.f), breadth, length);
  Shizu_List_appendObject(state, self->geometries, (Shizu_Object*)geometry);

  geometry = StaticGeometry_create(state, visualsContext);
  StaticGeometry_setDataCeiling(state, geometry, Vector3F32_create(state, 0.f, +height / 2.f, 0.f), breadth, length);
  Shizu_List_appendObject(state, self->geometries, (Shizu_Object*)geometry);

  geometry = StaticGeometry_create(state, visualsContext);
  StaticGeometry_setDataWestWall(state, geometry, Vector3F32_create(state, -breadth / 2.f, 0.f, 0.f), length, height);
  Shizu_List_appendObject(state, self->geometries, (Shizu_Object*)geometry);

  geometry = StaticGeometry_create(state, visualsContext);
  StaticGeometry_setDataNorthWall(state, geometry, Vector3F32_create(state, 0.f, 0.f, -length / 2.f), breadth, height);
  Shizu_List_appendObject(state, self->geometries, (Shizu_Object*)geometry);

  geometry = StaticGeometry_create(state, visualsContext);
  StaticGeometry_setDataEastWall(state, geometry, Vector3F32_create(state, +breadth / 2.f, 0.f, 0.f), length, height);
  Shizu_List_appendObject(state, self->geometries, (Shizu_Object*)geometry);

  geometry = StaticGeometry_create(state, visualsContext);
  StaticGeometry_setDataSouthWall(state, geometry, Vector3F32_create(state, 0.f, 0.f, +length / 2.f), breadth, height);
  Shizu_List_appendObject(state, self->geometries, (Shizu_Object*)geometry);

  self->player = Player_create(state);

  ((Shizu_Object*)self)->type = type;
  return self;
}

void
World_update
  (
    Shizu_State2* state,
    World* self,
    Shizu_Float32 tick
  )
{
  Player_update(state, self->player, tick);
}
