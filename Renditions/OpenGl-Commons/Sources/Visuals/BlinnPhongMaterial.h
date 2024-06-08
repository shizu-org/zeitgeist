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

#if !defined(VISUALS_BLINNPHONGMATERIAL_H_INCLUDED)
#define VISUALS_BLINNPHONGMATERIAL_H_INCLUDED

#include "Visuals/Material.h"

Shizu_declareType(Visuals_BlinnPhongMaterial);

struct Visuals_BlinnPhongMaterial {
  Visuals_Material _parent;

  /// [0,255].
  Shizu_Integer32 specular;

  /// [0,255].
  Shizu_Integer32 diffuse;

  /// [0,255].
  Shizu_Integer32 ambient;

  /// [0,255]
  Shizu_Integer32 shininess;

};

struct Visuals_BlinnPhongMaterial_Dispatch {
  Visuals_Material_Dispatch _parent;
};

void
Visuals_BlinnPhongMaterial_construct
  (
    Shizu_State* state,
    Visuals_BlinnPhongMaterial* self
  );

#endif // VISUALS_BLINNPHONGMATERIAL_H_INCLUDED
