// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#if !defined(ZEITGEIST_RENDITION_H_INCLUDED)
#define ZEITGEIST_RENDITION_H_INCLUDED

#include "Zeitgeist.h"

Shizu_declareObjectType(Zeitgeist_Rendition);

struct Zeitgeist_Rendition_Dispatch {
  Shizu_Object_Dispatch _parent;
};

struct Zeitgeist_Rendition {
  Shizu_Object _parent;

  /** @brief The path to the folder of the rendition. */
  Shizu_String* folderPath;

  /** @brief The library handle if the library is loaded. The null pointer otherwise. */
  Shizu_Dl* dl;

  Shizu_String*(*getName)(Shizu_State2* state, Zeitgeist_Rendition* rendition);
};

Zeitgeist_Rendition*
Zeitgeist_createRendition
  (
    Shizu_State2* state,
    Shizu_String* folderPath
  );

Shizu_String*
Zeitgeist_Rendition_getName
  (
    Shizu_State2* state,
    Zeitgeist_Rendition* rendition
  );

Shizu_CxxFunction*
Zeitgeist_Rendition_getUpdate
  (
    Shizu_State2* state,
    Zeitgeist_Rendition* rendition
  );

Shizu_CxxFunction*
Zeitgeist_Rendition_getLoad
  (
    Shizu_State2* state,
    Zeitgeist_Rendition* rendition
  );

Shizu_CxxFunction*
Zeitgeist_Rendition_getUnload
  (
    Shizu_State2* state,
    Zeitgeist_Rendition* rendition
  );

#endif // ZEITGEIST_RENDITION_H_INCLUDED
