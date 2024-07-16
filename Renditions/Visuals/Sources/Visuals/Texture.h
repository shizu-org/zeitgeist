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

#if !defined(VISUALS_TEXTURE_H_INCLUDED)
#define VISUALS_TEXTURE_H_INCLUDED

#include "Visuals/Object.h"

Shizu_declareEnumerationType(Visuals_PixelFormat);

enum Visuals_PixelFormat {
  Visuals_PixelFormat_ABGR8,
  Visuals_PixelFormat_ARGBU8,
  Visuals_PixelFormat_BGRAU8,
  Visuals_PixelFormat_BGRU8,
  Visuals_PixelFormat_RGBAU8,
  Visuals_PixelFormat_RGBU8,
};

/// @brief
/// A texture.
/// The type is
/// @code
/// class Visuals.Texture
/// @endcode
/// Its constructor is
/// @code
/// Visuals.Texture.construct()
/// @endcode
Shizu_declareObjectType(Visuals_Texture);

struct Visuals_Texture_Dispatch {
  Visuals_Object_Dispatch _parent;
  void (*setData)(Shizu_State2* state, Visuals_Texture* self, Visuals_PixelFormat pixelFormat, Shizu_Integer32 width, Shizu_Integer32 height, Shizu_ByteArray* pixels);
  Visuals_PixelFormat (*getPixelFormat)(Shizu_State2* state, Visuals_Texture* self);
  Shizu_Integer32 (*getWidth)(Shizu_State2* state, Visuals_Texture* self);
  Shizu_Integer32 (*getHeight)(Shizu_State2* state, Visuals_Texture* self);
};

struct Visuals_Texture {
  Visuals_Object parent;
};

static inline void
Visuals_Texture_setData
  (
    Shizu_State2* state,
    Visuals_Texture* self,
    Visuals_PixelFormat pixelFormat,
    Shizu_Integer32 width,
    Shizu_Integer32 height,
    Shizu_ByteArray* pixels
  )
{ Shizu_VirtualCall(Visuals_Texture, setData, self, pixelFormat, width, height, pixels); }

static inline Visuals_PixelFormat
Visuals_Texture_gePixelFormat
  (
    Shizu_State2* state,
    Visuals_Texture* self
  )
{ Shizu_VirtualCallWithReturn(Visuals_Texture, getPixelFormat, self); }

static inline Shizu_Integer32
Visuals_Texture_getWidth
  (
    Shizu_State2* state,
    Visuals_Texture* self
  )
{ Shizu_VirtualCallWithReturn(Visuals_Texture, getWidth, self); }

static inline Shizu_Integer32
Visuals_Texture_getHeight
  (
    Shizu_State2* state,
    Visuals_Texture* self
  )
{ Shizu_VirtualCallWithReturn(Visuals_Texture, getHeight, self); }

#endif // VISUALS_TEXTURE_H_INCLUDED
