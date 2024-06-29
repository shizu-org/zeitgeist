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

#include "Visuals/Gl/Wgl/Service.h"

// fprintf, stdout
#include <stdio.h>

// malloc, free
#include <malloc.h>

// Visuals_Service_emitKeyboardKeyMessage
#include "Visuals/Service.h"
#include "KeyboardKeyMessage.h"

#if Zeitgeist_Configuration_OperatingSystem_Windows == Zeitgeist_Configuration_OperatingSystem

	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
	#pragma comment (lib, "gdi32.lib")
	#pragma comment (lib, "user32.lib")
	#pragma comment (lib, "opengl32.lib")

	// Executables (but not DLLs) exporting this symbol with this value will be automatically directed to the high-performance GPU
	// on Nvidia Optimus systems with up-to-date drivers.
	__declspec(dllexport) DWORD NvOptimusEnablement = 1;

	// Executables (but not DLLs) exporting this symbol with this value will be automatically directed to the high-performance GPU
	// on AMD PowerXpress systems with up-to-date drivers
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;

#elif Zeitgeist_Configuration_OperatingSystem_Linux == Zeitgeist_Configuration_OperatingSystem
	#error("operating system not (yet) supported")
#else
	#error("operating system not (yet) supported")
#endif

#include <GL/gl.h>
#include <GL/glext.h>

#if Zeitgeist_Configuration_OperatingSystem_Windows == Zeitgeist_Configuration_OperatingSystem
	#include <GL/wglext.h>
#elif Zeitgeist_Configuration_OperatingSystem_Linux == Zeitgeist_Configuration_OperatingSystem
	#error("operating system not (yet) supported")
#else
	#error("operating system not (yet) supported")
#endif


static PFNWGLGETPIXELFORMATATTRIBFVARBPROC wglGetPixelFormatAttribfvARB = NULL;

static PFNWGLGETPIXELFORMATATTRIBIVARBPROC wglGetPixelFormatAttribivARB = NULL;

static PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB = NULL;

static PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = NULL;

static PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;


static HINSTANCE g_hInstance = NULL;

static char* g_className = NULL;

static ATOM g_classHandle = 0;

static HWND g_hWnd = NULL;

static HICON g_hSmallIcon = NULL;

static HICON g_hBigIcon = NULL;

static HDC g_hDc = NULL;

static HGLRC g_hGlrc = NULL;

static bool g_quitRequested = false;

// @brief Create an icon with the specified color and the specified width and height.
// @param RETURN A pointer to a HICON variable.
// @param width The width, in pixels, of the icon.
// @param height The height, in pixels, of the icon.
// @success <code>*RETURN</code> was assigned the icon handle.
static void
createIcon
	(
		Shizu_State2* state,
		HICON* RETURN,
		COLORREF color,
		int width,
		int height
	);

static void
createIcons
	(
		Shizu_State2* state,
		HWND hWnd,
		COLORREF color
	);

static void
destroyIcons
	(
		Shizu_State2* state
	);

// Helper to check for extension string presence.	Adapted from:
// http://www.opengl.org/resources/features/OGLextensions/
static bool
isExtensionSupported
	(
		const char* extensions,
		const char* extension
	);

static LRESULT CALLBACK
windowCallbackLegacy
	(
		HWND wnd,
		UINT msg,
		WPARAM wparam,
		LPARAM lparam
	);

static Shizu_Value
mapKeyboardKey
	(
		WPARAM wParam
	);

static LRESULT CALLBACK
windowCallback
	(
		HWND wnd,
		UINT msg,
		WPARAM wparam,
		LPARAM lparam
	);

static void
choosePixelFormatLegacy
	(
		Shizu_State2* state
	);

void
choosePixelFormat
	(
		Shizu_State2* state
	);

static void
shutdownLegacy
	(
		Shizu_State2* state
	);

static void
startupLegacy
	(
		Shizu_State2* state
	);

static void
shutdown
	(
		Shizu_State2* state
	);

static void
startup
	(
		Shizu_State2* state
	);

static void
createIcon
	(
		Shizu_State2* state,
		HICON* RETURN,
		COLORREF color,
		int width,
		int height
	)
{
	// Obtain a handle to the screen device context.
	HDC hdcScreen = GetDC(NULL);
	if (!hdcScreen) {
		Shizu_State2_setStatus(state, 1);
		Shizu_State2_jump(state);
	}

	// Create a memory device context, which we will draw into.
	HDC hdcMem = CreateCompatibleDC(hdcScreen);
	if (!hdcMem) {
		ReleaseDC(NULL, hdcScreen);
		hdcScreen = NULL;
		Shizu_State2_setStatus(state, 1);
		Shizu_State2_jump(state);
	}

	// Create the bitmap, and select it into the device context for drawing.
	HBITMAP hbmp = CreateCompatibleBitmap(hdcScreen, width, height);
	if (!hbmp) {
		DeleteDC(hdcMem);
		hdcMem = NULL;
		ReleaseDC(NULL, hdcScreen);
		hdcScreen = NULL;
		Shizu_State2_setStatus(state, 1);
		Shizu_State2_jump(state);
	}
	HBITMAP hbmpOld = (HBITMAP)SelectObject(hdcMem, hbmp);

	// Draw your icon.
	// 
	// For this simple example, we're just drawing a solid color rectangle
	// in the specified color with the specified dimensions.
	HPEN hpen = CreatePen(PS_SOLID, 1, color);
	HPEN hpenOld = (HPEN)SelectObject(hdcMem, hpen);
	HBRUSH hbrush = CreateSolidBrush(color);
	HBRUSH hbrushOld = (HBRUSH)SelectObject(hdcMem, hbrush);
	Rectangle(hdcMem, 0, 0, width, height);
	SelectObject(hdcMem, hbrushOld);
	SelectObject(hdcMem, hpenOld);
	DeleteObject(hbrush);
	DeleteObject(hpen);

	// Create an icon from the bitmap.
	// 
	// Icons require masks to indicate transparent and opaque areas. Since this
	// simple example has no transparent areas, we use a fully opaque mask.
	HBITMAP hbmpMask = CreateCompatibleBitmap(hdcScreen, width, height);
	ICONINFO ii;
	ii.fIcon = TRUE;
	ii.hbmMask = hbmpMask;
	ii.hbmColor = hbmp;
	HICON hIcon = CreateIconIndirect(&ii);
	DeleteObject(hbmpMask);

	// Clean-up.
	SelectObject(hdcMem, hbmpOld);
	DeleteObject(hbmp);
	DeleteDC(hdcMem);
	ReleaseDC(NULL, hdcScreen);

	*RETURN = hIcon;
}

static void
createIcons
	(
		Shizu_State2* state,
		HWND hWnd,
		COLORREF color
	)
{
	int size;

	// Create big icon.
	size = GetSystemMetrics(SM_CXICON);
	createIcon(state, &g_hBigIcon, color, size, size);

	// Create small icon.
	size = GetSystemMetrics(SM_CXSMICON);
	Shizu_JumpTarget jumpTarget;
	Shizu_State2_pushJumpTarget(state, &jumpTarget);
	if (!setjmp(jumpTarget.environment)) {
		createIcon(state, &g_hSmallIcon, color, size, size);
		Shizu_State2_popJumpTarget(state);
	} else {
		Shizu_State2_popJumpTarget(state);
		DestroyIcon(g_hBigIcon);
		g_hBigIcon = NULL;
		Shizu_State2_setStatus(state, 1);
		Shizu_State2_jump(state);
	}
}

static void
destroyIcons
	(
		Shizu_State2* state
	)
{
	if (g_hSmallIcon) {
		DestroyIcon(g_hSmallIcon);
		g_hSmallIcon = NULL;
	}
	if (g_hBigIcon) {
		DestroyIcon(g_hBigIcon);
		g_hBigIcon = NULL;
	}
}

// Helper to check for extension string presence.	Adapted from:
// http://www.opengl.org/resources/features/OGLextensions/
static bool
isExtensionSupported
	(
		const char* extensions,
		const char* extension
	)
{
	const char* start;
	const char* where, * terminator;

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

		if (where == start || *(where - 1) == ' ')
			if (*terminator == ' ' || *terminator == '\0')
				return true;

		start = terminator;
	}

	return false;
}

static LRESULT CALLBACK
windowCallbackLegacy
	(
		HWND wnd,
		UINT msg,
		WPARAM wparam,
		LPARAM lparam
	)
{
	return DefWindowProc(wnd, msg, wparam, lparam);
}

static Shizu_Value
mapKeyboardKey
	(
		WPARAM wParam
	)
{
	// https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
	switch (wParam) {
		case VK_UP: {
			Shizu_Value value;
			Shizu_Value_setInteger32(&value, KeyboardKey_Up);
			return value;
		} break;
		case VK_DOWN: {
			Shizu_Value value;
			Shizu_Value_setInteger32(&value, KeyboardKey_Down);
			return value;
		} break;
		case VK_LEFT: {
			Shizu_Value value;
			Shizu_Value_setInteger32(&value, KeyboardKey_Left);
			return value;
		} break;
		case VK_RIGHT: {
			Shizu_Value value;
			Shizu_Value_setInteger32(&value, KeyboardKey_Right);
			return value;
		} break;
		case VK_ESCAPE: {
			Shizu_Value value;
			Shizu_Value_setInteger32(&value, KeyboardKey_Escape);
			return value;
		} break;
		case 0x41: /*A*/ {
			Shizu_Value value;
			Shizu_Value_setInteger32(&value, KeyboardKey_A);
			return value;
		} break;
		case 0x44: /*D*/ {
			Shizu_Value value;
			Shizu_Value_setInteger32(&value, KeyboardKey_D);
			return value;
		} break;
		case 0x45: /*E*/ {
			Shizu_Value value;
			Shizu_Value_setInteger32(&value, KeyboardKey_E);
			return value;
		} break;
		case 0x4C: /*L*/ {
			Shizu_Value value;
			Shizu_Value_setInteger32(&value, KeyboardKey_L);
			return value;
		}	break;
		case 0x51: /*Q*/ {
			Shizu_Value value;
			Shizu_Value_setInteger32(&value, KeyboardKey_Q);
			return value;
		} break;
		case 0x53: /*S*/ {
			Shizu_Value value;
			Shizu_Value_setInteger32(&value, KeyboardKey_S);
			return value;
		} break;
		case 0x57: /*W*/ {
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

static LRESULT CALLBACK
windowCallback
	(
		HWND hWnd,
		UINT msg,
		WPARAM wParam,
		LPARAM lParam
	)
{
	switch (msg) {
		case WM_CREATE: {
			CREATESTRUCT* pCreateStruct = (CREATESTRUCT*)lParam;
			if (!pCreateStruct) {
				return -1;
			}
			Shizu_State2* state = (Shizu_State2*)pCreateStruct->lpCreateParams;
			if (!SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)state)) {
				if (GetLastError()) {
					return -1;
				}
			}
			return 0;
		} break;
		case WM_CLOSE: {
			DestroyWindow(hWnd);
			return 0;
		} break;
		case WM_KEYDOWN: {
			//fprintf(stdout, "[service wgl] WM_KEYDOWN\n");
			Shizu_State2* state = (Shizu_State2*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
			if (state) {
				Shizu_JumpTarget jumpTarget;
				Shizu_State2_pushJumpTarget(state, &jumpTarget);
				if (!setjmp(jumpTarget.environment)) {
					Shizu_Value mappedKey = mapKeyboardKey(wParam);
					if (Shizu_Value_isInteger32(&mappedKey)) {
						KeyboardKeyMessage* message = KeyboardKeyMessage_create(state, KeyboardKey_Action_Pressed, Shizu_Value_getInteger32(&mappedKey));
						Visuals_Service_emitKeyboardKeyMessage(state, message);
					}
					Shizu_State2_popJumpTarget(state);
				} else {
					Shizu_State2_popJumpTarget(state);
				}
			}
			return 0;
		} break;
		case WM_KEYUP: {
			//fprintf(stdout, "[service wgl] WM_KEYUP\n");
			Shizu_State2* state = (Shizu_State2*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
			if (state) {
				Shizu_JumpTarget jumpTarget;
				Shizu_State2_pushJumpTarget(state, &jumpTarget);
				if (!setjmp(jumpTarget.environment)) {
					Shizu_Value mappedKey = mapKeyboardKey(wParam);
					if (Shizu_Value_isInteger32(&mappedKey)) {
						KeyboardKeyMessage* message = KeyboardKeyMessage_create(state, KeyboardKey_Action_Released, Shizu_Value_getInteger32(&mappedKey));
						Visuals_Service_emitKeyboardKeyMessage(state, message);
					}
					Shizu_State2_popJumpTarget(state);
				} else {
					Shizu_State2_popJumpTarget(state);
				}
			}
			return 0;
		} break;
		case WM_DESTROY: {
			PostQuitMessage(0);
			return 0;
		} break;
	};
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

static void
choosePixelFormatLegacy
	(
		Shizu_State2* state
	)
{
	PIXELFORMATDESCRIPTOR descriptor;
	memset(&descriptor, 0, sizeof(descriptor));
	descriptor.nSize = sizeof(descriptor);
	descriptor.nVersion = 1;
	descriptor.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	descriptor.iPixelType = PFD_TYPE_RGBA;
	descriptor.cColorBits = 24;
	int pixelFormat = ChoosePixelFormat(g_hDc, &descriptor);
	if (!pixelFormat) {
		//
		Shizu_State2_setStatus(state, 1);
		Shizu_State2_jump(state);
	}
	if (!DescribePixelFormat(g_hDc, pixelFormat, sizeof(descriptor), &descriptor)) {
		//
		Shizu_State2_setStatus(state, 1);
		Shizu_State2_jump(state);
	}
	if (!SetPixelFormat(g_hDc, pixelFormat, &descriptor)) {
		//
		Shizu_State2_setStatus(state, 1);
		Shizu_State2_jump(state);
	}
}

static void
choosePixelFormat
	(
		Shizu_State2* state
	)
{ 
	int samples = 0;
	int const pixelFormatAttribs[] = {
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
		WGL_COLOR_BITS_ARB, 24,
		WGL_ALPHA_BITS_ARB, 8,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_STENCIL_BITS_ARB, 8,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_SAMPLE_BUFFERS_ARB, samples > 0 ? 1 : 0,
		WGL_SAMPLES_ARB, samples,
		WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB, GL_TRUE,
		0, 0
	};
	int i, n;
	if (!wglChoosePixelFormatARB(g_hDc, pixelFormatAttribs, 0, 1, &i, &n)) {
		Shizu_State2_setStatus(state, 1);
		Shizu_State2_jump(state);
	}
	PIXELFORMATDESCRIPTOR desc;
	if (!DescribePixelFormat(g_hDc, i, sizeof(desc), &desc)) {
		Shizu_State2_setStatus(state, 1);
		Shizu_State2_jump(state);
	}
	if (!SetPixelFormat(g_hDc, i, &desc)) {
		Shizu_State2_setStatus(state, 1);
		Shizu_State2_jump(state);
	}
}

static void
shutdownLegacy
	(
		Shizu_State2* state
	)
{
	wglCreateContextAttribsARB = NULL;
	wglGetPixelFormatAttribfvARB = NULL;
	wglChoosePixelFormatARB = NULL;
	wglGetPixelFormatAttribfvARB = NULL;
	wglGetPixelFormatAttribivARB = NULL;
}

static void
startupLegacy
	(
		Shizu_State2* state
	)
{
	Shizu_JumpTarget jumpTarget;

	g_hInstance = GetModuleHandle(NULL);
	if (!g_hInstance) {
		Shizu_State2_setStatus(state, 1);
		Shizu_State2_jump(state);
	}
	g_className = _strdup("windowClassLegacy");
	if (!g_className) {
		g_hInstance = NULL;
		//
		Shizu_State2_setStatus(state, 1);
		Shizu_State2_jump(state);
	}
	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = &windowCallbackLegacy;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = g_hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hIconSm = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = g_className;
	g_classHandle = RegisterClassEx(&wcex);
	if (!g_classHandle) {
		//
		free(g_className);
		g_className = NULL;
		//
		g_hInstance = NULL;
		//
		Shizu_State2_setStatus(state, 1);
		Shizu_State2_jump(state);
	}
	g_hWnd = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
		 										  g_className,
													"Zeitgeist",
													WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
													CW_USEDEFAULT,
													CW_USEDEFAULT,
													CW_USEDEFAULT,
													CW_USEDEFAULT,
													NULL,
													NULL,
													g_hInstance,
													state);
	if (!g_hWnd) {
		//
		UnregisterClass(g_className, g_hInstance);
		g_classHandle = 0;
		//
		free(g_className);
		g_className = NULL;
		//
		g_hInstance = NULL;
		//
		Shizu_State2_setStatus(state, 1);
		Shizu_State2_jump(state);
	}
	//
	COLORREF color = RGB(0, 0, 0);
	Shizu_State2_pushJumpTarget(state, &jumpTarget);
	if (!setjmp(jumpTarget.environment)) {
		createIcons(state, g_hWnd, color);
		Shizu_State2_popJumpTarget(state);
	} else {
		Shizu_State2_popJumpTarget(state);
		//
		DestroyWindow(g_hWnd);
		g_hWnd = NULL;
		//
		UnregisterClass(g_className, g_hInstance);
		g_classHandle = 0;
		//
		free(g_className);
		g_className = NULL;
		//
		g_hInstance = NULL;
		//
		Shizu_State2_setStatus(state, 1);
		Shizu_State2_jump(state);
	}
	SendMessage(g_hWnd, WM_SETICON, ICON_BIG, (LPARAM)g_hBigIcon);
	SendMessage(g_hWnd, WM_SETICON, ICON_SMALL, (LPARAM)g_hSmallIcon);
	//
	g_hDc = GetDC(g_hWnd);
	if (!g_hDc) {
		//
		destroyIcons(state);
		//
		DestroyWindow(g_hWnd);
		g_hWnd = NULL;
		//
		UnregisterClass(g_className, g_hInstance);
		g_classHandle = 0;
		//
		free(g_className);
		g_className = NULL;
		//
		g_hInstance = NULL;
		//
		Shizu_State2_setStatus(state, 1);
		Shizu_State2_jump(state);
	}
	Shizu_State2_pushJumpTarget(state, &jumpTarget);
	if (!setjmp(jumpTarget.environment)) {
		choosePixelFormatLegacy(state);
		Shizu_State2_popJumpTarget(state);
	} else {
		Shizu_State2_popJumpTarget(state);
		//
		ReleaseDC(g_hWnd, g_hDc);
		g_hDc = NULL;
		//
		destroyIcons(state);
		//
		DestroyWindow(g_hWnd);
		g_hWnd = NULL;
		//
		UnregisterClass(g_className, g_hInstance);
		g_classHandle = 0;
		//
		free(g_className);
		g_className = NULL;
		//
		g_hInstance = NULL;
		//
		Shizu_State2_setStatus(state, 1);
		Shizu_State2_jump(state);
	}
	g_hGlrc = wglCreateContext(g_hDc);
	if (!g_hGlrc) {
		//
		ReleaseDC(g_hWnd, g_hDc);
		g_hDc = NULL;
		//
		destroyIcons(state);
		//
		DestroyWindow(g_hWnd);
		g_hWnd = NULL;
		//
		UnregisterClass(g_className, g_hInstance);
		g_classHandle = 0;
		//
		free(g_className);
		g_className = NULL;
		//
		g_hInstance = NULL;
		//
		Shizu_State2_setStatus(state, 1);
		Shizu_State2_jump(state);
	}
	if (!wglMakeCurrent(g_hDc, g_hGlrc)) {
		//
		wglDeleteContext(g_hGlrc);
		g_hGlrc = NULL;
		//
		ReleaseDC(g_hWnd, g_hDc);
		g_hDc = NULL;
		//
		destroyIcons(state);
		//
		DestroyWindow(g_hWnd);
		g_hWnd = NULL;
		//
		UnregisterClass(g_className, g_hInstance);
		g_classHandle = 0;
		//
		free(g_className);
		g_className = NULL;
		//
		g_hInstance = NULL;
		//
		Shizu_State2_setStatus(state, 1);
		Shizu_State2_jump(state);
	}
	//
	wglGetPixelFormatAttribivARB = (PFNWGLGETPIXELFORMATATTRIBIVARBPROC)wglGetProcAddress("wglGetPixelFormatAttribivARB");
	if (!wglGetPixelFormatAttribivARB) {
		//
		wglDeleteContext(g_hGlrc);
		g_hGlrc = NULL;
		//
		ReleaseDC(g_hWnd, g_hDc);
		g_hDc = NULL;
		//
		destroyIcons(state);
		//
		DestroyWindow(g_hWnd);
		g_hWnd = NULL;
		//
		UnregisterClass(g_className, g_hInstance);
		g_classHandle = 0;
		//
		free(g_className);
		g_className = NULL;
		//
		g_hInstance = NULL;
		//
		Shizu_State2_setStatus(state, 1);
		Shizu_State2_jump(state);
	}
	wglGetPixelFormatAttribfvARB = (PFNWGLGETPIXELFORMATATTRIBFVARBPROC)wglGetProcAddress("wglGetPixelFormatAttribfvARB");
	if (!wglGetPixelFormatAttribfvARB) {
		wglGetPixelFormatAttribivARB = NULL;
		//
		wglDeleteContext(g_hGlrc);
		g_hGlrc = NULL;
		//
		ReleaseDC(g_hWnd, g_hDc);
		g_hDc = NULL;
		//
		destroyIcons(state);
		//
		DestroyWindow(g_hWnd);
		g_hWnd = NULL;
		//
		UnregisterClass(g_className, g_hInstance);
		g_classHandle = 0;
		//
		free(g_className);
		g_className = NULL;
		//
		g_hInstance = NULL;
		//
		Shizu_State2_setStatus(state, 1);
		Shizu_State2_jump(state);
	}
	wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
	if (!wglGetPixelFormatAttribfvARB) {
		wglGetPixelFormatAttribfvARB = NULL;
		wglGetPixelFormatAttribivARB = NULL;
		//
		wglDeleteContext(g_hGlrc);
		g_hGlrc = NULL;
		//
		ReleaseDC(g_hWnd, g_hDc);
		g_hDc = NULL;
		//
		destroyIcons(state);
		//
		DestroyWindow(g_hWnd);
		g_hWnd = NULL;
		//
		UnregisterClass(g_className, g_hInstance);
		g_classHandle = 0;
		//
		free(g_className);
		g_className = NULL;
		//
		g_hInstance = NULL;
		//
		Shizu_State2_setStatus(state, 1);
		Shizu_State2_jump(state);
	}
	wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsStringARB");
	if (!wglGetPixelFormatAttribfvARB) {
		wglChoosePixelFormatARB = NULL;
		wglGetPixelFormatAttribfvARB = NULL;
		wglGetPixelFormatAttribivARB = NULL;
		//
		wglDeleteContext(g_hGlrc);
		g_hGlrc = NULL;
		//
		ReleaseDC(g_hWnd, g_hDc);
		g_hDc = NULL;
		//
		destroyIcons(state);
		//
		DestroyWindow(g_hWnd);
		g_hWnd = NULL;
		//
		UnregisterClass(g_className, g_hInstance);
		g_classHandle = 0;
		//
		free(g_className);
		g_className = NULL;
		//
		g_hInstance = NULL;
		//
		Shizu_State2_setStatus(state, 1);
		Shizu_State2_jump(state);
	}
	wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
	if (!wglCreateContextAttribsARB) {
		wglChoosePixelFormatARB = NULL;
		wglGetPixelFormatAttribfvARB = NULL;
		wglGetPixelFormatAttribivARB = NULL;
		//
		wglDeleteContext(g_hGlrc);
		g_hGlrc = NULL;
		//
		ReleaseDC(g_hWnd, g_hDc);
		g_hDc = NULL;
		//
		destroyIcons(state);
		//
		DestroyWindow(g_hWnd);
		g_hWnd = NULL;
		//
		UnregisterClass(g_className, g_hInstance);
		g_classHandle = 0;
		//
		free(g_className);
		g_className = NULL;
		//
		g_hInstance = NULL;
		//
		Shizu_State2_setStatus(state, 1);
		Shizu_State2_jump(state);
	}
	//
	wglDeleteContext(g_hGlrc);
	g_hGlrc = NULL;
	//
	ReleaseDC(g_hWnd, g_hDc);
	g_hDc = NULL;
	//
	DestroyWindow(g_hWnd);
	g_hWnd = NULL;
	//
	UnregisterClass(g_className, g_hInstance);
	g_classHandle = 0;
	//
	free(g_className);
	g_className = NULL;
	//
	g_hInstance = NULL;
}

static void
shutdown
	(
		Shizu_State2* state
	)
{ 
	if (g_hGlrc) {
		wglDeleteContext(g_hGlrc);
		g_hGlrc = NULL;
	}
	//
	if (g_hDc) {
		ReleaseDC(g_hWnd, g_hDc);
		g_hDc = NULL;
	}
	//
	destroyIcons(state);
	//
	if (g_hWnd) {
		DestroyWindow(g_hWnd);
		g_hWnd = NULL;
	}
	//
	if (g_classHandle) {
		UnregisterClass(g_className, g_hInstance);
		g_classHandle = 0;
	}
	//
	if (g_className) {
		free(g_className);
		g_className = NULL;
	}
	//
	if (g_hInstance) {
		g_hInstance = NULL;
	}
}

static void
startup
	(
		Shizu_State2* state
	)
{
	Shizu_JumpTarget jumpTarget;

	g_hInstance = GetModuleHandle(NULL);
	if (!g_hInstance) {
		shutdownLegacy(state);
		//
		Shizu_State2_setStatus(state, 1);
		Shizu_State2_jump(state);
	}
	g_className = _strdup("windowClass");
	if (!g_className) {
		g_hInstance = NULL;
		//
		shutdownLegacy(state);
		//
		Shizu_State2_setStatus(state, 1);
		Shizu_State2_jump(state);
	}
	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = &windowCallback;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = g_hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hIconSm = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = g_className;
	g_classHandle = RegisterClassEx(&wcex);
	if (!g_classHandle) {
		//
		free(g_className);
		g_className = NULL;
		//
		g_hInstance = NULL;
		//
		shutdownLegacy(state);
		//
		Shizu_State2_setStatus(state, 1);
		Shizu_State2_jump(state);
	}
	g_hWnd = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
													g_className,
													"Zeitgeist",
													WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
													CW_USEDEFAULT,
													CW_USEDEFAULT,
													CW_USEDEFAULT,
													CW_USEDEFAULT,
													NULL,
													NULL,
													g_hInstance,
													state);
	if (!g_hWnd) {
		//
		UnregisterClass(g_className, g_hInstance);
		g_classHandle = 0;
		//
		free(g_className);
		g_className = NULL;
		//
		g_hInstance = NULL;
		//
		shutdownLegacy(state);
		//
		Shizu_State2_setStatus(state, 1);
		Shizu_State2_jump(state);
	}
	//
	COLORREF color = RGB(0, 0, 0);
	Shizu_State2_pushJumpTarget(state, &jumpTarget);
	if (!setjmp(jumpTarget.environment)) {
		createIcons(state, g_hWnd, color);
		Shizu_State2_popJumpTarget(state);
	} else {
		Shizu_State2_popJumpTarget(state);
		//
		DestroyWindow(g_hWnd);
		g_hWnd = NULL;
		//
		UnregisterClass(g_className, g_hInstance);
		g_classHandle = 0;
		//
		free(g_className);
		g_className = NULL;
		//
		g_hInstance = NULL;
		//
		Shizu_State2_setStatus(state, 1);
		Shizu_State2_jump(state);
	}
	SendMessage(g_hWnd, WM_SETICON, ICON_BIG, (LPARAM)g_hBigIcon);
	SendMessage(g_hWnd, WM_SETICON, ICON_SMALL, (LPARAM)g_hSmallIcon);
	//
	g_hDc = GetDC(g_hWnd);
	if (!g_hDc) {
		//
		destroyIcons(state);
		//
		DestroyWindow(g_hWnd);
		g_hWnd = NULL;
		//
		UnregisterClass(g_className, g_hInstance);
		g_classHandle = 0;
		//
		free(g_className);
		g_className = NULL;
		//
		g_hInstance = NULL;
		//
		shutdownLegacy(state);
		//
		Shizu_State2_setStatus(state, 1);
		Shizu_State2_jump(state);
	}
	//
	Shizu_State2_pushJumpTarget(state, &jumpTarget);
	if (!setjmp(jumpTarget.environment)) {
		choosePixelFormat(state);
		Shizu_State2_popJumpTarget(state);
	} else {
		Shizu_State2_popJumpTarget(state);
		//
		ReleaseDC(g_hWnd, g_hDc);
		g_hDc = NULL;
		//
		destroyIcons(state);
		//
		DestroyWindow(g_hWnd);
		g_hWnd = NULL;
		//
		UnregisterClass(g_className, g_hInstance);
		g_classHandle = 0;
		//
		free(g_className);
		g_className = NULL;
		//
		g_hInstance = NULL;
		//
		shutdownLegacy(state);
		//
		Shizu_State2_setStatus(state, 1);
		Shizu_State2_jump(state);
	}
	//
	ShowWindow(g_hWnd, SW_SHOW);
	//
	const int contextAttribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 1,
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
	};
	g_hGlrc = wglCreateContextAttribsARB(g_hDc, NULL, contextAttribs);
	if (!g_hGlrc) {
		//
		ReleaseDC(g_hWnd, g_hDc);
		g_hDc = NULL;
		//
		destroyIcons(state);
		//
		DestroyWindow(g_hWnd);
		g_hWnd = NULL;
		//
		UnregisterClass(g_className, g_hInstance);
		g_classHandle = 0;
		//
		free(g_className);
		g_className = NULL;
		//
		g_hInstance = NULL;
		//
		shutdownLegacy(state);
		//
		Shizu_State2_setStatus(state, 1);
		Shizu_State2_jump(state);
	}
	if (!wglMakeCurrent(g_hDc, g_hGlrc)) {
		//
		wglDeleteContext(g_hGlrc);
		g_hGlrc = NULL;
		//
		ReleaseDC(g_hWnd, g_hDc);
		g_hDc = NULL;
		//
		destroyIcons(state);
		//
		DestroyWindow(g_hWnd);
		g_hWnd = NULL;
		//
		UnregisterClass(g_className, g_hInstance);
		g_classHandle = 0;
		//
		free(g_className);
		g_className = NULL;
		//
		g_hInstance = NULL;
		//
		shutdownLegacy(state);
		//
		Shizu_State2_setStatus(state, 1);
		Shizu_State2_jump(state);
	}
	// Default value. See Visuals_Context.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Default value. See Visuals_Context.
	glDisable(GL_CULL_FACE);
	// Default value. See Visuals_Context.
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);
	// Default value. See Visuals_Context.
	glClearDepth(1.f);
	// Default value. See Visuals_Context.
	glClearColor(0.f, 0.f, 0.f, 0.f);
}

void
Visuals_Gl_Wgl_Service_startup
	(
		Shizu_State2* state
	)
{
	fprintf(stdout, "[Hello World (OpenGL)] starting up WGL service\n");
	startupLegacy(state);
	Shizu_JumpTarget jumpTarget;
	Shizu_State2_pushJumpTarget(state, &jumpTarget);
	if (!setjmp(jumpTarget.environment)) {
		startup(state);
		Shizu_State2_popJumpTarget(state);
	} else {
		Shizu_State2_popJumpTarget(state);
		shutdownLegacy(state);
		Shizu_State2_jump(state);
	}
	g_quitRequested = false;
}

void
Visuals_Gl_Wgl_Service_shutdown
	(
		Shizu_State2* state
 	)
{
	fprintf(stdout, "[Hello World (OpenGL)] shutting down WGL service\n");
	shutdown(state);
	shutdownLegacy(state);
}

void
Visuals_Gl_Wgl_Service_setTitle
	(
		Shizu_State2* state,
		Shizu_String* title
	)
{
	Shizu_String* zeroTerminator = Shizu_String_create(state, "", 1);
	title = Shizu_String_concatenate(state, title, zeroTerminator);
	SetWindowText(g_hWnd, Shizu_String_getBytes(state, title));
}

void
Visuals_Gl_Wgl_Service_update
	(
		Shizu_State2* state
	)
{
	MSG message;
	while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
		if (message.message == WM_QUIT) {
			g_quitRequested = true;
		} else {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}
}

Shizu_Boolean
Visuals_Gl_Wgl_Service_quitRequested
	(
		Shizu_State2* state
	)
{ return g_quitRequested; }

void
Visuals_Gl_Wgl_Service_getClientSize
	(
		Shizu_State2* state,
		Shizu_Integer32 *width,
		Shizu_Integer32 *height
	)
{
	RECT rectangle;
	GetClientRect(g_hWnd, &rectangle);
	*width = rectangle.right  - rectangle.left;
	*height = rectangle.bottom - rectangle.top;
}

void*
Visuals_Gl_Wgl_Service_link
	(
		Shizu_State2* state,
		char const* functionName,
		char const* extensionName
	)
{
	if (extensionName) {
		char const* extensionNames = wglGetExtensionsStringARB(g_hDc);
		if (!isExtensionSupported(extensionNames, extensionName)) {
			Shizu_State2_setStatus(state, 1);
			Shizu_State2_jump(state);
		}
	}
	void* p = wglGetProcAddress(functionName);
	if (!p) {
		Shizu_State2_setStatus(state, 1);
		Shizu_State2_jump(state);
	}
	return p;
}

void
Visuals_Gl_Wgl_Service_beginFrame
	(
		Shizu_State2* state
	)
{/*Intentionally empty.*/}

void
Visuals_Gl_Wgl_Service_endFrame
	(
		Shizu_State2* state
	)
{
	SwapBuffers(g_hDc);
}
