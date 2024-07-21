/*
  IdLib Math
  Copyright (C) 2023-2024 Michael Heilmann. All rights reserved.

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

#if !defined(IDLIB_SCALAR_H_INCLUDED)
#define IDLIB_SCALAR_H_INCLUDED

#include "idlib/math/configure.h"

// bool, true, false
#include <stdbool.h>

// NULL
#include <stddef.h>

// uint8_t
#include <inttypes.h>


/// @since 1.2
/// Alias for uint8_t.
typedef uint8_t idlib_u8;

/// @since 1.0
/// Alias for float.
typedef float idlib_f32;

/// @since 1.0
/// Alias for double.
typedef double idlib_f64;


#if _DEBUG

  void
  idlib_debug_assert
    (
      bool expression_value,
      char const* file,
      int line,
      char const* expression_string
    );

  #define IDLIB_DEBUG_ASSERT(expression) \
    idlib_debug_assert(expression, __FILE__, __LINE__, #expression);

#else

  #define IDLIB_DEBUG_ASSERT(expression)

#endif // _DEBUG

/**
 * @since 1.0
 * Symbolic constant for the idlib_f32 representation of Pi.
 */
#define IDLIB_PI_F32 3.1415926f

/**
 * @since 1.0
 * Symbolic constant for the idlib_f64 representation of Pi.
 */
#define IDLIB_PI_F64 3.1415926

/**
 * @since 1.0
 * Convert an angle in degrees into the corresponding angle in radians.
 * @param operand The angle in degrees.
 * @return The corresponding angle in radians.
 */
static inline idlib_f32
idlib_deg_to_rad_f32
  (
    idlib_f32 x
  )
{ return (x / 180.f) * IDLIB_PI_F32; }

/**
 * @since 1.0
 * Convert an angle in degrees into the corresponding angle in radians.
 * @param operand The angle in degrees.
 * @return The corresponding angle in radians.
 */
static inline idlib_f64
idlib_deg_to_rad_f64
  (
    idlib_f64 operand
  )
{ return (operand / 180.) * IDLIB_PI_F64; }

/**
 * @since 1.0
 * Compute the square root.
 * @param operand The operand value.
 * @return The square root of the operand value.
 */
idlib_f32
idlib_sqrt_f32
  (
    idlib_f32 operand
  );

/**
 * @since 1.0
 * Compute the square root.
 * @param operand The value.
 * @return The square root of the value.
 */
idlib_f64
idlib_sqrt_f64
  (
    idlib_f64 operand
  );

/**
 * @since 1.0
 * Compute the cosine.
 * @param operand An angle in radians.
 * @return The cosine of the angle.
 */
idlib_f32
idlib_cos_f32
  (
    idlib_f32 operand
  );

/**
 * @since 1.0
 * Compute the cosine.
 * @param operand An angle in radians.
 * @return The cosine of the angle.
 */
idlib_f64
idlib_cos_f64
  (
    idlib_f64 operand
  );

/**
 * @since 1.0
 * Compute the sine.
 * @param operand An angle in radians.
 * @return The sine of the angle.
 */
idlib_f32
idlib_sin_f32
  (
    idlib_f32 operand
  );

/**
 * @since 1.0
 * Compute the sine.
 * @param operand An angle in radians.
 * @return The sine of the angle.
 */
idlib_f64
idlib_sin_f64
  (
    idlib_f64 operand
  );

/**
 * @since 1.0
 * Compute the tangens.
 * @param operand An angle in radians.
 * @return The tangens of the angle.
 */
idlib_f32
idlib_tan_f32
  (
    idlib_f32 operand
  );

/**
 * @since 1.0
 * Compute the tangens.
 * @param operand An angle in radians.
 * @return The tangens of the angle.
 */
idlib_f64
idlib_tan_f64
  (
    idlib_f64 operand
  );

/**
 * @since 1.0
 * @brief Clamp a value to the range [0,1].
 * @param operand The value to be clamped.
 * @return The clamped value.
 */
static inline idlib_f32
idlib_clamp_f32
  (
    idlib_f32 operand
  )
{
  if (operand < 0.f) return 0.f;
  else if (operand > 1.f) return 1.f;
  else return operand;
}

/**
 * @since 1.0
 * @brief Clamp a value to the range [0,1].
 * @param operand The value to be clamped.
 * @return The clamped value.
 */
static inline idlib_f64
idlib_clamp_f64
  (
    idlib_f64 operand
  )
{
  if (operand < 0.) return 0.;
  else if (operand > 1.) return 1.;
  else return operand;
}

#endif // IDLIB_SCALAR_H_INCLUDED
