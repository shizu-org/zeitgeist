// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#include "Zeitgeist.h"
#include "Zeitgeist/UpstreamRequests.h"

// strlen
#include <string.h>

// fprintf, stdout
#include <stdio.h>

#if Shizu_Configuration_OperatingSystem_Windows == Shizu_Configuration_OperatingSystem
  #define Shizu_Rendition_Export _declspec(dllexport)
#else
  #define Shizu_Rendition_Export
#endif

Shizu_Rendition_Export char const*
Shizu_ModuleLibrary_getName
  (
  )
{
  static char const* NAME = "<Dl:Hello World>";
  return NAME;
}

Shizu_Rendition_Export Shizu_String*
Zeitgeist_Rendition_getName
  (
    Shizu_State2* state
  )
{ return Shizu_String_create(state, "Hello World", strlen("Hello World")); }

Shizu_Rendition_Export void
Zeitgeist_Rendition_update
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  )
{
  fprintf(stdout, "Hello, World!\n");
  Zeitgeist_UpstreamRequest* request = Zeitgeist_UpstreamRequest_createExitProcessRequest(state);
  Zeitgeist_sendUpstreamRequest(state, request);
}

Shizu_Rendition_Export void
Zeitgeist_Rendition_load
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  )
{/*Intentionally empty.*/}

Shizu_Rendition_Export void
Zeitgeist_Rendition_unload
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  )
{/*Intentionally empty.*/}
