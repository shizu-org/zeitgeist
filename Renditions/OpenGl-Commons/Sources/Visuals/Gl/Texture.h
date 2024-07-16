
#if !defined(VISUALS_GL_TEXTURE_H_INCLUDED)
#define VISUALS_GL_TEXTURE_H_INCLUDED

#include "Visuals/Texture.h"
#include "Visuals/Gl/ServiceGl.h"

/// @brief
/// The implementation of Visuals.Texture for OpenGL.
/// @details
/// textureId is the OpenGL representation of the texture.
/// The type is
/// @code
/// class Visuals.Gl.Texture
/// @endcode
/// Its constructor is
/// @code
/// Visuals.Gl.Texture.construct()
/// @endcode
Shizu_declareObjectType(Visuals_Gl_Texture);

struct Visuals_Gl_Texture_Dispatch {
  Visuals_Texture_Dispatch _parent;
};

struct Visuals_Gl_Texture {
  Visuals_Texture parent;
  /// @brief The OpenGL ID of the texture.
  GLuint textureId;
};

Visuals_Gl_Texture*
Visuals_Gl_Texture_create
  (
    Shizu_State2* state
  );

#endif // VISUALS_GL_TEXTURE_H_INCLUDED
