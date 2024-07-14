// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#if !defined(ZEITGEIST_H_INCLUDED)
#define ZEITGEIST_H_INCLUDED

// malloc, free
#include <malloc.h>

// fprintf, stderr
#include <stdio.h>

// exit, EXIT_FAILURE
#include <stdlib.h>

#include "Shizu/Runtime/Include.h"

/** @todo Move into Runtime. */
static inline Shizu_WeakReference*
Shizu_Runtime_Extensions_createWeakReference
  (
    Shizu_State2* state,
    Shizu_Object* reference
  )
{
  Shizu_Value returnValue = Shizu_Value_Initializer();
  Shizu_Value argumentValues[] = { Shizu_Value_Initializer(), Shizu_Value_Initializer() };
  Shizu_Value_setType(&argumentValues[0], Shizu_WeakReference_getType(state));
  if (reference) {
    Shizu_Value_setObject(&argumentValues[1], reference);
  } else {
    Shizu_Value_setVoid(&argumentValues[1], Shizu_Void_Void);
  }
  Shizu_Operations_create(state, &returnValue, 2, &argumentValues[0]);
  return (Shizu_WeakReference*)Shizu_Value_getObject(&returnValue);
}

#endif // ZEITGEIST_H_INCLUDED
