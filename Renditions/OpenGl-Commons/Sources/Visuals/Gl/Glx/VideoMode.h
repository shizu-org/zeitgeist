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

#if !defined(VISUALS_GL_GLX_VIDEOMODE_H_INCLUDED)
#define VISUALS_GL_GLX_VIDEOMODE_H_INCLUDED

#include <X11/Xlib.h>
#include "Visuals/Object.h"

Shizu_declareObjectType(Visuals_Gl_Glx_VideoMode);

struct Visuals_Gl_Glx_VideoMode {
  Visuals_Object _parent;

  /// The horizontal resolution, in pixels.
  Shizu_Integer32 horizontalResolution;

  /// The vertical resolution, in pixels.
  Shizu_Integer32 verticalResolution;

  /// The refresh rate, in Hertz.
  Shizu_Integer32 refreshRate;
};

struct Visuals_Gl_Glx_VideoMode_Dispatch {
  Visuals_Object_Dispatch _parent;
};

/// @brief Construct this Visuals.VideoMode object.
/// @param state A pointer to the Shizu.State2 object.
/// @param self  A pointer to the Shizu.VideoMode object.
/// @param horizontalResolution The horizontal resolution, in pixels.
/// @param verticalResolution The vertical resolution, in pixels.
void
Visuals_Gl_Glx_VideoMode_construct
  (
    Shizu_State2* state,
    Visuals_Gl_Glx_VideoMode* self,
    Shizu_Integer32 horizontalResolution,
    Shizu_Integer32 verticalResolution,
    Shizu_Integer32 refreshRate
  );

Visuals_Gl_Glx_VideoMode*
Visuals_Gl_Glx_VideoMode_create
  (
    Shizu_State2* state,
    Shizu_Integer32 horizontalResolution,
    Shizu_Integer32 verticalResolution,
    Shizu_Integer32 refreshRate
  );

/// @brief Get the list of video modes.
/// @param state A pointer to the Shizu.State2 object.
/// @return The list of video modes.
Shizu_List*
Visuals_Gl_Glx_getVideoModes
  (
    Shizu_State2* state,
    Display* display
  );

#endif // VISUALS_GL_GLX_VIDEOMODE_H_INCLUDED
