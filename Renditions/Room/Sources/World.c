#include "World.h"

#include "Visuals/BlinnPhongMaterialTechnique.h"
#include "Visuals/Context.h"
#include "Visuals/PhongMaterialTechnique.h"
#include "Visuals/VertexBuffer.h"

#include "Loader.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static Shizu_ByteArray* getFileContents(Shizu_State2* state, Shizu_String* relativePath) {
  Shizu_Value returnValue;
  Shizu_Value argumentValues[2];
  Shizu_Environment* environment = Shizu_Environment_getEnvironment(state, Shizu_State2_getGlobalEnvironment(state), Shizu_String_create(state, "FileSystem", strlen("FileSystem")));
  Shizu_CxxProcedure* p = Shizu_Environment_getCxxProcedure(state, environment, Shizu_String_create(state, "getWorkingDirectory", strlen("getWorkingDirectory")));
  p->f(state, &returnValue, 0, argumentValues);
  if (!Shizu_Value_isObject(&returnValue)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  if (!Shizu_Types_isSubTypeOf(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), Shizu_Value_getObject(&returnValue)->type,
    Shizu_String_getType(state))) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  Shizu_String* path = (Shizu_String*)Shizu_Value_getObject(&returnValue);
  // Get the directory separator.
  Shizu_String* directorySeparator = Shizu_Environment_getString(state, environment, Shizu_String_create(state, "directorySeparator", strlen("directorySeparator")));
  path = Shizu_String_concatenate(state, path, directorySeparator);
  path = Shizu_String_concatenate(state, path, relativePath);
  // Get the file contents.
  p = Shizu_Environment_getCxxProcedure(state, environment, Shizu_String_create(state, "getFileContents", strlen("getFileContents")));
  Shizu_Value_setObject(&argumentValues[0], (Shizu_Object*)path);
  p->f(state, &returnValue, 1, &argumentValues[0]);
  if (!Shizu_Value_isObject(&returnValue)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  if (!Shizu_Types_isSubTypeOf(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), Shizu_Value_getObject(&returnValue)->type,
    Shizu_ByteArray_getType(state))) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  Shizu_ByteArray* received = (Shizu_ByteArray*)Shizu_Value_getObject(&returnValue);
  return received;
}

static Shizu_Object* parse(Shizu_State2* state, Shizu_ByteArray* inputByteArray) {
  Shizu_String* inputString = NULL;
  inputString = Shizu_String_create(state, Shizu_ByteArray_getRawBytes(state, inputByteArray), Shizu_ByteArray_getNumberOfRawBytes(state, inputByteArray));
  Shizu_Environment* environment = Shizu_Environment_getEnvironment(state, Shizu_State2_getGlobalEnvironment(state), Shizu_String_create(state, "DataDefinitionLanguage", strlen("DataDefinitionLanguage")));
  Shizu_CxxProcedure* p = Shizu_Environment_getCxxProcedure(state, environment, Shizu_String_create(state, "createParser", strlen("createParser")));
  Shizu_Value returnValue; Shizu_Value arguments[1];
  p->f(state, &returnValue, 0, &arguments[0]);
  Shizu_Value self = returnValue;
  // set the input
  {
    Shizu_Value returnValue;
    Shizu_Value arguments[1];
    Shizu_Value_setObject(&arguments[0], (Shizu_Object*)inputString);
    uint8_t const methodNameBytes[] = "setInput";
    Shizu_Object_call(state, Shizu_Value_getObject(&self), &methodNameBytes[0], sizeof(methodNameBytes) - 1, &returnValue, 1, &arguments[0]);
  }
  // run
  {
    Shizu_Value returnValue;
    Shizu_Value arguments[1];
    uint8_t const methodNameBytes[] = "run";
    Shizu_Object_call(state, Shizu_Value_getObject(&self), &methodNameBytes[0], sizeof(methodNameBytes) - 1, &returnValue, 0, &arguments[0]);
    if (!Shizu_Value_isObject(&returnValue)) {
      Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
      Shizu_State2_jump(state);
    }
    return Shizu_Value_getObject(&returnValue);
  }
}

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

Shizu_defineObjectType("Zeitgeist.StaticGeometryGl", StaticGeometryGl, Shizu_Object);

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
    Shizu_List_appendObject(state, self->materials, (Shizu_Object*)Visuals_PhongMaterialTechnique_create(state));
    Shizu_List_appendObject(state, self->materials, (Shizu_Object*)Visuals_BlinnPhongMaterialTechnique_create(state));
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

Shizu_defineObjectType("Zeitgeist.World", World, Shizu_Object);

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

  Reader reader = { 
    .file = { 
      .path = NULL,
      .contents = NULL,
    },
    .names = {
      .astType = NULL,
      .astTypeFile = 0,
      .astTypeList = 0,
      .astTypeMap = 0,
    },
    .environment = NULL,
    .ast = NULL,
  };
  reader.file.path = Shizu_String_create(state, "Renditions", strlen("Renditions"));
  reader.file.path = Shizu_String_concatenate(state, reader.file.path, Shizu_String_create(state, Shizu_OperatingSystem_DirectorySeparator "Zeitgeist-Room", strlen(Shizu_OperatingSystem_DirectorySeparator "Zeitgeist-Room")));
  reader.file.path = Shizu_String_concatenate(state, reader.file.path, Shizu_String_create(state, Shizu_OperatingSystem_DirectorySeparator "Assets", strlen(Shizu_OperatingSystem_DirectorySeparator "Assets")));
  reader.file.path = Shizu_String_concatenate(state, reader.file.path, Shizu_String_create(state, Shizu_OperatingSystem_DirectorySeparator "material1.ddl", strlen(Shizu_OperatingSystem_DirectorySeparator "material1.ddl")));
  reader.file.contents = getFileContents(state, reader.file.path);
  reader.ast = parse(state, reader.file.contents);
  if (!isFile(state, &reader, reader.ast) || 1 != getNumberOfChildren(state, &reader, reader.ast)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  Shizu_Object* map = getChildAt(state, &reader, reader.ast, 0);
  Shizu_Object* object = NULL;
  object = getChildByName(state, &reader, map, Shizu_String_create(state, "type", sizeof("type") - 1));
  Shizu_String* typeText = getText(state, &reader, object);
  Shizu_Value materialTypeTextValue = Shizu_Value_InitializerObject(reader.names.material);
  if (!Shizu_Object_isEqualTo(state, (Shizu_Object*)typeText, &materialTypeTextValue)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  Shizu_Object* versionNode = getChildByName(state, &reader, map, reader.names.version);
  if (!isMap(state, &reader, versionNode)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  Shizu_Value received, expected;
  Shizu_Value_setObject(&received, (Shizu_Object*)getText(state, &reader, getChildByName(state, &reader, versionNode, reader.names.major)));
  Shizu_Operations_toInteger32(state, &received, 1, &received);
  Shizu_Value_setInteger32(&expected, INT32_C(1));
  if (!Shizu_Value_isEqualTo(state, &received, &expected)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  Shizu_Value_setObject(&received, (Shizu_Object*)getText(state, &reader, getChildByName(state, &reader, versionNode, reader.names.minor)));
  Shizu_Operations_toInteger32(state, &received, 1, &received);
  Shizu_Value_setInteger32(&expected, INT32_C(0));
  if (!Shizu_Value_isEqualTo(state, &received, &expected)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }



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
