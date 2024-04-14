// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#include "Zeitgeist/UpstreamRequests.h"

#include "Zeitgeist.h"

// malloc, free
#include <malloc.h>

Zeitgeist_ObjectType const g_Zeitgeist_UpstreamRequest_Type = {
  .name = "Zeitgeist.UpstreamRequest",
  .parentType = &g_Zeitgeist_Object_Type,
  .destruct = NULL,
  .visit = NULL,
};

Zeitgeist_UpstreamRequest*
Zeitgeist_UpstreamRequest_createExitProcessRequest
  (
    Zeitgeist_State* state
  )
{
  Zeitgeist_UpstreamRequest* self = Zeitgeist_allocateObject(state, sizeof(Zeitgeist_UpstreamRequest), NULL, NULL);
  self->type = Zeitgeist_UpstreamRequestType_ExitProcessRequest;
  return self;
}

void
Zeitgeist_sendUpstreamRequest
  (
    Zeitgeist_State* state,
    Zeitgeist_UpstreamRequest* request
  )
{ 
  if (Zeitgeist_UpstreamRequestType_ExitProcessRequest == request->type) {
    state->exitProcessRequested = true;
  }
}
