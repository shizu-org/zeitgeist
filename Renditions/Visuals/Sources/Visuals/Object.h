/*
  Shizu Visuals
  Copyright (C) 2024 Michael Heilmann. All rights reserved.

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#if !defined(VISUALS_OBJECT_H_INCLUDED)
#define VISUALS_OBJECT_H_INCLUDED

#include "Zeitgeist.h"

Shizu_declareObjectType(Visuals_Object);

struct Visuals_Object_Dispatch {
  Shizu_Object_Dispatch _parent;
  void (*materialize)(Shizu_State2* state, Visuals_Object*);
  void (*unmaterialize)(Shizu_State2* state, Visuals_Object*);
  void (*notifyVisualsShutdown)(Shizu_State2*, Visuals_Object*);
};

struct Visuals_Object {
  Shizu_Object _parent;
};

void
Visuals_Object_construct
  (
    Shizu_State2* state,
    Visuals_Object* self
  );

/// @since 1.0
/// Notify a Visuals.Object before the shutdown of the visuals backend.
/// The Visuals.Object must relinquish all its native resources.
/// The default implementation calls Visuals_Object_unmaterialize.
static inline void
Visuals_Object_notifyVisualsShutdown
  (
    Shizu_State2 *state,
    Visuals_Object* self
  )
{ Shizu_VirtualCall(Visuals_Object, materialize, self); }

static inline void
Visuals_Object_materialize
  (
    Shizu_State2* state,
    Visuals_Object* self
  )
{ Shizu_VirtualCall(Visuals_Object, materialize, self); }

static inline void
Visuals_Object_unmaterialize
  (
    Shizu_State2* state,
    Visuals_Object* self
  )
{ Shizu_VirtualCall(Visuals_Object, unmaterialize, self); }

#endif // VISUALS_OIBJECT_H_INCLUDED
