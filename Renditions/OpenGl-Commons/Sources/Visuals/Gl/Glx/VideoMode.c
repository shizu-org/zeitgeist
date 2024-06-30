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

#include "Visuals/Gl/Glx/VideoMode.h"

static Shizu_TypeDescriptor const Visuals_Gl_Glx_VideoMode_Type = {
  .postCreateType = NULL,
  .preDestroyType = NULL,
  .visitType = NULL,
  .size = sizeof(Visuals_Gl_Glx_VideoMode),
  .finalize = (Shizu_OnFinalizeCallback*)NULL,
  .visit = NULL,
  .dispatchSize = sizeof(Visuals_Gl_Glx_VideoMode_Dispatch),
  .dispatchInitialize = (Shizu_OnDispatchInitializeCallback*)NULL,
  .dispatchUninitialize = NULL,
};

Shizu_defineType(Visuals_Gl_Glx_VideoMode, Visuals_Object);

void
Visuals_Gl_Glx_VideoMode_construct
  (
    Shizu_State2* state,
    Visuals_Gl_Glx_VideoMode* self,
    Shizu_Integer32 horizontalResolution,
    Shizu_Integer32 verticalResolution,
    Shizu_Integer32 refreshRate
  )
{
  Shizu_Type* TYPE = Visuals_Gl_Glx_VideoMode_getType(state);
  Visuals_Object_construct(state, (Visuals_Object*)self);
  self->horizontalResolution = horizontalResolution;
  self->verticalResolution = verticalResolution;
  self->refreshRate = refreshRate;
  ((Shizu_Object*)self)->type = TYPE;
}
  
Visuals_Gl_Glx_VideoMode*
Visuals_Gl_Glx_VideoMode_create
  (
    Shizu_State2* state,
    Shizu_Integer32 horizontalResolution,
    Shizu_Integer32 verticalResolution,
    Shizu_Integer32 refreshRate
  )
{
  Visuals_Gl_Glx_VideoMode* self = (Visuals_Gl_Glx_VideoMode*)Shizu_Gc_allocateObject(state, sizeof(Visuals_Gl_Glx_VideoMode));
  Visuals_Gl_Glx_VideoMode_construct(state, self, horizontalResolution, verticalResolution, refreshRate);
  return self;
}

#include <X11/extensions/Xrandr.h>

Shizu_List*
Visuals_Gl_Glx_getVideoModes
  (
    Shizu_State2* state,
    Display* display
  )
{
  Shizu_List* list = Shizu_List_create(state);
  //
  int dummy;
  if (!XQueryExtension(display, "RANDR", &dummy, &dummy, &dummy)) {
    return NULL;
  }
  //
  XRRScreenConfiguration* screenConfiguration = XRRGetScreenInfo(display, DefaultRootWindow(display));
  if (!screenConfiguration) {
    return NULL;
  }
  Shizu_JumpTarget jumpTarget;
  Shizu_State2_pushJumpTarget(state, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    //
    Rotation currentRotation;
    int currentScreenSizeId = XRRConfigCurrentConfiguration(screenConfiguration, &currentRotation);
    //
    XRRScreenSize* screenSizes = NULL;
    int numberOfScreenSizes = 0;
    screenSizes = XRRConfigSizes(screenConfiguration, &numberOfScreenSizes);
    //
    Visuals_Gl_Glx_VideoMode* videoMode = NULL;
    videoMode = Visuals_Gl_Glx_VideoMode_create(state, screenSizes[currentScreenSizeId].width,
                                                       screenSizes[currentScreenSizeId].height,
                                                       XRRConfigCurrentRate(screenConfiguration));
    Shizu_List_appendObject(state, list, (Shizu_Object*)videoMode);
    for (int screenSizeId = 0; screenSizeId < numberOfScreenSizes; screenSizeId++) {
        short* rates;
        int numberOfRates = 0;
        rates = XRRConfigRates(screenConfiguration, screenSizeId, &numberOfRates);
        for (int rateId = 0; rateId < numberOfRates; rateId++) {
          videoMode = Visuals_Gl_Glx_VideoMode_create(state, screenSizes[screenSizeId].width,
                                                             screenSizes[screenSizeId].height,
                                                             rates[rateId]);
          Shizu_List_appendObject(state, list, (Shizu_Object*)videoMode);
        }
    }
    XRRFreeScreenConfigInfo(screenConfiguration);
    screenConfiguration = NULL;
    Shizu_State2_popJumpTarget(state);
    return list;
  } else {
    Shizu_State2_popJumpTarget(state);
    XRRFreeScreenConfigInfo(screenConfiguration);
    screenConfiguration = NULL;
    Shizu_State2_jump(state);
  }
}
