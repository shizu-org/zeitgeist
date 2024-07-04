/*
  Zeitgeist
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

#include "ColorRGBU8.h"

static Shizu_ObjectTypeDescriptor const ColorRGBU8_Type = {
  .postCreateType = NULL,
  .preDestroyType = NULL,
  .visitType = NULL,
  .size = sizeof(ColorRGBU8),
  .finalize = NULL,
  .visit = NULL,
  .dispatchSize = sizeof(ColorRGBU8_Dispatch),
  .dispatchInitialize = NULL,
  .dispatchUninitialize = NULL,
};

Shizu_defineObjectType(ColorRGBU8, Shizu_Object);

void
ColorRGBU8_construct
  (
    Shizu_State2* state,
    ColorRGBU8* self,
    Shizu_Integer32 r,
    Shizu_Integer32 g,
    Shizu_Integer32 b
  )
{
  Shizu_Type* TYPE = ColorRGBU8_getType(state);
  Shizu_Object_construct(state, (Shizu_Object*)self);
  self->r = r;
  self->g = g;
  self->b = b;
  ((Shizu_Object*)self)->type = TYPE;
}

ColorRGBU8*
ColorRGBU8_create
  (
    Shizu_State2* state,
    Shizu_Integer32 r,
    Shizu_Integer32 g,
    Shizu_Integer32 b
  )
{
  Shizu_Type* TYPE = ColorRGBU8_getType(state);
  ColorRGBU8* self = (ColorRGBU8*)Shizu_Gc_allocateObject(state, sizeof(ColorRGBU8));
  ColorRGBU8_construct(state, self, r, g, b);
  return self;
}
