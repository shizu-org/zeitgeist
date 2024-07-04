// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#if !defined(ZEITGEIST_UPSTREAMREQUESTS_H_INCLUDED)
#define ZEITGEIST_UPSTREAMREQUESTS_H_INCLUDED

#include "Zeitgeist.h"

/**
 * @since 0.1
 * @brief
 * An upstream request is send from an Rendition to Zeitgeist.
 * The only upstream
 */
Shizu_declareObjectType(Zeitgeist_UpstreamRequest);

/**
 * @since 0.1
 * @brief
 * Symbolic constant that when assigned to Zeitgeist_UpstreamRequest indicates that it is a "exit process request"
 * upstream request.
 */
#define Zeitgeist_UpstreamRequestType_ExitProcessRequest (1)

struct Zeitgeist_UpstreamRequest_Dispatch {
  Shizu_Object_Dispatch _parent;
};

struct Zeitgeist_UpstreamRequest {
  Shizu_Object _parent;
  Shizu_Integer32 type;
};

/**
 * @since 0.1
 * @brief Create a "exit process request" upstream request.
 * @param state A pointer to a Shizu_State2 value.
 * @return A pointer to the Zeitgeist_UpstreamRequest object.
 */
Zeitgeist_UpstreamRequest*
Zeitgeist_UpstreamRequest_createExitProcessRequest
  (
    Shizu_State2* state
  );

/**
 * @since 0.1
 * @brief Send an upstream request.
 * @param state A pointer to a Shizu_State2 value.
 * @param state A pointer to the Zeitgeist_UpstreamRequest value.
 */
void
Zeitgeist_sendUpstreamRequest
  (
    Shizu_State2* state,
    Zeitgeist_UpstreamRequest* request
  );

#endif // ZEITGEIST_UPSTREAMREQUESTS_H_INCLUDED
