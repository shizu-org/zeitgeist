// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#if !defined(ZEITGEIST_RENDITION_H_INCLUDED)
#define ZEITGEIST_RENDITION_H_INCLUDED

#include "Zeitgeist.h"

Shizu_declareType(Zeitgeist_Rendition);

struct Zeitgeist_Rendition {
  Shizu_Object _parent;

  /** The path to the folder of the rendition. */
  Shizu_String* folderPath;

  /** The library handle if the library is loaded. The null pointer otherwise. */
  Shizu_Dl* dl;

  Shizu_String*(*getName)(Shizu_State* state, Zeitgeist_Rendition* rendition);
};

Zeitgeist_Rendition*
Zeitgeist_createRendition
  (
    Shizu_State* state,
    Shizu_String* folderPath
  );

Shizu_String*
Zeitgeist_Rendition_getName
  (
    Shizu_State* state,
    Zeitgeist_Rendition* rendition
  );

Shizu_CxxFunction*
Zeitgeist_Rendition_getUpdate
  (
    Shizu_State* state,
    Zeitgeist_Rendition* rendition
  );

Shizu_CxxFunction*
Zeitgeist_Rendition_getLoad
  (
    Shizu_State* state,
    Zeitgeist_Rendition* rendition
  );

Shizu_CxxFunction*
Zeitgeist_Rendition_getUnload
  (
    Shizu_State* state,
    Zeitgeist_Rendition* rendition
  );

#endif // ZEITGEIST_RENDITION_H_INCLUDED
