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

#include "Visuals/Gl/Glx/Service.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <GL/gl.h>
#include <GL/glx.h>

#include <X11/Xatom.h>

// Service_emitKeyboardKeyMessage
#include "Visuals/Service.h"
#include "KeyboardKeyMessage.h"

typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

static int (*g_oldErrorHandler)(Display*, XErrorEvent*) = NULL;
static bool g_error = false;
static Display* g_display = NULL;
static Window g_window;
static GLXFBConfig g_glx_bestFbConfig;
static Colormap g_colorMap;
static GLXContext g_context;
static Atom WM_DELETE_WINDOW;

static bool g_quitRequested = false;

// Helper to check for extension string presence.  Adapted from:
// http://www.opengl.org/resources/features/OGLextensions/
static bool
isExtensionSupported
  (
    const char *extensions,
    const char *extension
  );

static int
getBestFbc
  (
    Shizu_State2* state,
    GLXFBConfig* configs,
    int numberOfConfigs
  );

static int
errorHandler
  (
    Display* display,
    XErrorEvent*
  );

static void
shutdownContext
  (
    Shizu_State2* state
  );

static void
startupWindow
  (
    Shizu_State2* state
  );

static void
shutdownWindow
  (
    Shizu_State2* state
  );

static Shizu_Value
mapKeyboardKey
  (
    XEvent *event
  );

// Helper to check for extension string presence.  Adapted from:
// http://www.opengl.org/resources/features/OGLextensions/
static bool
isExtensionSupported
  (
    const char *extensions,
    const char *extension
  )
{
  const char *start;
  const char *where, *terminator;
  
  /* Extension names should not have spaces. */
  where = strchr(extension, ' ');
  if (where || *extension == '\0')
    return false;

  /* It takes a bit of care to be fool-proof about parsing the
     OpenGL extensions string. Don't be fooled by sub-strings,
     etc. */
  for (start = extensions;;) {
    where = strstr(start, extension);

    if (!where)
      break;

    terminator = where + strlen(extension);

    if ( where == start || *(where - 1) == ' ' )
      if ( *terminator == ' ' || *terminator == '\0' )
        return true;

    start = terminator;
  }

  return false;
}

static int
getBestFbc
  (
    Shizu_State2* state,
    GLXFBConfig* configs,
    int numberOfConfigs
  )
{
  int best = -1, worst = -1, best_num_samp = -1, worst_num_samp = 999;
  int current;
  for (current = 0; current < numberOfConfigs; ++current) {
    XVisualInfo *visual = glXGetVisualFromFBConfig(g_display, configs[current]);
    if (visual) {
      int samp_buf, samples;
      glXGetFBConfigAttrib( g_display, configs[current], GLX_SAMPLE_BUFFERS, &samp_buf);
      glXGetFBConfigAttrib( g_display, configs[current], GLX_SAMPLES, &samples);
      if ( best < 0 || samp_buf && samples > best_num_samp )
        best = current, best_num_samp = samples;
      if ( worst < 0 || !samp_buf || samples < worst_num_samp )
        worst = current, worst_num_samp = samples;
    }
    XFree(visual);
  }
  return best;
}

static int
errorHandler
  (
    Display* display,
    XErrorEvent*
  )
{
  g_error = true;
}

static void
startupContext
  (
    Shizu_State2* state
  )
{
  g_oldErrorHandler = XSetErrorHandler(&errorHandler);

  const char *extensions = glXQueryExtensionsString( g_display,
                                                     DefaultScreen( g_display ) );
  if (!extensions) {
    fprintf(stderr, "%s:%d: unable to get extension strings\n", __FILE__, __LINE__);
    XSetErrorHandler(g_oldErrorHandler);
    Shizu_State2_setStatus(state, 1);
    Shizu_State2_jump(state);
  }

  // NOTE: It is not necessary to create or make current to a context before calling glXGetProcAddressARB.
  glXCreateContextAttribsARBProc glXCreateContextAttribsARB = NULL;
  glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddressARB((GLubyte const *)"glXCreateContextAttribsARB");
  if (!glXCreateContextAttribsARB) {
    fprintf(stderr, "%s:%d: unable to get %s\n", __FILE__, __LINE__, "glXCreateContextAttribsARB");
    XSetErrorHandler(g_oldErrorHandler);
    Shizu_State2_setStatus(state, 1);
    Shizu_State2_jump(state);
  }
  
  // Check for the GLX_ARB_create_context extension string and the function.
  // If either is not present, use GLX 1.3 context creation method.
  if (!isExtensionSupported(extensions, "GLX_ARB_create_context") || !glXCreateContextAttribsARB) {
    fprintf(stderr, "%s:%d: unable to get %s extension\n", __FILE__, __LINE__, "GLX_ARB_create_context  / glXCreateContextAttribsARB");
    XSetErrorHandler(g_oldErrorHandler);
    Shizu_State2_setStatus(state, 1);
    Shizu_State2_jump(state);
  }
  
  int contextAttribs[] = {
    GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
    GLX_CONTEXT_MINOR_VERSION_ARB, 0,
    //GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
    None
  };
  g_context = glXCreateContextAttribsARB(g_display, g_glx_bestFbConfig, 0,
                                         True, contextAttribs );
  XSync(g_display, False);
  if (!g_context) {
    fprintf(stderr, "%s:%d: unable to get create context\n", __FILE__, __LINE__);
    XSetErrorHandler(g_oldErrorHandler);
    Shizu_State2_setStatus(state, 1);
    Shizu_State2_jump(state);
  }
  
  if (!glXIsDirect(g_display, g_context)) {
    fprintf(stderr, "%s:%d: unable to get create context\n", __FILE__, __LINE__);
    glXDestroyContext(g_display, g_context);
    g_context = NULL;
    XSetErrorHandler(g_oldErrorHandler);
    Shizu_State2_setStatus(state, 1);
    Shizu_State2_jump(state);
  }
  
  if (!glXMakeCurrent(g_display, g_window, g_context)) {
    fprintf(stderr, "%s:%d: unable to make context current\n", __FILE__, __LINE__);
    glXDestroyContext(g_display, g_context);
    g_context = NULL;
    XSetErrorHandler(g_oldErrorHandler);
    Shizu_State2_setStatus(state, 1);
    Shizu_State2_jump(state);
  }
  
  XSetErrorHandler(g_oldErrorHandler);
}

static void
shutdownContext
  (
    Shizu_State2* state
  )
{
  glXMakeCurrent(g_display, 0, 0);
  glXDestroyContext(g_display, g_context);
  g_context = NULL;
}

static void
startupWindow
  (
    Shizu_State2* state
  )
{ 
  g_oldErrorHandler = XSetErrorHandler(&errorHandler);

  g_display = XOpenDisplay(NULL);
  if (!g_display) {
    fprintf(stderr, "%s:%d: unable to open display\n", __FILE__, __LINE__);
    XSetErrorHandler(g_oldErrorHandler);
    Shizu_State2_setStatus(state, 1);
    Shizu_State2_jump(state);
  }
  // Get a matching FB config
  static int glx_visualAttributes[] = {
    GLX_X_RENDERABLE    , True,
    GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
    GLX_RENDER_TYPE     , GLX_RGBA_BIT,
    GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
    GLX_RED_SIZE        , 8,
    GLX_GREEN_SIZE      , 8,
    GLX_BLUE_SIZE       , 8,
    GLX_ALPHA_SIZE      , 8,
    GLX_DEPTH_SIZE      , 24,
    GLX_STENCIL_SIZE    , 8,
    GLX_DOUBLEBUFFER    , True,
    //GLX_SAMPLE_BUFFERS, 1,
    //GLX_SAMPLES       , 4,
    None
  };
  int glx_major, glx_minor;
 
  // FBConfigs were added in GLX version 1.3.
  if (!glXQueryVersion( g_display, &glx_major, &glx_minor)) {
    XCloseDisplay(g_display);
    g_display = NULL;
    fprintf(stderr, "%s:%d: unable to get GLX version\n", __FILE__, __LINE__);
    XSetErrorHandler(g_oldErrorHandler);
    Shizu_State2_setStatus(state, 1);
    Shizu_State2_jump(state);
  }
  if ((glx_major == 1 && glx_minor < 3) || (glx_major < 1)) {
    XCloseDisplay(g_display);
    g_display = NULL;
    fprintf(stderr, "%s:%d: GLX version %d.%d smaller than the minimum required version %d.%d\n", __FILE__, __LINE__, glx_major, glx_minor, 1, 3);
    XSetErrorHandler(g_oldErrorHandler);
    Shizu_State2_setStatus(state, 1);
    Shizu_State2_jump(state);
  }
  
  int glx_fbConfigCount;
  GLXFBConfig* glx_fbConfigs = glXChooseFBConfig(g_display, DefaultScreen(g_display), glx_visualAttributes, &glx_fbConfigCount);
  if (!glx_fbConfigs) {
    XCloseDisplay(g_display);
    g_display = NULL;
    fprintf(stderr, "%s:%d: unable to get matching frame buffer configuration(s)\n", __FILE__, __LINE__);
    XSetErrorHandler(g_oldErrorHandler);
    Shizu_State2_setStatus(state, 1);
    Shizu_State2_jump(state);
  }
  //
  int glx_bestFbConfigIndex = getBestFbc(state, glx_fbConfigs, glx_fbConfigCount);
  if (-1 == glx_bestFbConfigIndex) {
    XFree(glx_fbConfigs);
    glx_fbConfigs = NULL;
    XCloseDisplay(g_display);
    g_display = NULL;
    fprintf(stderr, "%s:%d: unable to get matching frame buffer configuration(s)\n", __FILE__, __LINE__);
    XSetErrorHandler(g_oldErrorHandler);
    Shizu_State2_setStatus(state, 1);
    Shizu_State2_jump(state);
  }
  g_glx_bestFbConfig = glx_fbConfigs[ glx_bestFbConfigIndex ];
  
  //
  XFree(glx_fbConfigs);
  glx_fbConfigs = NULL;
  
  XVisualInfo *x_visualInfo = glXGetVisualFromFBConfig( g_display, g_glx_bestFbConfig );
  if (NULL == x_visualInfo) {
    XCloseDisplay(g_display);
    g_display = NULL;
    fprintf(stderr, "%s:%d: unable to get matching visual\n", __FILE__, __LINE__);
    XSetErrorHandler(g_oldErrorHandler);
    Shizu_State2_setStatus(state, 1);
    Shizu_State2_jump(state);
  }
  
  XSetWindowAttributes swa;
  swa.colormap = g_colorMap = XCreateColormap( g_display,
                                               RootWindow( g_display, x_visualInfo->screen ), 
                                               x_visualInfo->visual, AllocNone );
  if (g_error) {
    XFree(x_visualInfo);
    x_visualInfo = NULL;
    XCloseDisplay(g_display);
    g_display = NULL;
    fprintf(stderr, "%s:%d: unable to create a color map\n", __FILE__, __LINE__);
    XSetErrorHandler(g_oldErrorHandler);
    Shizu_State2_setStatus(state, 1);
    Shizu_State2_jump(state);
  }
  swa.background_pixmap = None ;
  swa.border_pixel      = 0;
  swa.event_mask        = StructureNotifyMask;
  
  g_window = XCreateWindow( g_display, RootWindow( g_display, x_visualInfo->screen ), 
                            0, 0, 640, 480, 0, x_visualInfo->depth, InputOutput, 
                            x_visualInfo->visual, 
                            CWBorderPixel|CWColormap|CWEventMask, &swa );
  if (g_error) {
    XFree(x_visualInfo);
    x_visualInfo = NULL;
    XCloseDisplay(g_display);
    g_display = NULL;
    fprintf(stderr, "%s:%d: unable to create window\n", __FILE__, __LINE__);
    XSetErrorHandler(g_oldErrorHandler);
    Shizu_State2_setStatus(state, 1);
    Shizu_State2_jump(state);
  }
  
  XFree(x_visualInfo);
  x_visualInfo = NULL;
  
  WM_DELETE_WINDOW = XInternAtom(g_display, "WM_DELETE_WINDOW", False); 
  XSetWMProtocols(g_display, g_window, &WM_DELETE_WINDOW, 1);  
  
  XMapWindow( g_display, g_window );
  
  XSync(g_display, False);
  
  // We are only interested in keyboard events
  long eventMask = 0;
  eventMask |= KeyPressMask | KeyReleaseMask;
  eventMask |= ButtonPressMask | ButtonReleaseMask;
  eventMask |= PointerMotionMask;
  // Set the input mask for our window on the current display
  XSelectInput(g_display, g_window, eventMask);


  XSetErrorHandler(g_oldErrorHandler);
  
  g_quitRequested = false;
}

static void
shutdownWindow
  (
    Shizu_State2* state
  )
{ 
  XDestroyWindow(g_display, g_window);
  XFreeColormap(g_display, g_colorMap);
  XCloseDisplay(g_display);
  g_display = NULL;
}

static Shizu_Value
mapKeyboardKey
  (
    XEvent *event
  )
{
  KeySym keySym = XLookupKeysym(&event->xkey, 0);
  switch (keySym) {
    case XK_Escape: {
      Shizu_Value value;
      Shizu_Value_setInteger32(&value, KeyboardKey_Escape);
      return value;
    } break;
    case XK_Up: {
      Shizu_Value value;
      Shizu_Value_setInteger32(&value, KeyboardKey_Up);
      return value;
    } break;
    case XK_Down: {
      Shizu_Value value;
      Shizu_Value_setInteger32(&value, KeyboardKey_Down);
      return value;
    } break;
    case XK_Left: {
      Shizu_Value value;
      Shizu_Value_setInteger32(&value, KeyboardKey_Left);
      return value;
    } break;
    case XK_Right: {
      Shizu_Value value;
      Shizu_Value_setInteger32(&value, KeyboardKey_Right);
      return value;
    } break;
    case XK_a:
    case XK_A: {
      Shizu_Value value;
      Shizu_Value_setInteger32(&value, KeyboardKey_A);
      return value;
    } break;
    case XK_d:
    case XK_D: {
      Shizu_Value value;
      Shizu_Value_setInteger32(&value, KeyboardKey_D);
      return value;
    } break;
    case XK_e:
    case XK_E: {
      Shizu_Value value;
      Shizu_Value_setInteger32(&value, KeyboardKey_E);
      return value;
    } break;
    case XK_q:
    case XK_Q: {
      Shizu_Value value;
      Shizu_Value_setInteger32(&value, KeyboardKey_Q);
      return value;
    } break;
    case XK_s:
    case XK_S: {
      Shizu_Value value;
      Shizu_Value_setInteger32(&value, KeyboardKey_S);
      return value;
    } break;
    case XK_w:
    case XK_W: {
      Shizu_Value value;
      Shizu_Value_setInteger32(&value, KeyboardKey_W);
      return value;
    } break;
    default: {
      Shizu_Value value;
      Shizu_Value_setVoid(&value, Shizu_Void_Void);
      return value;
    } break;
  };
}

void
Visuals_Gl_Glx_Service_startup
  (
    Shizu_State2* state
  )
{ 
  startupWindow(state);
  startupContext(state);
}

void
Visuals_Gl_Glx_Service_shutdown
  (
    Shizu_State2* state
  )
{
  shutdownContext(state);
  shutdownWindow(state);
}

void
Visuals_Gl_Glx_Service_setTitle
  (
    Shizu_State2* state,
    Shizu_String* title
  )
{
  Shizu_String* zeroTerminator = Shizu_String_create(state, "", 1);
  title = Shizu_String_concatenate(state, title, zeroTerminator);
  
  XTextProperty prop;
  prop.value = (char*)Shizu_String_getBytes(state, title);
  prop.encoding = XA_STRING;
  prop.format = 8;
  prop.nitems = Shizu_String_getNumberOfBytes(state, title) - 1;
  g_oldErrorHandler = XSetErrorHandler(&errorHandler);
  XSetWMName(g_display, g_window, &prop);
  XStoreName(g_display, g_window, Shizu_String_getBytes(state, title));
  XSync(g_display, False);
  if (g_error) {
    fprintf(stderr, "failed to set title\n");
  }
  XSetErrorHandler(g_oldErrorHandler);
}

void
Visuals_Gl_Glx_Service_update
  (
    Shizu_State2* state
  )
{
  XEvent e;
  while (XPending(g_display)) {
    XNextEvent(g_display, &e);
    switch (e.type) {
      case ClientMessage: {
        if (e.xclient.data.l[0] == WM_DELETE_WINDOW) {
          g_quitRequested = true;
        }
      } break;
      case KeyPress: {
        Shizu_JumpTarget jumpTarget;
        Shizu_State2_pushJumpTarget(state, &jumpTarget);
        if (!setjmp(jumpTarget.environment)) {
          Shizu_Value mappedKey = mapKeyboardKey(&e);
          if (Shizu_Value_isInteger32(&mappedKey)) {
            KeyboardKeyMessage* message = KeyboardKeyMessage_create(state, KeyboardKey_Action_Pressed, Shizu_Value_getInteger32(&mappedKey));
            Visuals_Service_emitKeyboardKeyMessage(state, message);
          }
          Shizu_State2_popJumpTarget(state);
        } else {
          Shizu_State2_popJumpTarget(state);
        }
      } break;
      case KeyRelease: {
        Shizu_JumpTarget jumpTarget;
        Shizu_State2_pushJumpTarget(state, &jumpTarget);
        if (!setjmp(jumpTarget.environment)) {
          Shizu_Value mappedKey = mapKeyboardKey(&e);
          if (Shizu_Value_isInteger32(&mappedKey)) {
            KeyboardKeyMessage* message = KeyboardKeyMessage_create(state, KeyboardKey_Action_Released, Shizu_Value_getInteger32(&mappedKey));
            Visuals_Service_emitKeyboardKeyMessage(state, message);
          }
          Shizu_State2_popJumpTarget(state);
        } else {
          Shizu_State2_popJumpTarget(state);
        }
      } break;
    };
  }
}

Shizu_Boolean
Visuals_Gl_Glx_Service_quitRequested
  (
    Shizu_State2* state
  )
{ return g_quitRequested; }

void
Visuals_Gl_Glx_Service_getClientSize
  (
    Shizu_State2* state,
    Shizu_Integer32 *width,
    Shizu_Integer32 *height
  )
{
  XWindowAttributes windowAttributes;
  XGetWindowAttributes(g_display, g_window, &windowAttributes);
  *width = windowAttributes.width;
  *height = windowAttributes.height;
}

void*
Visuals_Gl_Glx_Service_link
  (
    Shizu_State2* state,
    char const* functionName,
    char const* extensionName
  )
{
  if (extensionName) {
    char const* extensionNames = NULL;
    extensionNames = glXQueryExtensionsString(g_display, DefaultScreen(g_display));
    if (!isExtensionSupported(extensionNames, extensionName)) {
      extensionNames = glGetString(GL_EXTENSIONS);
      if (!isExtensionSupported(extensionNames, extensionName)) {
        Shizu_State2_setStatus(state, 1);
        Shizu_State2_jump(state);
      }
    }
  }
  void* p = glXGetProcAddress(functionName);
  if (!p) {
    Shizu_State2_setStatus(state, 1);
    Shizu_State2_jump(state);
  }
  return p;
}

void
Visuals_Gl_Glx_Service_beginFrame
  (
    Shizu_State2* state
  )
{/*Intentionally empty.*/}

void
Visuals_Gl_Glx_Service_endFrame
  (
    Shizu_State2* state
  )
{
  glXSwapBuffers(g_display, g_window);
}
