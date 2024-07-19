#include "Zeitgeist.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "idlib/file_system.h"
#include "Zeitgeist/Rendition.h"

static Shizu_String* getWorkingDirectory(Shizu_State2* state) {
  Shizu_Value returnValue;
  Shizu_Value argumentValues[2];
  Shizu_CxxProcedure* p;
  Shizu_Environment* environment = Shizu_Environment_getEnvironment(state, Shizu_State2_getGlobalEnvironment(state), Shizu_String_create(state, "FileSystem", strlen("Filesystem")));
  // Compute the path.
  p = Shizu_Environment_getCxxProcedure(state, environment, Shizu_String_create(state, "getWorkingDirectory", strlen("getWorkingDirectory")));
  p->f(state, &returnValue, 0, argumentValues);
  if (!Shizu_Value_isObject(&returnValue)) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  if (!Shizu_Types_isSubTypeOf(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), Shizu_Value_getObject(&returnValue)->type, Shizu_String_getType(state))) {
    Shizu_State2_setStatus(state, Shizu_Status_ArgumentTypeInvalid);
    Shizu_State2_jump(state);
  }
  Shizu_String* path = (Shizu_String*)Shizu_Value_getObject(&returnValue);
  return path;
}

typedef struct LoadRenditionsContext {
  Shizu_State2* state;
  Shizu_List* list;
  Shizu_String* prefix;
} LoadRenditionsContext;

static bool loadRenditionsCallback(LoadRenditionsContext* context, char const* bytes, size_t numberOfBytes) {
  Shizu_JumpTarget jumpTarget;
  Shizu_State2_pushJumpTarget(context->state, &jumpTarget);
  bool result = true;
  if (!setjmp(jumpTarget.environment)) {
    Shizu_String* string = Shizu_String_create(context->state, bytes, numberOfBytes);
    string = Shizu_String_concatenate(context->state, context->prefix, string);
    Zeitgeist_Rendition* rendition = Zeitgeist_createRendition(context->state, string);
    Shizu_List_appendObject(context->state, context->list, (Shizu_Object*)rendition);
    Shizu_State2_popJumpTarget(context->state);
  } else {
    Shizu_State2_popJumpTarget(context->state);
    return false;
  }
  return true;
}

static Shizu_List*
loadRenditions
  (
    Shizu_State2* state
  )
{
  Shizu_String *path = getWorkingDirectory(state);
  path = Shizu_String_concatenate(state, path, Shizu_String_create(state, Shizu_OperatingSystem_DirectorySeparator "Renditions", strlen(Shizu_OperatingSystem_DirectorySeparator "Renditions")));


  Shizu_List* renditions = Shizu_Runtime_Extensions_createList(state);
  LoadRenditionsContext context;
  context.state = state;
  context.list = renditions;
  context.prefix = Shizu_String_concatenate(state, path, Shizu_String_create(state, Shizu_OperatingSystem_DirectorySeparator, strlen(Shizu_OperatingSystem_DirectorySeparator)));
  if (idlib_enumerate_files(Shizu_String_getBytes(state, Shizu_String_concatenate(state, path, Shizu_String_create(state, "", 1))), &context, &loadRenditionsCallback, true, true)) {
    Shizu_State2_setStatus(state, Shizu_Status_EnvironmentFailed);
    Shizu_State2_jump(state);
  }
  return renditions;
}

// command implementation: "--list-renditions"
static void
onListRenditions
  (
    Shizu_State2* state
  )
{
  Shizu_List* renditions = loadRenditions(state);
  for (size_t i = 0, n = Shizu_List_getSize(state, renditions); i < n; ++i) {
    Shizu_Value value = Shizu_List_getValue(state, renditions, i);
    if (!Shizu_Value_isObject(&value)) {
      Shizu_State2_setStatus(state, 1);
      Shizu_State2_jump(state);
    }
    Zeitgeist_Rendition* rendition = (Zeitgeist_Rendition*)Shizu_Value_getObject(&value);
    Shizu_String* name = Zeitgeist_Rendition_getName(state, rendition);
    fprintf(stdout, "%zu) `%.*s`\n", i + 1, (int)Shizu_String_getNumberOfBytes(state, name), Shizu_String_getBytes(state, name));
  }
}

static void
onRendition1
  (
    Shizu_State2* state,
    Zeitgeist_Rendition* rendition
  )
{ 
  Shizu_String* renditionName = Zeitgeist_Rendition_getName(state, rendition);
  Shizu_CxxFunction* loadFunction = Zeitgeist_Rendition_getLoad(state, rendition);
  if (!loadFunction) {
    fprintf(stderr, "unable to acquire unload function of rendition `%.*s`\n", (int)Shizu_String_getNumberOfBytes(state, renditionName), Shizu_String_getBytes(state, renditionName));
    Shizu_State2_jump(state);
  }
  Shizu_CxxFunction* unloadFunction = Zeitgeist_Rendition_getUnload(state, rendition);
  if (!unloadFunction) {
    fprintf(stderr, "unable to acquire unload function of rendition `%.*s`\n", (int)Shizu_String_getNumberOfBytes(state, renditionName), Shizu_String_getBytes(state, renditionName));
    Shizu_State2_jump(state);
  }
  Shizu_CxxFunction* updateFunction = Zeitgeist_Rendition_getUpdate(state, rendition);
  if (!updateFunction) {
    fprintf(stderr, "unable to acquire update function of rendition `%.*s`\n", (int)Shizu_String_getNumberOfBytes(state, renditionName), Shizu_String_getBytes(state, renditionName));
    Shizu_State2_jump(state);
  }
  Shizu_Value returnValue;
  Shizu_Value argumentValues[] = { Shizu_Value_InitializerVoid(Shizu_Void_Void) };
  (*loadFunction)(state, &returnValue, 0, &argumentValues[0]);
  Shizu_JumpTarget jumpTarget1;
  Shizu_State2_pushJumpTarget(state, &jumpTarget1);
  if (!setjmp(jumpTarget1.environment)) {
    Shizu_Stack_pushObject(Shizu_State2_getState1(state), Shizu_State2_getStack(state), (Shizu_Object*)rendition);
    Shizu_JumpTarget jumpTarget2;
    Shizu_State2_pushJumpTarget(state, &jumpTarget2);
    if (!setjmp(jumpTarget1.environment)) {
      while (!Shizu_State2_getProcessExitRequested(state)) {
        (*updateFunction)(state, &returnValue, 0, &argumentValues[0]);
        Shizu_Gc_run(state, Shizu_State2_getGc(state), NULL);
      }
      Shizu_State2_popJumpTarget(state);
      (*unloadFunction)(state, &returnValue, 0, &argumentValues[0]);
    } else {
      Shizu_State2_popJumpTarget(state);
      (*unloadFunction)(state, &returnValue, 0, &argumentValues[0]);
      Shizu_State2_jump(state);
    }
    Shizu_State2_popJumpTarget(state);
    Shizu_Stack_pop(Shizu_State2_getState1(state), Shizu_State2_getStack(state));
  } else {
    Shizu_State2_popJumpTarget(state);
    Shizu_Stack_pop(Shizu_State2_getState1(state), Shizu_State2_getStack(state));
    Shizu_State2_jump(state);
  }
}

// command implementation: "--rendition <name>"
static void
onRendition
  (
    Shizu_State2* state,
    Shizu_String* renditionName
  )
{ 
  Shizu_List* loadedRenditions = loadRenditions(state);
  for (size_t i = 0, n = Shizu_List_getSize(state, loadedRenditions); i < n; ++i) {
    Shizu_Value value = Shizu_List_getValue(state, loadedRenditions, i);
    if (!Shizu_Value_isObject(&value)) {
      Shizu_State2_setStatus(state, 1);
      Shizu_State2_jump(state);
    }
    Zeitgeist_Rendition* loadedRendition = (Zeitgeist_Rendition*)Shizu_Value_getObject(&value);
    Shizu_String* loadedRenditionName = Zeitgeist_Rendition_getName(state, loadedRendition);
    Shizu_Value temporary = Shizu_Value_InitializerObject(loadedRenditionName);
    if (Shizu_Object_isEqualTo(state, (Shizu_Object*)renditionName, &temporary)) {
      onRendition1(state, loadedRendition);
      return;
    }
  }
}

static void
onHelp
  (
    Shizu_State2* state
  )
{
  fprintf(stdout, "usage: zeitgeist-interpreter [--rendition <name> ] [--list-renditions] [--help]\n");
  fprintf(stdout, "--rendition <name> Execute rendition by its name\n");
  fprintf(stdout, "--list-renditions List names of all available renditions\n");
  fprintf(stdout, "--help Show this help\n");
}

static void
main1
  (
    Shizu_State2* state,
    int argc,
    char** argv
  )
{
  Shizu_Value listRenditions = Shizu_Value_InitializerObject(Shizu_String_create(state, "--list-renditions", strlen("--list-renditions")));
  Shizu_Value rendition = Shizu_Value_InitializerObject(Shizu_String_create(state, "--rendition", strlen("--rendition")));
  Shizu_Value help = Shizu_Value_InitializerObject(Shizu_String_create(state, "--help", strlen("--help")));
  if (argc < 2) {
    fprintf(stderr, "error: no command specified\n");
    Shizu_State2_jump(state);
  }
  for (int argi = 1; argi < argc; ++argi) {
    Shizu_String* arg = Shizu_String_create(state, argv[argi], strlen(argv[argi]));
    if (Shizu_String_getNumberOfBytes(state, arg) > 128) {
      fprintf(stderr, "error: command `%.*s` too long\n", (int)64, Shizu_String_getBytes(state, arg));
      Shizu_State2_jump(state);
    }
    if (Shizu_Object_isEqualTo(state, (Shizu_Object*)arg, &listRenditions)) {
      if (argc != 2) {
        fprintf(stderr, "error: unknown arguments to command `%.*s`\n", (int)Shizu_String_getNumberOfBytes(state, arg), Shizu_String_getBytes(state, arg));
        Shizu_State2_jump(state);
      }
      fprintf(stdout, "listing renditions\n");
      onListRenditions(state);
      break;
    } else if (Shizu_Object_isEqualTo(state, (Shizu_Object*)arg, &rendition)) {
      if (argc != 3) {
        fprintf(stderr, "error: missing argument for command `%.*s`\n", (int)Shizu_String_getNumberOfBytes(state, arg), Shizu_String_getBytes(state, arg));
        Shizu_State2_jump(state);
      }
      fprintf(stdout, "executing rendition\n");
      onRendition(state, Shizu_String_create(state, argv[argi + 1], strlen(argv[argi + 1])));
      break;
    } else if (Shizu_Object_isEqualTo(state, (Shizu_Object*)arg, &help)) {
      onHelp(state);
      Shizu_State2_setProcessExitRequested(state, Shizu_Boolean_True);
    } else {
      fprintf(stderr, "error: unknown command `%.*s`\n", (int)Shizu_String_getNumberOfBytes(state, arg), Shizu_String_getBytes(state, arg));
      Shizu_State2_jump(state);
    }
  }
}

int
main
  (
    int argc,
    char** argv
  )
{
  Shizu_State2* state = NULL;
  if (Shizu_State2_acquire(&state)) {
    return EXIT_FAILURE;
  }
  int exitCode = EXIT_SUCCESS;
  Shizu_JumpTarget jumpTarget;
  Shizu_State2_pushJumpTarget(state, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    Shizu_State2_ensureModulesLoaded(state);
    main1(state, argc, argv);
    exitCode = EXIT_SUCCESS;
    Shizu_State2_popJumpTarget(state);
  } else {
    exitCode = EXIT_FAILURE;
    Shizu_State2_popJumpTarget(state);
  }
  Shizu_State2_relinquish(state);
  state = NULL;
  return exitCode;
}
