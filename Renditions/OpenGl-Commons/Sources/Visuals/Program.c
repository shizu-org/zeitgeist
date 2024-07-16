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

#include "Visuals/Program.h"

static void
Visuals_Program_visit
  (
    Shizu_State2* state,
    Visuals_Program* self
  );

static Shizu_ObjectTypeDescriptor const Visuals_Program_Type = {
  .postCreateType = NULL,
  .preDestroyType = NULL,
  .visitType = NULL,
  .size = sizeof(Visuals_Program),
  .finalize = NULL,
  .visit = (Shizu_OnVisitCallback*)&Visuals_Program_visit,
  .dispatchSize = sizeof(Visuals_Program_Dispatch),
  .dispatchInitialize = NULL,
  .dispatchUninitialize = NULL,
};

Shizu_defineObjectType("Zeitgeist.Visuals.Program", Visuals_Program, Visuals_Object);

static void
Visuals_Program_visit
  (
    Shizu_State2* state,
    Visuals_Program* self
  )
{
  if (self->vertexProgramSource) {
    Shizu_Gc_visitObject(Shizu_State2_getState1(state), Shizu_State2_getGc(state), (Shizu_Object*)self->vertexProgramSource);
  }
  if (self->fragmentProgramSource) {
    Shizu_Gc_visitObject(Shizu_State2_getState1(state), Shizu_State2_getGc(state), (Shizu_Object*)self->fragmentProgramSource);
  }
}

void
Visuals_Program_construct
  (
    Shizu_State2* state,
    Visuals_Program* self,
    Shizu_String* vertexProgramSource,
    Shizu_String* fragmentProgramSource
  )
{
  Shizu_Type* TYPE = Visuals_Program_getType(state);
  Visuals_Object_construct(state, (Visuals_Object*)self);
  self->vertexProgramSource = vertexProgramSource;
  self->fragmentProgramSource = fragmentProgramSource;
  ((Shizu_Object*)self)->type = TYPE;
}
