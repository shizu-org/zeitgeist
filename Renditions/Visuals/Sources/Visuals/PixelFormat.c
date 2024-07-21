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

#include "Visuals/PixelFormat.h"

Shizu_defineEnumerationType("Zeitgeist.Visuals.PixelFormat", Visuals_PixelFormat);

Shizu_WarnUnusedReturnValue()Shizu_Integer32
Visuals_PixelFormat_getBytesPerPixel
  (
    Shizu_State2* state,
    Visuals_PixelFormat self
  )
{
  switch (self) {
    case Visuals_PixelFormat_ABGR_U8: {
      return 4;
    } break;
    case Visuals_PixelFormat_ARGB_U8: {
      return 4;
    } break;
    case Visuals_PixelFormat_BGR_U8: {
      return 3;
    } break;
    case Visuals_PixelFormat_BGRA_U8: {
      return 4;
    } break;
    case Visuals_PixelFormat_RGB_U8: {
      return 3;
    } break;
    case Visuals_PixelFormat_RGBA_U8: {
      return 4;
    } break;
    default: {
      Shizu_State2_setStatus(state, Shizu_Status_ArgumentValueInvalid);
      Shizu_State2_jump(state);
    } break;
  };
}

Shizu_WarnUnusedReturnValue()Shizu_Integer32
Visuals_PixelFormat_getBitsPixel
  (
    Shizu_State2* state,
    Visuals_PixelFormat self
  )
{ return Visuals_PixelFormat_getBytesPerPixel(state, self) * 8; }

Shizu_WarnUnusedReturnValue()Shizu_Integer32
Visuals_PixelFormat_getComponents
  (
    Shizu_State2* state,
    Visuals_PixelFormat self
  )
{
  switch (self) {
    case Visuals_PixelFormat_ABGR_U8: {
      return 4;
    } break;
    case Visuals_PixelFormat_ARGB_U8: {
      return 4;
    } break;
    case Visuals_PixelFormat_BGR_U8: {
      return 3;
    } break;
    case Visuals_PixelFormat_BGRA_U8: {
      return 4;
    } break;
    case Visuals_PixelFormat_RGB_U8: {
      return 3;
    } break;
    case Visuals_PixelFormat_RGBA_U8: {
      return 4;
    } break;
    default: {
      Shizu_State2_setStatus(state, Shizu_Status_ArgumentValueInvalid);
      Shizu_State2_jump(state);
    } break;
  };
}
