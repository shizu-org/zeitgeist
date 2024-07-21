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

#if !defined(VISUALS_PIXELFORMAT_H_INCLUDED)
#define VISUALS_PIXELFORMAT_H_INCLUDED

#include "Zeitgeist.h"

/// An enumeration of pixel formats.
Shizu_declareEnumerationType(Visuals_PixelFormat);

enum Visuals_PixelFormat {
  /// A format of a pixel consisting of four components, alpha, blue, green, and red in that order.
  /// Each component is an `uint8_t` value where `0` denotes minimum intensity and `255` denotes maximum intensity.
  /// The default value is opaque black `(255,0,0,0)`.
  Visuals_PixelFormat_ABGR_U8,
  /// Format of a pixel consisting of four components, alpha, red, green, and blue in that order.
  /// Each component is an `uint8_t` value where `0` denotes minimum intensity and `255` denotes maximum intensity.
  /// The default value is opaque black `(255,0,0,0)`.
  Visuals_PixelFormat_ARGB_U8,
  /// Format of a pixel consisting of three components, blue, green, and red in that order.
  /// Each component is an `uint8_t` value where `0` denotes minimum intensity and `255` denotes maximum intensity.
  /// The default value is opaque black `(0,0,0)`.
  Visuals_PixelFormat_BGR_U8,
  /// Format of a pixel consisting of four components, blue, green, red, and alpha in that order.
  /// Each component is an `uint8_t` value where `0` denotes minimum intensity and `255` denotes maximum intensity.
  /// The default value is opaque black `(0,0,0,255)`.
  Visuals_PixelFormat_BGRA_U8,
  /// Format of a pixel consisting of three components, red, green, and blue in that order.
  /// Each component is an `uint8_t` value where `0` denotes minimum intensity and `255` denotes maximum intensity.
  /// The default value is opaque black `(0,0,0)`.
  Visuals_PixelFormat_RGB_U8,
  /// Format of a pixel consisting of four components, red, green, blue, and alpha in that order.
  /// Each component is an `uint8_t` value where `0` denotes minimum intensity and `255` denotes maximum intensity.
  /// The default value is opaque black `(0,0,0,255)`.
  Visuals_PixelFormat_RGBA_U8,
};

/// @brief Get the number of bytes per pixel of this pixel format.
/// @param state The state.
/// @param self The pixel format.
/// @return The number of bytes per pixel.
Shizu_WarnUnusedReturnValue()Shizu_Integer32
Visuals_PixelFormat_getBytesPerPixel
  (
    Shizu_State2* state,
    Visuals_PixelFormat self
  );

/// @brief Get the number of bits per pixel of this pixel format.
/// @param state The state.
/// @param self The pixel format.
/// @return The number of bits per pixel.
Shizu_WarnUnusedReturnValue()Shizu_Integer32
Visuals_PixelFormat_getBitsPixel
  (
    Shizu_State2* state,
    Visuals_PixelFormat self
  );

/// @brief Get the number of components per pixel format.
/// @param state The state.
/// @param self The pixel format.
/// @return The number of components per pixel.
Shizu_WarnUnusedReturnValue()Shizu_Integer32
Visuals_PixelFormat_getComponents
  (
    Shizu_State2* state,
    Visuals_PixelFormat self
  );

#endif // VISUALS_PIXELFORMAT_H_INCLUDED
