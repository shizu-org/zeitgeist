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

#include "Visuals/RenderBuffer.h"

static Shizu_TypeDescriptor const Visuals_RenderBuffer_Type = {
  .postCreateType = NULL,
  .preDestroyType = NULL,
  .visitType = NULL,
  .size = sizeof(Visuals_RenderBuffer),
  .finalize = NULL,
  .visit = NULL,
  .dispatchSize = sizeof(Visuals_RenderBuffer_Dispatch),
  .dispatchInitialize = NULL,
  .dispatchUninitialize = NULL, 
};

Shizu_defineType(Visuals_RenderBuffer, Visuals_Object);

void
Visuals_RenderBuffer_construct
  (
    Shizu_State* state,
    Visuals_RenderBuffer* self
  )
{
  Shizu_Type* type = Visuals_RenderBuffer_getType(state);
  ((Shizu_Object*)self)->type = type;
}
