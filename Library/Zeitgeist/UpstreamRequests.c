// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#include "Zeitgeist/UpstreamRequests.h"

#include "Zeitgeist.h"

// malloc, free
#include <malloc.h>

static Shizu_TypeDescriptor const Zeitgeist_UpstreamRequest_Type = {
  .staticInitialize = NULL,
  .staticFinalize = NULL,
  .staticVisit = NULL,
  .finalize = NULL,
  .visit = NULL,
};

Shizu_defineType(Zeitgeist_UpstreamRequest, Shizu_Object);

Zeitgeist_UpstreamRequest*
Zeitgeist_UpstreamRequest_createExitProcessRequest
  (
    Shizu_State* state
  )
{
  Zeitgeist_UpstreamRequest* self = (Zeitgeist_UpstreamRequest*)Shizu_Gc_allocate(state, sizeof(Zeitgeist_UpstreamRequest));
  self->type = Zeitgeist_UpstreamRequestType_ExitProcessRequest;
  ((Shizu_Object*)self)->type = Zeitgeist_UpstreamRequest_getType(state);
  return self;
}

void
Zeitgeist_sendUpstreamRequest
  (
    Shizu_State* state,
    Zeitgeist_UpstreamRequest* request
  )
{ 
  if (Zeitgeist_UpstreamRequestType_ExitProcessRequest == request->type) {
    Shizu_State_setProcessExitRequested(state, Shizu_Boolean_True);
  }
}
