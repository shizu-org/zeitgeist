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

#include "Visuals/Gl/Program.h"
// strlen
#include <string.h>

#define VertexPositionName "vertexPosition"
#define VertexNormalName "vertexNormal"
#define VertexAmbientName "vertexAmbient"
#define VertexDiffuseName "vertexDiffuse"
#define VertexSpecularName "vertexSpecular"
#define VertexShininessName "vertexShininess"

#define MatrixInfoType \
  /* Information on matrices. */ \
  "struct MatrixInfo {\n" \
  "  mat4 projection;\n" \
  "  mat4 view;\n" \
  "  mat4 world;\n" \
  "};\n"

#define ViewerInfoType \
  /* Information on a viewer. */ \
  "struct ViewerInfo {\n" \
  /* The position of the viewer in world space. */ \
  "  vec3 position;\n" \
  "};\n"

#define PhongInfoType \
  /* Information on the ambient, diffuse, specular, and emissive properties of a fragment. */ \
  "struct PhongInfo {\n" \
    /* The ambient factor per channel.  */ \
    "  vec3 ambient;\n" \
    /* The diffuse factor per channel. */ \
    "  vec3 diffuse;\n" \
    /* The specular factor per channel. */ \
    "  vec3 specular;\n" \
    /* The shininess. */ \
    "  float shininess;\n" \
  "};\n"

#define BlinnPhongInfoType \
  /* Information on the ambient, diffuse, specular, and emissive properties of a fragment. */ \
  "struct BlinnPhongInfo {\n" \
  /* The ambient factor per channel.  */ \
  "  vec3 ambient;\n" \
  /* The diffuse factor per channel. */ \
  "  vec3 diffuse;\n" \
  /* The specular factor per channel. */ \
  "  vec3 specular;\n" \
  /* The shininess. */ \
  "  float shininess;\n" \
  "};\n"

// Blinn-Phong variables.
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
  "layout(location = 2) in vec3 " VertexAmbientName ";\n"
  "layout(location = 3) in vec3 " VertexDiffuseName ";\n"
  "layout(location = 4) in vec3 " VertexSpecularName ";\n"
  "layout(location = 5) in float " VertexShininessName ";\n"
  "#define VertexSemantics_PositionXyz (1)\n"
  "#define VertexSemantics_PositionXyz_NormalXyz_AmbientRgb (2)\n"
  "#define VertexSemantics_PositionXyz_NormalXyz_AmbientRgb_DiffuseRgb_SpecularRgb_Shininess (4)\n"
  "uniform int vertexDescriptor;\n"

  "struct FragmentInfo {\n"
  "  vec3 position;\n"
  "  vec3 color;\n"
  "  vec3 normal;\n"
  "  float specular;\n"
  "};\n"

  MatrixInfoType
  ViewerInfoType
  PhongInfoType
  BlinnPhongInfoType

  "out FRAGMENT {\n"
  "  vec3 worldPosition;\n"
  "  vec3 color;\n"
  "  vec3 normal;\n"
  "  PhongInfo phong;\n"
  "  BlinnPhongInfo blinnPhong;\n"
  "} _fragment;\n"

  "out VIEWER {\n"
  "  vec3 position;\n"
  "} _viewer;\n"

  "uniform PhongInfo phongMaterial;\n"
  "uniform BlinnPhongInfo blinnPhongMaterial;\n"
  "uniform MatrixInfo matrices;\n"
  "uniform ViewerInfo viewer;\n"
  "void main() {\n"
  "  mat4 modelToProjectionMatrix = matrices.projection * matrices.view * matrices.world;\n"
  "  mat3 normalMatrix = mat3(transpose(inverse(matrices.world)));"
  "  gl_Position = modelToProjectionMatrix * vec4(vertexPosition, 1.);\n"
  "  _fragment.worldPosition = (matrices.world * vec4(vertexPosition, 1.)).xyz;\n"
  "  _fragment.normal = normalMatrix * vertexNormal;\n"
  "  _viewer.position = viewer.position;\n"
  // Use the per-mesh Phong/Blinn-Phong information.
  "  _fragment.phong.ambient = phongMaterial.ambient;\n"
  "  _fragment.phong.diffuse = phongMaterial.diffuse;\n"
  "  _fragment.phong.specular = phongMaterial.specular;\n"
  "  _fragment.phong.shininess = phongMaterial.shininess;\n"
  "  _fragment.blinnPhong.ambient = blinnPhongMaterial.ambient;\n"
  "  _fragment.blinnPhong.diffuse = blinnPhongMaterial.diffuse;\n"
  "  _fragment.blinnPhong.specular = blinnPhongMaterial.specular;\n"
  "  _fragment.blinnPhong.shininess = blinnPhongMaterial.shininess;\n"
  // Use the per-vertex Phong/Blinn-Phong information.
  "  if (vertexDescriptor == VertexSemantics_PositionXyz_NormalXyz_AmbientRgb) {\n"
  "    _fragment.phong.ambient = vertexAmbient;\n"
  "    _fragment.blinnPhong.ambient = vertexAmbient;\n"
  "  }\n"
  "  if (vertexDescriptor == VertexSemantics_PositionXyz_NormalXyz_AmbientRgb_DiffuseRgb_SpecularRgb_Shininess) {\n"
  "    _fragment.phong.ambient = vertexAmbient;\n"
  "    _fragment.phong.diffuse = vertexDiffuse;\n"
  "    _fragment.phong.specular = vertexSpecular;\n"
  "    _fragment.phong.shininess = vertexShininess;\n"
  "    _fragment.blinnPhong.ambient = vertexAmbient;\n"
  "    _fragment.blinnPhong.diffuse = vertexDiffuse;\n"
  "    _fragment.blinnPhong.specular = vertexSpecular;\n"
  "    _fragment.blinnPhong.shininess = vertexShininess;\n"
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

  MatrixInfoType
  ViewerInfoType
  PhongInfoType
  BlinnPhongInfoType

  "in FRAGMENT {\n"
  "  vec3 worldPosition;\n"
  "  vec3 color;\n"
  "  vec3 normal;\n"
  "  PhongInfo phong;\n"
  "  BlinnPhongInfo blinnPhong;\n"
  "} _fragment; \n"

  "in VIEWER {\n"
  "  vec3 position;\n"
  "} _viewer;\n"

  "#define LightType_Ambient (4)\n"
  "#define LightType_DirectionalDiffuse (8)\n"
  // @todo Add support.
  //"#define LightType_PointDiffuse (16)\n"
  // @todo Add support.
  //"#define LightType_DirectionalSpecular (32)\n"
  "#define LightType_PointSpecularPhong (64)\n"
  "#define LightType_PointSpecularBlinnPhong (128)\n"

  /* Type of light model "Phong". */
  "#define LightModel_Phong (1)\n"
  /* Type of light model "Blinn-Phong". */
  "#define LightModel_BlinnPhong (2)\n"

  /// The maximum number of lights.
  "#define maximumNumberOfLights 32\n"
  /// The current number of lights.
  "uniform int currentNumberOfLights;\n"

  "struct Light {\n"
  "  int type;\n"
  "  vec3 color;\n"
  "  vec3 position;\n"
  "  vec3 direction;\n"
  "};\n"
  
  "uniform Light g_lights[maximumNumberOfLights];\n"

  "struct FragmentInfo {\n"
  "  vec3 worldPosition;\n"
  "  vec3 color;\n"
  "  vec3 normal;\n"
  "  PhongInfo phong;\n"
  "  BlinnPhongInfo blinnPhong;\n"
  "};\n"

  "vec3 onAmbient2(in FragmentInfo fragment, in Light light) {\n"
  "  return fragment.phong.ambient * light.color;\n"
  "}\n"
  
  "vec3 onDiffuse2(in FragmentInfo fragment, in Light light) {\n"
  "  vec3 fragmentNormal = normalize(fragment.normal);\n"
  "  vec3 lightDirection = normalize(light.direction);\n"
  "  float intensity = max(dot(fragmentNormal, -lightDirection), 0.0);\n"
  "  return intensity * fragment.phong.diffuse * light.color; \n"
  "}\n"

  "vec3 onPointSpecular2(in FragmentInfo fragment, in ViewerInfo viewer, in Light light) {\n"
  "  vec3 fragmentToViewerDirection = normalize(viewer.position - fragment.worldPosition);\n"
  "  vec3 fragmentToLightDirection = normalize(light.position - fragment.worldPosition);\n"
  "  switch (light.type) {\n"
  "    case LightType_PointSpecularPhong: {\n"
  "     vec3 reflectionDirection = normalize(reflect(-fragmentToLightDirection, fragment.normal)); \n"
  "     float term = dot(fragmentToViewerDirection, reflectionDirection);\n"
  "     float intensity;\n"
  "     intensity = pow(max(term, 0.0), fragment.phong.shininess);"
  "     intensity = pow(smoothstep(0.0, 1.0, term), fragment.phong.shininess);\n"
  "     return intensity * fragment.phong.specular * light.color;\n"
  "    } break;\n"
  "    case LightType_PointSpecularBlinnPhong: {\n"
  "      vec3 halfWay = normalize(fragmentToViewerDirection + fragmentToLightDirection);\n"
  "      float term =  dot(fragment.normal, halfWay);\n"
  "      float intensity;\n"
  "      intensity = pow(max(term, 0.0), fragment.phong.shininess);"
  "      intensity = pow(smoothstep(0.0, 1.0, term), fragment.blinnPhong.shininess);\n"
  "      return intensity * fragment.blinnPhong.specular * light.color;\n"
  "    } break;\n"
  "    default: {\n"
  "      return vec3(0);\n"
  "    } break;\n"
  "  };\n"
  "}\n"
   
  "void onLight2(in FragmentInfo fragment, in ViewerInfo viewer, in Light light, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular) {\n"
  "  switch(light.type) {\n"
  "    case LightType_DirectionalDiffuse: {\n"
  "      diffuse += onDiffuse2(fragment, light);\n"
  "    } break;\n"
  "    case LightType_Ambient: {\n"
  "      ambient += onAmbient2(fragment, light);\n"
  "    } break;"
  "    case LightType_PointSpecularPhong:\n"
  "    case LightType_PointSpecularBlinnPhong: {\n"
  "      specular += onPointSpecular2(fragment, viewer, light);\n"
  "    } break;"
  "  };\n"
  "}\n"

  "void main() {\n"
  "  FragmentInfo fragmentInfo;\n"
  "  fragmentInfo.worldPosition = _fragment.worldPosition;\n"
  "  fragmentInfo.color = _fragment.color;\n"
  "  fragmentInfo.normal = _fragment.normal;\n"
  "  fragmentInfo.phong = _fragment.phong;\n"
  "  fragmentInfo.blinnPhong = _fragment.blinnPhong;\n"
  "\n"
  "  ViewerInfo viewerInfo;\n"
  "  viewerInfo.position = _viewer.position;\n"
  "\n"
  "  vec3 ambient = vec3(0), diffuse = vec3(0), specular = vec3(0);\n"
  "  for (int i = 0, n = currentNumberOfLights; i < n; ++i) {\n"
  "    onLight2(fragmentInfo, viewerInfo, g_lights[i], ambient, diffuse, specular);"
  "  }\n"
  "\n"
  "  if (diffuse.x <= 0.f) specular.x = 0.f;\n"
  "  if (diffuse.y <= 0.f) specular.y = 0.f;\n"
  "  if (diffuse.z <= 0.f) specular.z = 0.f;\n"
  "  vec3 color = ambient + diffuse + specular;\n"
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
Visuals_Program* Visuals_getProgram(Shizu_State2* state, char const* name) {
  if (!strcmp(name, "simple")) {
    return (Visuals_Program*)
           Visuals_Gl_Program_create(state, Shizu_String_create(state, Programs_Simple_vertexProgram, strlen(Programs_Simple_vertexProgram)),
                                            Shizu_String_create(state, Programs_Simple_fragmentProgram, strlen(Programs_Simple_fragmentProgram)));
  } else if (!strcmp(name, "pbr1")) {
    return (Visuals_Program*)
           Visuals_Gl_Program_create(state, Shizu_String_create(state, Programs_Pbr1_vertexProgram, strlen(Programs_Pbr1_vertexProgram)),
                                            Shizu_String_create(state, Programs_Pbr1_fragmentProgram, strlen(Programs_Pbr1_fragmentProgram)));
  } else {
    Shizu_State2_setStatus(state, 1);
    Shizu_State2_jump(state);
  }
}