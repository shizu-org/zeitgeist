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

#include "Visuals/DefaultPrograms.h"

#define VertexPositionName "vertexPosition"
#define VertexNormalName "vertexNormal"
#define VertexColorName "vertexColor"

// Vertex shader.
// constant inputs:
//  meshAmbientColor vec4: The ambient color of the mesh in RGBF32.
//  inputFragmentAmbientColorType : Determine if the mesh ambient color or the vertex ambient color
//                                  is used as the input fragment color. This can assume two values:
//                                  0 (default and fallback, the mesh ambient color is used)
//                                  1 (the vertex ambient color is used)
// variable inputs:
//  vertexPosition vec3: The vertex position in model coordinates.
//  vertexAmbientColor vec4: The vertex ambient color in RGBF32.
static const GLchar* Programs_Pbr1_vertexProgram =
  "#version 330 core\n"
  // vertex variables
  "layout(location = 0) in vec3 " VertexPositionName ";\n"
  "layout(location = 1) in vec3 " VertexNormalName ";\n"
  "layout(location = 2) in vec3 " VertexColorName ";\n"
  // decide if mesh color or vertex color is used.
  "uniform int inputFragmentColorType = 0;\n"
  "uniform vec3 meshColor = vec3(1.0, 0.8, 0.2);\n"
  "out vec3 inputFragmentPosition;\n"
  "out vec3 inputFragmentNormal;\n"
  "out vec3 inputFragmentColor;\n"
  "uniform mat4 projection = mat4(1);\n"
  "uniform mat4 view = mat4(1);\n"
  "uniform mat4 world = mat4(1);\n"
  "void main() {\n"
  "  mat4 worldProjection = projection * view * world;\n"
  "  gl_Position = worldProjection * vec4(vertexPosition, 1.);\n"
  "  inputFragmentPosition = (worldProjection * vec4(vertexPosition, 1.)).xyz;\n"
  "  inputFragmentNormal = vertexNormal;\n"
  "  if(inputFragmentColorType == 1) {\n"
  "    inputFragmentColor = vertexColor;\n"
  "  } else {\n"
  "    inputFragmentColor = meshColor;\n"
  "  }\n"
  "}\n"
  ;

// Fragment shader.
// light type 1:
// directionless, locationless light
// 
// light type 2:
// emit light rays from the points on a plane in the direction of the normal vector
// 
// The color (255, 204, 51) is the websafe color "sunglow".
static const GLchar* Programs_Pbr1_fragmentProgram =
  "#version 330 core\n"
  "out vec4 outputFragmentColor;\n"
  "in vec3 inputFragmentPosition;\n"
  "in vec3 inputFragmentNormal;\n"
  "in vec3 inputFragmentColor;\n"

  /* diffuse lighting (color + direction) */
  "uniform vec3 diffuseLightDirection = vec3(1, 1, 1);\n"
  "uniform vec3 diffuseLightColor = vec3(1, 1, 1);\n"

  /* ambient lighting (color) */
  "uniform vec3 ambientLightColor = vec3(1, 1, 1);\n"

  "void main() {\n"
  "  vec3 n = normalize(inputFragmentNormal);\n"
  "  vec3 d = normalize(diffuseLightDirection);\n"
  "  float diffuseIntensity = max(dot(n, d), 0.0);\n"
  "  vec3 diffuse = diffuseIntensity * diffuseLightColor;\n"
  "  vec3 ambient = ambientLightColor;\n"
  "  vec3 color = (ambient + diffuse) * inputFragmentColor;\n"
  "  outputFragmentColor = vec4(color, 1.f);\n"
  "}\n"
  ;

// Vertex shader.
static const GLchar* Programs_Simple_vertexProgram =
  "#version 330 core\n"
  "layout(location = 0) in vec3 " VertexPositionName ";\n"
  "uniform mat4 scale = mat4(1);\n"
  "void main() {\n"
  "  gl_Position = scale * vec4(vertexPosition.xyz, 1.0);\n"
  "}\n"
  ;

// Fragment shader.
// The color (255, 204, 51) is the websafe color "sunglow".
static const GLchar* Programs_Simple_fragmentProgram =
  "#version 330 core\n"
  "out vec4 outputFragmentColor;\n"
  //"uniform vec4 inputColor = vec4(1.0, 1.0, 1.0, 0.0);\n"
  "uniform vec4 inputColor = vec4(1.0, 0.8, 0.2, 0.0);\n"
  "void main() {\n"
  "  outputFragmentColor = inputColor;\n"
  "}\n"
  ;

/**
 * @since 1.0
 * Get the program of the specified name.
 * There are two programs currently:
 * "simple" and "pbr1".
 */
Visuals_Program* Visuals_getProgram(Shizu_State* state, char const* name) {
  if (!strcmp(name, "simple")) {
    return (Visuals_Program*)
           Visuals_GlProgram_create(state, Shizu_String_create(state, Programs_Simple_vertexProgram, strlen(Programs_Simple_vertexProgram)),
                                           Shizu_String_create(state, Programs_Simple_fragmentProgram, strlen(Programs_Simple_fragmentProgram)));
  } else if (!strcmp(name, "pbr1")) {
    return (Visuals_Program*)
           Visuals_GlProgram_create(state, Shizu_String_create(state, Programs_Pbr1_vertexProgram, strlen(Programs_Pbr1_vertexProgram)),
                                           Shizu_String_create(state, Programs_Pbr1_fragmentProgram, strlen(Programs_Pbr1_fragmentProgram)));
  } else {
    Shizu_State_setError(state, 1);
    Shizu_State_jump(state);
  }
}