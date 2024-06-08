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

#define VertexPositionName "vertexPosition"
#define VertexNormalName "vertexNormal"
#define VertexColorName "vertexColor"

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
  "layout(location = 2) in vec3 " VertexColorName ";\n"

  "struct FragmentInfo {\n"
  "  vec3 position;\n"
  "  vec3 color;\n"
  "  vec3 normal;\n"
  "  float specular;\n"
  "};\n"

  "struct ViewerInfo {\n"
  "  vec3 position;\n"
  "};\n"

  "struct MatrixInfo {\n"
  "  mat4 projection;\n"
  "  mat4 view;\n"
  "  mat4 world;\n"
  "};\n"

  // declared a second time in the fragment shader.
  "struct PhongInfo {\n"
  "  vec3 ambient;\n"
  "  vec3 diffuse;\n"
  "  vec3 specular;\n"
  "  float shininess;\n"
  "};\n"

  // declared a second time in the fragment shader.
  "struct BlinnPhongInfo {\n"
  "  vec3 ambient;\n"
  "  vec3 diffuse;\n"
  "  vec3 specular;\n"
  "  float shininess;\n"
  "};\n"

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

  // decide if mesh color or vertex color is used.
  "uniform int inputFragmentColorType = 0;\n"
  "uniform vec3 meshColor = vec3(1.0, 0.8, 0.2);\n"
  "uniform MatrixInfo matrices;\n"
  "uniform ViewerInfo viewer;\n"
  "void main() {\n"
  "  mat4 modelToProjectionMatrix = matrices.projection * matrices.view * matrices.world;\n"
  "  mat3 normalMatrix = mat3(transpose(inverse(matrices.world)));"
  "  gl_Position = modelToProjectionMatrix * vec4(vertexPosition, 1.);\n"
  "  _fragment.worldPosition = (matrices.world * vec4(vertexPosition, 1.)).xyz;\n"
  "  _fragment.normal = normalMatrix * vertexNormal;\n"
  "  _fragment.phong.shininess = 0.3;\n"
  "  _fragment.blinnPhong.shininess = 0.3;\n"
  "  _viewer.position = viewer.position;\n"
  "  if(inputFragmentColorType == 1) {\n"
  "    _fragment.color = vertexColor;\n"
  "    _fragment.phong.ambient = vertexColor;\n"
  "    _fragment.phong.diffuse = vertexColor;\n"
  "    _fragment.phong.specular = vertexColor;\n"
  "    _fragment.blinnPhong.ambient = vertexColor;\n"
  "    _fragment.blinnPhong.diffuse = vertexColor;\n"
  "    _fragment.blinnPhong.specular = vertexColor;\n"
  "  } else {\n"
  "    _fragment.color = meshColor;\n"
  "    _fragment.phong.ambient = meshColor;\n"
  "    _fragment.phong.diffuse = meshColor;\n"
  "    _fragment.phong.specular = meshColor;\n"
  "    _fragment.blinnPhong.ambient = meshColor;\n"
  "    _fragment.blinnPhong.diffuse = meshColor;\n"
  "    _fragment.blinnPhong.specular = meshColor;\n"
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

  // Information on the ambient, diffuse, specular, and emissive properties of a fragment.
  "struct PhongInfo {\n"
  // The ambient factor per channel.
  "  vec3 ambient;\n"
  // The diffuse factor per channel.
  "  vec3 diffuse;\n"
  // The specular factor per channel.
  "  vec3 specular;\n"
  "  float shininess;\n"
  "};\n"

  // Information on the ambient, diffuse, specular, and emissive properties of a fragment.
  "struct BlinnPhongInfo {\n"
  // The ambient factor per channel.
  "  vec3 ambient;\n"
  // The diffuse factor per channel.
  "  vec3 diffuse;\n"
  // The specular factor per channel.
  "  vec3 specular;\n"
  "  float shininess;\n"
  "};\n"

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

  "struct ViewerInfo {\n"
  "  vec3 position;\n"  
  "};\n"

  /* Type of light with a direction but without position. */
  "#define LightType_Directional (1)\n"
  /* Type of light with a position but without direction. */
  "#define LightType_Point (2)\n"

  /* Type of light model "Phong". */
  "#define LightModel_Phong (1)\n"
  /* Type of light model "Blinn-Phong". */
  "#define LightModel_BlinnPhong (2)\n"

  "struct SpecularLightInfo {\n"
  "  vec3 color;\n"
  "  vec3 direction;\n"   /* Used if LightType_Directional. */
  "  vec3 position;\n"    /* Used if LighttType_Point. */
  "  int lightType;\n"    /* One of LightType_Directional or LightType_Point. */
  "  int lightModel;\n"   /* One of LightModelPhong or LightModel_BlinnPhong. */
  "};\n"

  "struct DiffuseLightInfo {\n"
  "  vec3 color;\n"
  "  vec3 direction;\n"
  "};\n"

  "struct AmbientLightInfo {\n"
  "  vec3 color;\n"
  "};\n" 

  /* diffuse light info. */
  "uniform DiffuseLightInfo diffuseLightInfo;\n"

  /* ambient light info. */
  "uniform AmbientLightInfo ambientLightInfo;\n"

  /* specular light info. */
  "uniform SpecularLightInfo specularLightInfo;\n"

  "struct FragmentInfo {\n"
  "  vec3 worldPosition;\n"
  "  vec3 color;\n"
  "  vec3 normal;\n"
  "  PhongInfo phong;\n"
  "  BlinnPhongInfo blinnPhong;\n"
  "};\n"

  /*
   * The ambient light.
   */
  "vec3 onAmbient(in FragmentInfo fragment, in AmbientLightInfo ambient) {\n"
  "  vec3 ambientColor = fragment.phong.ambient * ambient.color;\n"
  "  return ambientColor;\n"
  "}\n"

  /*
   * The diffuse light: directional light (direction but no origin).
   */
  "vec3 onDiffuse(in FragmentInfo fragment, in DiffuseLightInfo diffuse) {\n"
  "  fragment.normal = normalize(fragment.normal);\n"
  "  diffuse.direction = normalize(diffuse.direction);\n"
  /* dot(x,cy) = c dot(x,y)*/
  "  float diffuseIntensity = max(dot(fragment.normal, -diffuse.direction), 0.0);\n"
  "  vec3 diffuseColor = diffuseIntensity * fragment.phong.diffuse * diffuse.color;\n"
  "  return diffuseColor;\n"
  "}\n"

  /*
   * The specular light: point light (origin but no direction) or directional light (direction but no origin).
   */
  "vec3 onSpecular(in FragmentInfo fragment, in ViewerInfo viewer, in SpecularLightInfo specular) {\n"
  "  vec3 fragmentToViewerDirection = normalize(viewer.position - fragment.worldPosition);\n"
  "  vec3 lightDirection;\n"
  "  if (specular.lightType == LightType_Point) {\n"
  /* Specular point light. */
  "    lightDirection = normalize(fragment.worldPosition - specular.position);\n"
  "  } else if (specular.lightType == LightType_Directional) {\n"
  /* Specular directional light. */
  "    lightDirection = normalize(specular.direction);\n"
  "  } else {\n"
  "    return vec3(1); \n"
  "  }\n"
  "  vec3 surfaceToLightDirection = -lightDirection;\n;"
  "  float intensity = 0;"
  "  if (specular.lightModel == LightModel_Phong) {\n"
  "   vec3 reflectionDirection = normalize(reflect(lightDirection, fragment.normal));\n"
  "   float term = dot(fragmentToViewerDirection, reflectionDirection);\n"
  "   intensity = pow(max(term, 0.0), fragment.phong.shininess);"
  "   intensity = pow(smoothstep(0.0, 1.0, term), fragment.phong.shininess);\n"
  "   return intensity * fragment.phong.specular * specular.color;\n"
  "  } else if (specular.lightModel == LightModel_BlinnPhong) {\n"
  "   vec3 halfWay = normalize(fragmentToViewerDirection + surfaceToLightDirection);\n"
  "   float term =  dot(fragment.normal, halfWay);\n"
  "   intensity = pow(max(term, 0.0), fragment.phong.shininess);"
  "   intensity = pow(smoothstep(0.0, 1.0, term), fragment.blinnPhong.shininess);\n"
  "   return intensity * fragment.blinnPhong.specular * specular.color;\n"
  "  } else {\n"
  "   return vec3(1);\n"
  "  }\n"
  "}\n"
 
  "vec3 light(in FragmentInfo fragment, in ViewerInfo viewer, in AmbientLightInfo ambient, in DiffuseLightInfo diffuse, in SpecularLightInfo specular) {\n"
  "  vec3 ambientColor = onAmbient(fragment, ambient);\n"
  "  vec3 diffuseColor = onDiffuse(fragment, diffuse);\n"
  "  vec3 specularColor = onSpecular(fragment, viewer, specular);\n"
  "  return (0.3f * ambientColor + 0.3f * diffuseColor + 0.3f * specularColor);\n"
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
  "  vec3 color = light(fragmentInfo, viewerInfo, ambientLightInfo, diffuseLightInfo, specularLightInfo);\n"
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
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
}