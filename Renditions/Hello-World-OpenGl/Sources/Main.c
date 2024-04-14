// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#include "Zeitgeist.h"
#include "Zeitgeist/Value.h"
#include "Zeitgeist/Object.h"

#include "Zeitgeist/UpstreamRequests.h"

// strlen
#include <string.h>

// fprintf, stdout
#include <stdio.h>

#include "ServiceGl.h"

#if Zeitgeist_Configuration_OperatingSystem_Windows == Zeitgeist_Configuration_OperatingSystem
	#define Zeitgeist_Rendition_Export _declspec(dllexport)
#elif Zeitgeist_Configuration_OperatingSystem_Linux == Zeitgeist_Configuration_OperatingSystem
	#define Zeitgeist_Rendition_Export
#else
	#error("operating system not (yet) supported")
#endif

Zeitgeist_Rendition_Export Zeitgeist_String*
Zeitgeist_Rendition_getName
	(
		Zeitgeist_State* state
	)
{
	return Zeitgeist_State_createString(state, "Hello World (OpenGL)", strlen("Hello World (OpenGL)"));
}

// Vertex shader.
const GLchar* g_vertexShader =
	"#version 330\n"
	"layout(location = 0) in vec3 point;\n"
	"void main() {\n"
	"  mat4 scalexy = mat4\n"
	"	   (\n"
	"		   0.75, 0,    0, 0,\n"
	"			 0,    0.75, 0, 0,\n"
	"			 0,    0,    1, 0,\n"
	"      0,    0,    0, 1 \n"
	"    );\n"
	"  gl_Position = scalexy * vec4(point.xyz, 1.0);\n"
	"}\n"
	;

// Fragment shader.
// The color (255, 204, 51) is the websafe color "sunglow".
const GLchar* g_fragmentShader =
	"#version 330\n"
	"out vec4 color;\n"
	"void main() {\n"
	"  color = vec4(1, 0.8, 0.2, 0);\n"
	"}\n"
	;

static GLint Positions_Index = 0;

typedef struct VERTEX {
	float x, y, z;
} VERTEX;

static VERTEX const SQUARE[] = {
	{ -1.0f,  1.0f, 0.f, },
	{ -1.0f, -1.0f, 0.f, },
	{  1.0f,  1.0f, 0.f, },
	{  1.0f, -1.0f, 0.f, },
};

static GLuint g_programId = 0;
static GLuint g_bufferId = 0;
static GLuint g_vertexArrayId = 0;

Zeitgeist_Rendition_Export void
Zeitgeist_Rendition_update
	(
		Zeitgeist_State* state
	)
{
	ServiceGl_update(state);

	if (ServiceGl_quitRequested(state)) {
		Zeitgeist_UpstreamRequest* request = Zeitgeist_UpstreamRequest_createExitProcessRequest(state);
		Zeitgeist_sendUpstreamRequest(state, request);
	}

	Zeitgeist_Integer viewportWidth, viewportHeight;
	ServiceGl_getClientSize(state, &viewportWidth, &viewportHeight);
	ServiceGl_beginFrame(state);

	glViewport(0, 0, viewportWidth, viewportHeight);
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClearDepth(1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(g_programId);
	glBindVertexArray(g_vertexArrayId);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(SQUARE)/ sizeof(VERTEX));
	glBindVertexArray(0);
	glUseProgram(0);

	ServiceGl_endFrame(state);
}

Zeitgeist_Rendition_Export void
Zeitgeist_Rendition_load
	(
		Zeitgeist_State* state
	)
{
	ServiceGl_startup(state);
	ServiceGl_setTitle(state, Zeitgeist_State_createString(state, "Hello World (OpenGL)", strlen("Hello World (OpenGL)")));

	GLuint vertexShaderId, fragmentShaderId;

	vertexShaderId = ServiceGl_compileShader(state, GL_VERTEX_SHADER, g_vertexShader);
	fragmentShaderId = ServiceGl_compileShader(state, GL_FRAGMENT_SHADER, g_fragmentShader);
	g_programId = ServiceGl_linkProgram(state, vertexShaderId, fragmentShaderId);
	glDeleteShader(fragmentShaderId);
	glDeleteShader(vertexShaderId);

	glGenBuffers(1, &g_bufferId);
	glBindBuffer(GL_ARRAY_BUFFER, g_bufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SQUARE), SQUARE, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &g_vertexArrayId);
	glBindVertexArray(g_vertexArrayId);
	glBindBuffer(GL_ARRAY_BUFFER, g_bufferId);
	glVertexAttribPointer(Positions_Index, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(Positions_Index);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Zeitgeist_Rendition_Export void
Zeitgeist_Rendition_unload
	(
		Zeitgeist_State* state
	)
{
	glDeleteVertexArrays(1, &g_vertexArrayId);
	g_vertexArrayId = 0;
	glDeleteBuffers(1, &g_bufferId);
	g_bufferId = 0;
	glDeleteProgram(g_programId);
	g_programId = 0;
	ServiceGl_shutdown(state);
}
