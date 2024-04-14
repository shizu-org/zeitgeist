// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#include "Zeitgeist/UpstreamRequests.h"

// strlen
#include <string.h>

// fprintf, stdout
#include <stdio.h>

#include "ServiceGl.h"

#if Shizu_Configuration_OperatingSystem_Windows == Shizu_Configuration_OperatingSystem
  #define Shizu_Rendition_Export _declspec(dllexport)
#elif Shizu_Configuration_OperatingSystem_Linux == Shizu_Configuration_OperatingSystem
  #define Shizu_Rendition_Export
#else
  #error("operating system not (yet) supported")
#endif

Shizu_Rendition_Export char const*
Shizu_getDlName
  (
    Shizu_State* state
  )
{
  static char const* NAME = "<Dl:Hello World (OpenGL)>";
  return NAME;
}

Shizu_Rendition_Export Shizu_String*
Zeitgeist_Rendition_getName
  (
    Shizu_State* state
  )
{
  return Shizu_String_create(state, "Hello World (OpenGL)", strlen("Hello World (OpenGL)"));
}

// Vertex shader.
static const GLchar* g_vertexShader =
  "#version 330 core\n"
  "layout(location = 0) in vec3 vertexPosition;\n"
  "uniform mat4 scale = mat4(1);\n"
  "void main() {\n"
  "  gl_Position = scale * vec4(vertexPosition.xyz, 1.0);\n"
  "}\n"
  ;

// Fragment shader.
// The color (255, 204, 51) is the websafe color "sunglow".
static const GLchar* g_fragmentShader =
  "#version 330 core\n"
  "out vec4 outputColor;\n"
  //"uniform vec4 inputColor = vec4(1.0, 1.0, 1.0, 0.0);\n"
  "uniform vec4 inputColor = vec4(1.0, 0.8, 0.2, 0.0);\n"
  "void main() {\n"
  "  outputColor = inputColor;\n"
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

#include "Visuals/Program.h"

static Zeitgeist_Visuals_GlProgram* g_program = NULL;
static GLuint g_bufferId = 0;
static GLuint g_vertexArrayId = 0;

Shizu_Rendition_Export void
Zeitgeist_Rendition_update
  (
    Shizu_State* state
  )
{
  ServiceGl_update(state);

  if (ServiceGl_quitRequested(state)) {
    Zeitgeist_UpstreamRequest* request = Zeitgeist_UpstreamRequest_createExitProcessRequest(state);
    Zeitgeist_sendUpstreamRequest(state, request);
  }

  Shizu_Integer32 viewportWidth, viewportHeight;
  ServiceGl_getClientSize(state, &viewportWidth, &viewportHeight);
  ServiceGl_beginFrame(state);

  glViewport(0, 0, viewportWidth, viewportHeight);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glUseProgram(g_program->programId);

  GLint location;
  
  location = glGetUniformLocation(g_program->programId, "scale");
  if (-1 == location) {
    fprintf(stderr, "%s:%d: unable to get uniform location of `%s`\n", __FILE__, __LINE__, "scale");
  } else {
    GLfloat matrix[16] = {
      .75f, 0.f,  0.f, 0.f,
      0.f,  .75f, 0.f, 0.f,
      0.f,  0.f,  1.f, 0.f,
      0.f,  0.f,  0.f, 1.f,
    };
    glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0]);
  }
  
  location = glGetUniformLocation(g_program->programId, "inputColor");
  if (-1 == location) {
    fprintf(stderr, "%s:%d: unable to get uniform location of `%s`\n", __FILE__, __LINE__, "inputColor");
  } else {
    // The color (255, 204, 51) is the websafe color "sunglow".
    GLfloat vector[4] = { 1.0, 0.8, 0.2, 1.0 };
    glUniform4fv(location, 1, &vector[0]);
  }

  glBindVertexArray(g_vertexArrayId);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(SQUARE)/ sizeof(VERTEX));
  glBindVertexArray(0);
  glUseProgram(0);

  ServiceGl_endFrame(state);
}

static void
loadPrograms
  (
    Shizu_State* state
  )
{
  g_program = Zeitgeist_Visuals_GlProgram_create(state, Shizu_String_create(state, g_vertexShader, strlen(g_vertexShader)),
                                                        Shizu_String_create(state, g_fragmentShader, strlen(g_fragmentShader)));
  Shizu_Object_lock(state, (Shizu_Object*)g_program);
  Zeitgeist_Visuals_GlProgram_materialize(state, g_program);
}

static void
unloadPrograms
  (
    Shizu_State* state
  )
{
  Zeitgeist_Visuals_GlProgram_unmaterialize(state, g_program);
  Shizu_Object_unlock(state, (Shizu_Object*)g_program);
  g_program = NULL;
}

Shizu_Rendition_Export void
Zeitgeist_Rendition_load
  (
    Shizu_State* state
  )
{
  ServiceGl_startup(state);
  ServiceGl_setTitle(state, Shizu_String_create(state, "Hello World (OpenGL)", strlen("Hello World (OpenGL)")));

  loadPrograms(state);

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
  
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glClearColor(0.0f, 0.0f, 0.0f, 0.f);
  glClearDepth(1.f);
}

Shizu_Rendition_Export void
Zeitgeist_Rendition_unload
  (
    Shizu_State* state
  )
{
  glDeleteVertexArrays(1, &g_vertexArrayId);
  g_vertexArrayId = 0;
  glDeleteBuffers(1, &g_bufferId);
  g_bufferId = 0;
  unloadPrograms(state);
  ServiceGl_shutdown(state);
}
