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

#if !defined(VISUALS_PHONGMATERIALTECHNIQUE_H_INCLUDED)
#define VISUALS_PHONGMATERIALTECHNIQUE_H_INCLUDED

#include "Visuals/MaterialTechnique.h"

/// @brief
/// A material technique for "Phong" technique.
/// @details
/// The type is
/// @code
/// class Visuals.PhongMaterialTechnique
/// @endcode
/// Its constructor is
/// @code
/// Visuals.PhongMaterialTechnique.construct()
/// @endcode
/// constructing the material technique with default values.
/// The default values are
/// - diffuse = ambient = specular = (85,85,85)
/// - shininesss = 230
Shizu_declareObjectType(Visuals_PhongMaterialTechnique);

struct Visuals_PhongMaterialTechnique {
  Visuals_MaterialTechnique _parent;

  /// The specular reflectance for the red component of light.
  /// [0,255].
  Shizu_Integer32 specularR;
  /// The specular reflectance for the green component of light.
  /// [0,255].
  Shizu_Integer32 specularG;
  /// The specular reflectance for the blue component of light.
  /// [0,255].
  Shizu_Integer32 specularB;

  /// The diffuse reflectance for the red component of light.
  /// [0,255].
  Shizu_Integer32 diffuseR;
  /// The diffuse reflectance for the green component of light.
  /// [0,255].
  Shizu_Integer32 diffuseG;
  /// The diffuse reflectance for the blue component of light.
  /// [0,255].
  Shizu_Integer32 diffuseB;

  /// The ambient reflectance for the red component of light.
  /// [0,255].
  Shizu_Integer32 ambientR;
  /// The ambient reflectance for the green component of light.
  /// [0,255].
  Shizu_Integer32 ambientG;
  /// The ambient reflectance for the blue component of light.
  /// [0,255].
  Shizu_Integer32 ambientB;

  /// The shininesss.
  /// [0,255]
  Shizu_Integer32 shininess;

};

struct Visuals_PhongMaterialTechnique_Dispatch {
  Visuals_MaterialTechnique_Dispatch _parent;
};

Visuals_PhongMaterialTechnique*
Visuals_PhongMaterialTechnique_create
  (
    Shizu_State2* state
  );

#endif // VISUALS_PHONGMATERIALTECHNIQUE_H_INCLUDED
