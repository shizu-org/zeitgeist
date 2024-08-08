#if !defined(LOADER_H_INCLUDED)
#define LOADER_H_INCLUDED

typedef struct Reader {
  Shizu_Environment* environment;
  Shizu_Object* ast;
  struct {
    Shizu_String* path;
    Shizu_ByteArray* contents;
  } file;
  struct {
    // The string "version".
    Shizu_String* version;
    // The string "major".
    Shizu_String* major;
    // The string "minor".
    Shizu_String* minor;
    // The string "AstType".
    Shizu_String* astType;
    // The string "Material".
    Shizu_String* material;
    Shizu_Integer32 astTypeFile;
    Shizu_Integer32 astTypeInteger;
    Shizu_Integer32 astTypeList;
    Shizu_Integer32 astTypeMap;
    Shizu_Integer32 astTypeMapElement;
    Shizu_Integer32 astTypeName;
  } names;
} Reader;

static inline ensureReaderIsInitialized(Shizu_State2* state, Reader* reader);

static inline Shizu_Boolean isFile(Shizu_State2* state, Reader* reader, Shizu_Object* ast);
static inline Shizu_Boolean isList(Shizu_State2* state, Reader* reader, Shizu_Object* ast);
static inline Shizu_Boolean isMap(Shizu_State2* state, Reader* reader, Shizu_Object* ast);
static inline Shizu_Boolean isMapElement(Shizu_State2* state, Reader* reader, Shizu_Object* ast);
static inline Shizu_Boolean isName(Shizu_State2* state, Reader* reader, Shizu_Object* ast);

static inline Shizu_Object* getChildAt(Shizu_State2* state, Reader* reader, Shizu_Object* parent, Shizu_Integer32 index);
static inline Shizu_Integer32 getNumberOfChildren(Shizu_State2* state, Reader* reader, Shizu_Object* ast);
static inline Shizu_String* getText(Shizu_State2* state, Reader* reader, Shizu_Object* ast);
static inline Shizu_Object* getChildByName(Shizu_State2* state, Reader* reader, Shizu_Object* parent, Shizu_String* name);

static inline ensureReaderIsInitialized(Shizu_State2* state, Reader* reader) {
  if (!reader->environment) {
    reader->environment = Shizu_Environment_getEnvironment(state, Shizu_State2_getGlobalEnvironment(state), Shizu_String_create(state, "DataDefinitionLanguage", strlen("DataDefinitionLanguage")));
    reader->names.astType = Shizu_String_create(state, "AstType", strlen("AstType"));
    reader->names.material = Shizu_String_create(state, "Material", strlen("Material"));
    reader->names.version = Shizu_String_create(state, "version", strlen("version"));
    reader->names.major = Shizu_String_create(state, "major", strlen("major"));
    reader->names.minor = Shizu_String_create(state, "minor", strlen("minor"));

    Shizu_Environment* environment = Shizu_Environment_getEnvironment(state, reader->environment, Shizu_String_create(state, "AstType", strlen("AstType")));
    reader->names.astTypeFile = Shizu_Environment_getInteger32(state, environment, Shizu_String_create(state, "File", strlen("File")));
    reader->names.astTypeList = Shizu_Environment_getInteger32(state, environment, Shizu_String_create(state, "List", strlen("List")));
    reader->names.astTypeMap = Shizu_Environment_getInteger32(state, environment, Shizu_String_create(state, "Map", strlen("Map")));
    reader->names.astTypeMapElement = Shizu_Environment_getInteger32(state, environment, Shizu_String_create(state, "MapElement", strlen("MapElement")));
    reader->names.astTypeName = Shizu_Environment_getInteger32(state, environment, Shizu_String_create(state, "Name", strlen("Name")));
  }
}

static inline Shizu_Boolean isFile(Shizu_State2* state, Reader* reader, Shizu_Object* ast) {
  ensureReaderIsInitialized(state, reader);
  Shizu_Value returnValue;
  Shizu_Value arguments[] = { Shizu_Value_InitializerVoid(Shizu_Void_Void) };
  Shizu_Object_call(state, ast, "getType", sizeof("getType") - 1, &returnValue, 0, &arguments[0]);
  if (!Shizu_Value_isInteger32(&returnValue)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  return reader->names.astTypeFile == Shizu_Value_getInteger32(&returnValue);
}

static inline Shizu_Boolean isList(Shizu_State2* state, Reader* reader, Shizu_Object* ast) {
  ensureReaderIsInitialized(state, reader);
  Shizu_Value returnValue;
  Shizu_Value arguments[] = { Shizu_Value_InitializerVoid(Shizu_Void_Void) };
  Shizu_Object_call(state, ast, "getType", sizeof("getType") - 1, &returnValue, 0, &arguments[0]);
  if (!Shizu_Value_isInteger32(&returnValue)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  return reader->names.astTypeList == Shizu_Value_getInteger32(&returnValue);
}

static inline Shizu_Boolean isMap(Shizu_State2* state, Reader* reader, Shizu_Object* ast) {
  ensureReaderIsInitialized(state, reader);
  Shizu_Value returnValue;
  Shizu_Value arguments[] = { Shizu_Value_InitializerVoid(Shizu_Void_Void) };
  Shizu_Object_call(state, ast, "getType", sizeof("getType") - 1, &returnValue, 0, &arguments[0]);
  if (!Shizu_Value_isInteger32(&returnValue)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  return reader->names.astTypeMap == Shizu_Value_getInteger32(&returnValue);
}

static inline Shizu_Boolean isMapElement(Shizu_State2* state, Reader* reader, Shizu_Object* ast) {
  ensureReaderIsInitialized(state, reader);
  Shizu_Value returnValue;
  Shizu_Value arguments[] = { Shizu_Value_InitializerVoid(Shizu_Void_Void) };
  Shizu_Object_call(state, ast, "getType", sizeof("getType") - 1, &returnValue, 0, &arguments[0]);
  if (!Shizu_Value_isInteger32(&returnValue)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  return reader->names.astTypeMapElement == Shizu_Value_getInteger32(&returnValue);
}

static inline Shizu_Boolean isName(Shizu_State2* state, Reader* reader, Shizu_Object* ast) {
  ensureReaderIsInitialized(state, reader);
  Shizu_Value returnValue;
  Shizu_Value arguments[] = { Shizu_Value_InitializerVoid(Shizu_Void_Void) };
  Shizu_Object_call(state, ast, "getType", sizeof("getType") - 1, &returnValue, 0, &arguments[0]);
  if (!Shizu_Value_isInteger32(&returnValue)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  return reader->names.astTypeName == Shizu_Value_getInteger32(&returnValue);
}

static inline Shizu_Object* getChildAt(Shizu_State2* state, Reader* reader, Shizu_Object* parent, Shizu_Integer32 index) {
  ensureReaderIsInitialized(state, reader);
  Shizu_Value returnValue;
  Shizu_Value arguments[] = { Shizu_Value_InitializerInteger32(index) };
  Shizu_Object_call(state, parent, "getChildAt", sizeof("getChildAt") - 1, &returnValue, 1, &arguments[0]);
  if (!Shizu_Value_isObject(&returnValue)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  return Shizu_Value_getObject(&returnValue);
}

static inline Shizu_Integer32 getNumberOfChildren(Shizu_State2* state, Reader* reader, Shizu_Object* ast) {
  ensureReaderIsInitialized(state, reader);
  Shizu_Value returnValue;
  Shizu_Value arguments[] = { Shizu_Value_InitializerVoid(Shizu_Void_Void) };
  Shizu_Object_call(state, ast, "getNumberOfChildren", sizeof("getNumberOfChildren") - 1, &returnValue, 0, &arguments[0]);
  if (!Shizu_Value_isInteger32(&returnValue)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  return Shizu_Value_getInteger32(&returnValue);
}

static inline Shizu_String* getText(Shizu_State2* state, Reader* reader, Shizu_Object* ast) {
  ensureReaderIsInitialized(state, reader);
  Shizu_Value returnValue;
  Shizu_Value arguments[] = { Shizu_Value_InitializerVoid(Shizu_Void_Void) };
  Shizu_Object_call(state, ast, "getText", sizeof("getText") - 1, &returnValue, 0, &arguments[0]);
  return Shizu_Runtime_Extensions_getStringValue(state, &returnValue);
}

static inline Shizu_Object* getChildByName(Shizu_State2* state, Reader* reader, Shizu_Object* parent, Shizu_String* name) {
  if (!isMap(state, reader, parent)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  for (Shizu_Integer32 i = 0, n = getNumberOfChildren(state, reader, parent); i < n; ++i) {
    Shizu_Object* child = getChildAt(state, reader, parent, i);
    Shizu_Object* key = getChildAt(state, reader, child, 0);
    if (!isName(state, reader, key)) {
      Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
      Shizu_State2_jump(state);
    }
    Shizu_String* keyText = getText(state, reader, key);
    Shizu_Value temporary = Shizu_Value_InitializerObject(name);
    if (Shizu_Object_isEqualTo(state, (Shizu_Object*)keyText, &temporary)) {
      Shizu_Object* value = getChildAt(state, reader, child, 1);
      return value;
    }
  }
  return NULL;
}

#endif // LOADER_H_INCLUDED
