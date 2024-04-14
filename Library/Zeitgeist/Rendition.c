// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#include "Zeitgeist/Rendition.h"

#include "Zeitgeist.h"

// malloc, free
#include <malloc.h>

// strlen
#include <string.h>

// fprintf, stdio
#include <stdio.h>

static void
Zeitgeist_Rendition_finalize
  (
    Shizu_State* state,
    Zeitgeist_Rendition* rendition
  );

static void
Zeitgeist_Rendition_visit
  (
    Shizu_State* state,
    Zeitgeist_Rendition* rendition
  );

Shizu_defineType(Zeitgeist_Rendition, Shizu_Object);

Shizu_TypeDescriptor const Zeitgeist_Rendition_Type = {
  .staticInitialize = NULL,
  .staticFinalize = NULL,
  .staticVisit = NULL,
  .finalize = (Shizu_OnFinalizeCallback*)&Zeitgeist_Rendition_finalize,
  .visit = (Shizu_OnVisitCallback*)&Zeitgeist_Rendition_visit,
};

static void
Zeitgeist_Rendition_finalize
  (
    Shizu_State* state,
    Zeitgeist_Rendition* self
  )
{ 
  if (self->dll) {
    Shizu_Dll_unref(state, self->dll);
    self->dll = NULL;
  }
}

static void
Zeitgeist_Rendition_visit
  (
    Shizu_State* state,
    Zeitgeist_Rendition* self
  )
{
  if (self->folderPath) {
    Shizu_Gc_visitObject(state, (Shizu_Object*)self->folderPath);
  }
}

static void
Zeitgeist_Rendition_ensureLibraryLoaded
  (
    Shizu_State* state,
    Zeitgeist_Rendition* self
  )
{
  if (!self->dll) {
    Shizu_String* zeroTerminator = Shizu_String_create(state, "", sizeof(char));
    Shizu_String* libraryPath = Shizu_String_create(state, Shizu_OperatingSystem_DirectorySeparator "library" Shizu_OperatingSystem_DllExtension,
                                                    strlen(Shizu_OperatingSystem_DirectorySeparator "library" Shizu_OperatingSystem_DllExtension));
    libraryPath = Shizu_String_concatenate(state, self->folderPath, libraryPath);
    libraryPath = Shizu_String_concatenate(state, libraryPath, zeroTerminator);
    self->dll = Shizu_State_getOrLoadDll(state, Shizu_String_getBytes(state, libraryPath), true);
    if (!self->dll) {
      fprintf(stderr, "unable to link `%.*s`\n", (int)Shizu_String_getNumberOfBytes(state, libraryPath), Shizu_String_getBytes(state, libraryPath));
      Shizu_State_jump(state);
    }
  }
}

Zeitgeist_Rendition*
Zeitgeist_createRendition
  (
    Shizu_State* state,
    Shizu_String* folderPath
  )
{
  Zeitgeist_Rendition* self = (Zeitgeist_Rendition*)Shizu_Gc_allocate(state, sizeof(Zeitgeist_Rendition));
  self->folderPath = folderPath;
  self->dll = NULL;
  ((Shizu_Object*)self)->type = Zeitgeist_Rendition_getType(state);
  return self;
}

Shizu_String*
Zeitgeist_Rendition_getName
  (
    Shizu_State* state,
    Zeitgeist_Rendition* rendition
  )
{
  Zeitgeist_Rendition_ensureLibraryLoaded(state, rendition);
  Shizu_JumpTarget jumpTarget;
  Shizu_State_pushJumpTarget(state, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    Shizu_String* (*getName)(Shizu_State*) = (Shizu_String * (*)(Shizu_State*))Shizu_Dll_getSymbol(state, rendition->dll, "Zeitgeist_Rendition_getName");
    if (!getName) {
      fprintf(stderr, "unable to link `%s` of `%.*s`\n", "Zeitgeist_Rendition_getName", (int)Shizu_String_getNumberOfBytes(state, rendition->folderPath), Shizu_String_getBytes(state, rendition->folderPath));
      Shizu_State_jump(state);
    }
    Shizu_String* name = getName(state);
    Shizu_State_popJumpTarget(state);
    return name;
  } else {
    Shizu_State_popJumpTarget(state);
    Shizu_State_jump(state);
  }
}

Shizu_CxxFunction*
Zeitgeist_Rendition_getUpdate
  (
    Shizu_State* state,
    Zeitgeist_Rendition* rendition
  )
{
  Zeitgeist_Rendition_ensureLibraryLoaded(state, rendition);
  Shizu_JumpTarget jumpTarget;
  Shizu_State_pushJumpTarget(state, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    Shizu_CxxFunction* f = (void (*)(Shizu_State*))Shizu_Dll_getSymbol(state, rendition->dll, "Zeitgeist_Rendition_update");
    if (!f) {
      fprintf(stderr, "unable to link `%s` of `%.*s`\n", "Zeitgeist_Rendition_update", (int)Shizu_String_getNumberOfBytes(state, rendition->folderPath), Shizu_String_getBytes(state, rendition->folderPath));
      Shizu_State_jump(state);
    }
    Shizu_State_popJumpTarget(state);
    return f;
  } else {
    Shizu_State_popJumpTarget(state);
    Shizu_State_jump(state);
  }
}

Shizu_CxxFunction*
Zeitgeist_Rendition_getLoad
  (
    Shizu_State* state,
    Zeitgeist_Rendition* rendition
  )
{
  Zeitgeist_Rendition_ensureLibraryLoaded(state, rendition);
  Shizu_JumpTarget jumpTarget;
  Shizu_State_pushJumpTarget(state, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    Shizu_CxxFunction* f = (void (*)(Shizu_State*))Shizu_Dll_getSymbol(state, rendition->dll, "Zeitgeist_Rendition_load");
    if (!f) {
      fprintf(stderr, "unable to link `%s` of `%.*s`\n", "Zeitgeist_Rendition_load", (int)Shizu_String_getNumberOfBytes(state, rendition->folderPath), Shizu_String_getBytes(state, rendition->folderPath));
      Shizu_State_jump(state);
    }
    Shizu_State_popJumpTarget(state);
    return f;
  } else {
    Shizu_State_popJumpTarget(state);
    Shizu_State_jump(state);
  }
}

Shizu_CxxFunction*
Zeitgeist_Rendition_getUnload
  (
    Shizu_State* state,
    Zeitgeist_Rendition* rendition
  )
{
  Zeitgeist_Rendition_ensureLibraryLoaded(state, rendition);
  Shizu_JumpTarget jumpTarget;
  Shizu_State_pushJumpTarget(state, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    Shizu_CxxFunction* f = (void (*)(Shizu_State*))Shizu_Dll_getSymbol(state, rendition->dll, "Zeitgeist_Rendition_unload");
    if (!f) {
      fprintf(stderr, "unable to link `%s` of `%.*s`\n", "Zeitgeist_Rendition_unload", (int)Shizu_String_getNumberOfBytes(state, rendition->folderPath), Shizu_String_getBytes(state, rendition->folderPath));
      Shizu_State_jump(state);
    }
    Shizu_State_popJumpTarget(state);
    return f;
  } else {
    Shizu_State_popJumpTarget(state);
    Shizu_State_jump(state);
  }
}
