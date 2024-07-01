// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#include "Zeitgeist/UpstreamRequests.h"

static Shizu_ObjectTypeDescriptor const Zeitgeist_UpstreamRequest_Type = {
  .preDestroyType = NULL,
  .postCreateType = NULL,
  .visitType = NULL,
  .size = sizeof(Zeitgeist_UpstreamRequest),
  .finalize = NULL,
  .visit = NULL,
  .dispatchSize = sizeof(Zeitgeist_UpstreamRequest_Dispatch),
  .dispatchInitialize = NULL,
  .dispatchUninitialize = NULL,
};

Shizu_defineObjectType(Zeitgeist_UpstreamRequest, Shizu_Object);

Zeitgeist_UpstreamRequest*
Zeitgeist_UpstreamRequest_createExitProcessRequest
  (
    Shizu_State2* state
  )
{
  Zeitgeist_UpstreamRequest* self = (Zeitgeist_UpstreamRequest*)Shizu_Gc_allocateObject(state, sizeof(Zeitgeist_UpstreamRequest));
  self->type = Zeitgeist_UpstreamRequestType_ExitProcessRequest;
  ((Shizu_Object*)self)->type = Zeitgeist_UpstreamRequest_getType(state);
  return self;
}

void
Zeitgeist_sendUpstreamRequest
  (
    Shizu_State2* state,
    Zeitgeist_UpstreamRequest* request
  )
{
  if (Zeitgeist_UpstreamRequestType_ExitProcessRequest == request->type) {
    Shizu_State2_setProcessExitRequested(state, Shizu_Boolean_True);
  }
}
