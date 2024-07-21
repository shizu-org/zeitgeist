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

#if !defined(IDLIB_MATRIX_4X4_H_INCLUDED)
#define IDLIB_MATRIX_4X4_H_INCLUDED

#include "scalar.h"
#include "vector_3.h"

// 'Windows.h', which is frequently included in Windows
// programs, defines the macros 'near' and 'far' causing
// an unintended substitution of names of parameters in
// 'matrix_4x4.h'.This prevents this substitution.
#if IDLIB_COMPILER_C == IDLIB_COMPILER_C_MSVC
  #pragma push_macro("near")
  #undef near
  #pragma push_macro("far")
  #undef far
#endif

/// @since 1.0
/// @brief A row-major matrix with elements of type idlib_f32.
/// Row major means: The first index denotes the row, the second index denotes the column.
typedef struct idlib_matrix_4x4_f32 {
  idlib_f32 e[4][4];
} idlib_matrix_4x4_f32;

/// @since 1.4
/// @brief Add an idlib_matrix_4x4_f32 object to another idlib_matrix_4x4_f32 object. Assign the result to a idlib_matrix_4x4_f32 object.
/// @param target Pointer to the idlib_matrix_4x4_f32 object to which the result is assigned.
/// @param operand1 Pointer to the idlib_matrix_4x4_f32 object which is the augend (aka first summand aka first term).
/// @param operand2 Pointer to the idlib_matrix_4x_f32 object which is the addend (aka second summand aka second term).
/// @see https://github.com/michaelheilmann/idlib-math/tree/feature-markdown-documentation/documentation/matrix/matrix_4x4_f32_add.md
static inline void
idlib_matrix_4x4_f32_add
  (
    idlib_matrix_4x4_f32* target,
    idlib_matrix_4x4_f32 const* operand1,
    idlib_matrix_4x4_f32 const* operand2
  );

/// @since 1.4
/// @brief Subtract an idlib_matrix_4x4_f32 object to another idlib_matrix_4x4_f32 object. Assign the result to a idlib_matrix_4x4_f32 object.
/// @param target Pointer to the idlib_matrix_4x4_f32 object to which the result is assigned.
/// @param operand1 Pointer to the idlib_matrix_4x4_f32 object which is the minuend (aka first term).
/// @param operand2 Pointer to the idlib_matrix_4x_f32 object which is the subtrahend (aka second term).
/// @see https://github.com/michaelheilmann/idlib-math/tree/feature-markdown-documentation/feature-markdown-documentation/matrix/matrix_4x4_f32_subtract.md
static inline void
idlib_matrix_4x4_f32_subtract
  (
    idlib_matrix_4x4_f32* target,
    idlib_matrix_4x4_f32 const* operand1,
    idlib_matrix_4x4_f32 const* operand2
  );

/// @since 1.4
/// @brief Assign an idlib_matrix_4x4_f32 object the values of the zero matrix.
/// @param target Pointer to the idlib_matrix_4x4_f32 object to which the result is assigned.
/// @see https://github.com/michaelheilmann/idlib-math/tree/feature-markdown-documentation/documentation/matrix/matrix_4x4_f32_set_zero.md
static inline void
idlib_matrix_4x4_f32_set_zero
  (
    idlib_matrix_4x4_f32* target
  );

/// @since 1.0
/// @brief Assign an idlib_matrix_4x4_f32 object the values of the identity matrix.
/// @param target Pointer to the idlib_matrix_4x4_f32 object to which the result is assigned.
/// @see https://github.com/michaelheilmann/idlib-math/tree/feature-markdown-documentation/matrix/matrix_4x4_f32_set_identity.md
static inline void
idlib_matrix_4x4_f32_set_identity
  (
    idlib_matrix_4x4_f32* target
  );

/// @since 1.0
/// @brief Assign an idlib_matrix_4x4_f32 object the values of a translation matrix.
/// @param target Pointer to the idlib_matrix_4x4_f32 object to assign the result to.
/// @param operand Pointer to an idlib_vector_3_f32 object.
/// Its x, y, and z component values denote the translations along the x-, y-, and z-axis, respectively.
/// @remarks
/// @code
/// | 1 | 0 | 0 | x |
/// | 0 | 1 | 0 | y |
/// | 0 | 0 | 1 | z |
/// | 0 | 0 | 0 | 1 |
/// @endcode
/// with
/// @code
/// x = idlib_vector_3_f32_get_x(operand)
/// y = idlib_vector_3_f32_get_y(operand)
/// z = idlib_vector_3_f32_get_z(operand)
/// @endcode
/// @see https://github.com/michaelheilmann/idlib-math/tree/feature-markdown-documentation/matrix/matrix_4x4_f32_set_translate.md
static inline void
idlib_matrix_4x4_f32_set_translate
  (
    idlib_matrix_4x4_f32* target,
    idlib_vector_3_f32 const* operand
  );

/// @since 1.0
/// @brief Assign an idlib_matrix_4x4_f32 object the values of a "rotation matrix"
/// (for a counter-clockwise rotation around the x-axis).
/// @param target A pointer to the idlib_matrix_4x4_f32 object to assign the result to.
/// @param operand The angle of rotation, in degrees.
/// @remarks
/// @code
/// | 1 | 0 |  0 | 0 |
/// | 0 | c | -s | 0 |
/// | 0 | s |  c | 0 |
/// | 0 | 0 |  0 | 1 |
/// @endcode
/// with
/// @code
/// c = cos(2 * pi * operand1 / 360)
/// s = sin(2 * pi * operand1 / 360)
/// @endcode
/// @see https://github.com/michaelheilmann/idlib-math/tree/feature-markdown-documentation/matrix/matrix_4x4_f32_set_rotation_x.md
static inline void
idlib_matrix_4x4_f32_set_rotation_x
  (
    idlib_matrix_4x4_f32* target,
    idlib_f32 operand
  );

/// @since 1.0
/// @brief Assign an idlib_matrix_4x4_f32 object the values of a "rotation matrix"
/// (for a counter-clockwise rotation around the y-axis).
/// @param target A pointer to the idlib_matrix_4x4_f32 object to assign the result to.
/// @param operand The angle of rotation, in degrees.
/// @remarks
/// @code
/// |  c | 0 | s | 0 |
/// |  0 | 1 | 0 | 0 |
/// | -s | 0 | c | 0 |
/// |  0 | 0 | 0 | 1 |
/// @endcode
/// with
/// @code
/// c = cos(2 * pi * operand1 / 360)
/// s = sin(2 * pi * operand1 / 360)
/// @endcode
/// @see https://github.com/michaelheilmann/idlib-math/tree/feature-markdown-documentation/matrix/matrix_4x4_f32_set_rotation_y.md
static inline void
idlib_matrix_4x4_f32_set_rotation_y
  (
    idlib_matrix_4x4_f32* target,
    idlib_f32 operand
  );

/// @since 1.0
/// @brief Assign an idlib_matrix_4x4_f32 object the values of a "rotation matrix"
/// (for a counter-clockwise rotation around the z-axis).
/// @param target A pointer to the idlib_matrix_4x4_f32 object to assign the result to.
/// @param operand The angle of rotation, in degrees.
/// @remarks
/// @code
/// | c | -s | 0 | 0 |
/// | s |  c | 0 | 0 |
/// | 0 |  0 | 1 | 0 |
/// | 0 |  0 | 0 | 1 |
/// @endcode
/// with
/// @code
/// c = cos(2 * pi * operand1 / 360)
/// s = sin(2 * pi * operand1 / 360)
/// @endcode
/// @see https://github.com/michaelheilmann/idlib-math/tree/feature-markdown-documentation/matrix/matrix_4x4_f32_set_rotation_z.md
static inline void
idlib_matrix_4x4_f32_set_rotation_z
  (
    idlib_matrix_4x4_f32* target,
    idlib_f32 operand
  );

/// @since 1.0
/// @brief Assign an idlib_matrix_4x4_f32 object the values of an "orthographic projection matrix".
/// @param target A pointer to the idlib_matrix_4x4_f32 object to assign the result to.
/// @param left The distance to the left clip plane.
/// @param right The distance to the right clip plane.
/// @param bottom The distance to the bottom clip plane.
/// @param top The distance to the top clip plane.
/// @param near The distance to the near clip plane.
/// @param far The distance to the far clip plane.
/// @remarks
/// The following matrix is created
/// @code
/// | 2/a | 0   | 0     | u |
/// | 0   | 2/b | 0     | v |
/// | 0   | 0   | - 2/c | w |
/// | 0   | 0   | 0     | 1 |
/// @endcode
/// where
/// @code
/// a = right - left
/// b = top - bottom
/// c = far - near
/// u = -(right + left) / a
/// v = -(top + bottom) / b
/// w = -(far + near) / 2c
/// @endcode
/// @remarks
/// A few properties of the transformation
/// - the positive z-axis points out of the screen (negative z-axis points into the screen)
/// - the positive x-axis points to the right
/// - the positive y-axis points to the top
static inline void
idlib_matrix_4x4_f32_set_orthographic
  (
    idlib_matrix_4x4_f32* target,
    idlib_f32 left,
    idlib_f32 right,
    idlib_f32 bottom,
    idlib_f32 top,
    idlib_f32 near,
    idlib_f32 far
  );

/// @since 1.0
/// @brief Assign an idlib_matrix_4x4_f32 object the values of a "perspective projection matrix".
/// @param target A pointer to the idlib_matrix_4x4_f32 object to assign the result to.
/// @param field_of_view_y The field of view along the y-axis in degrees.
/// In other terms: The angle, in degrees, in between a plane passing through the camera position as well as the top of your screen and another plane passing
/// through the camera position and the bottom of your screen.  The bigger this angle is, the more you can see of the world - but at the same time, the objects
/// you can see will become smaller.
/// @param aspect_ratio The aspect ratio, that is, the ratio of the width to the height of the screen.
/// An aspect ratio of x means that the width is x times the height.
/// The aspect ratio is usually computed by width / height.
/// @param near The distance to the near clip plane.
/// @param far The distance to the far clip plane.
/// @remarks
/// @remarks
/// This function creates the following matrix
/// @code
/// | f / aspectRatio | 0                       | 0                          0 |
/// | 0               | f                       | 0                          0 |
/// | 0               | 0 (far+near)/(near-far) | (2 * far *  near)/(near-far) |
/// | 0               | 0                    -1 |                            0 |
/// @endcode
/// where
/// @code
/// f = cot(fieldOfVision/2)
/// @endcode
/// @remarks
/// A few properties of the transformation
/// - the positive z-axis points out of the screen (negative z-axis points into the screen)
/// - the positive x-axis points to the right
/// - the positive y-axis points to the top
static inline void
idlib_matrix_4x4_f32_set_perspective
  (
    idlib_matrix_4x4_f32* target,
    idlib_f32 field_of_view_y,
    idlib_f32 aspect_ratio,
    idlib_f32 near,
    idlib_f32 far
  );

/// @since 1.0
/// @brief Compute the product of two matrices.
/// @param target Pointer to a idlib_matrix_4x4_f32 object to assign the result to.
/// @param operand1 Pointer to a idlib_matrix_4x4_f32 object, the multiplier (first operand).
/// @param operand2 Pointer to a idlib_matrix_4x4_f32 object, the multiplicand (second operand).
/// @remarks @a target, @a operand1, and @a operand2 all may refer to the same object.
static inline void
idlib_matrix_4x4_f32_multiply
  (
    idlib_matrix_4x4_f32* target,
    idlib_matrix_4x4_f32 const* operand1,
    idlib_matrix_4x4_f32 const* operand2
  );

/// @since 1.0
/// @brief Assign this matrix the value a of a view matrix.
/// @param target A pointer to the idlib_matrix_4x4_f32 object to assign the result to.
/// @param source A pointer to the idlib_matrix_4x4_f32 object representing the point at which the viewer is positioned at
/// @param target A pointer to the idlib_matrix_4x4_f32 object representing the point at which the viewer is looking at
/// @param up A pointer to the idlib_matrix_4x4_f32 object representing the upward direction of the viewer.
/// @remarks
/// This function constructs a view matrix <code>V</code>given
/// - the position the viewer is located at <code>source</code>,
/// - the position the viewer is looking at <code>target</code>, and
/// - the vector indicating the up direction of the viewer <code>up</code>.
/// The view matrix <code>V</code> is constructed as follows
/// Let
/// @code
/// forward := norm(target - source)
/// right := forward x norm(up)
/// up' := right x forward
/// @endcode
/// Then the view matrix <code>V</code> is given by
/// @code
/// V :=
/// | right.x    | right.y    | right.z    | 0
/// | up'.x      | up'.y      | u'.z       | 0
/// | -forward.x | -forward.y | -forward.z | 0
/// | 0          | 0          | 0          | 1
/// @endcode
static inline void
idlib_matrix_4x4_f32_set_look_at
  (
    idlib_matrix_4x4_f32* target,
    idlib_vector_3_f32 const* operand1,
    idlib_vector_3_f32 const* operand2,
    idlib_vector_3_f32 const* operand3
  );

/// @since 1.1
/// @brief Assign this matrix the values of scaling matrix representing.
/// @param target A pointer to the idlib_matrix_4x4_f32 object to assign the result to.
/// @param operand A pointer to the idlib_vector_3_f32 object.
/// Its x, y, and z component values denote the scalings along the x-, y-, and z-axis, respectively.
/// @remarks
/// @code
/// | x | 0 | 0 | 0 |
/// | 0 | y | 0 | 0 |
/// | 0 | 0 | z | 0 |
/// | 0 | 0 | 0 | 1 |
/// @endcode
/// where
/// @code
/// x = idlib_vector_3_f32_get_x(operand)
/// y = idlib_vector_3_f32_get_y(operand)
/// z = idlib_vector_3_f32_get_z(operand)
/// @endcode
static inline void
idlib_matrix_4x4_f32_set_scale
  (
    idlib_matrix_4x4_f32* target,
    idlib_vector_3_f32* operand
  );

/// @since 1.1
/// @brief Get a pointer to the data of a idlib_matrix_4x4_f32 object.
/// @param operand A pointer to the idlib_matrix_4x4_f32 object.
/// @return A pointer to the data. The pointer remains valid as long as the object remains valid and is not modified.
static inline void*
idlib_matrix_4x4_f32_get_data
  (
    idlib_matrix_4x4_f32* operand
  );

/// @since 1.1
/// @brief Negate a matrix.
/// @param target Pointer to the idlib_matrix_4x4_f32 object to assign the result to.
/// @param operand Pointer to the idlib_matrix_4x4_f32 object to negate.
/// @remarks @a target and @a operand all may refer to the same idlib_matrix_4x4_f32 object.
static inline void
idlib_matrix_4x4_f32_negate
  (
    idlib_matrix_4x4_f32* target,
    idlib_matrix_4x4_f32 const* operand
  );

/// @since 1.3
/// @brief Transpose a matrix.
/// @param target A pointer to the idlib_matrix_4x4_f32 object to assign the result to.
/// @param operand Pointer to the idlib_matrix_4x4_f32 object to transpose.
/// @remarks @a target and @a operand all may refer to the same idlib_matrix_4x4_f32 object.
/// @see https://github.com/michaelheilmann/idlib-math/tree/feature-markdown-documentation/documentation/matrix/matrix_4x4_f32_transpose.md
static inline void
idlib_matrix_4x4_f32_transpose
  (
    idlib_matrix_4x4_f32* target,
    idlib_matrix_4x4_f32 const* operand
  );

/// @since 1.3
/// @brief Transform a position vector.
/// @param target Pointer to an idlib_vector_3_f32 object receiving the result.
/// @param operand1 Pointer to an idlib_matrix_4x4_f32 object, the multiplier (first operand).
/// @param operand2 Pointer to an idlib_vector_3_f32 object, the multiplicand (second operand).
static inline void
idlib_matrix_4x4_3f_transform_point
  (
    idlib_vector_3_f32* target,
    idlib_matrix_4x4_f32 const* operand1,
    idlib_vector_3_f32 const* operand2
  );

/// @since 1.3
/// @brief Transform a direction vector.
/// @param target Pointer to an idlib_vector_3_f32 object receiving the result.
/// @param operand1 Pointer to an idlib_matrix_4x4_f32 object, the multiplier (first operand).
/// @param operand2 Pointer to an idlib_vector_3_f32 object, the multiplicand (second operand).
static inline void
idlib_matrix_4x4_3f_transform_direction
  (
    idlib_vector_3_f32* target,
    idlib_matrix_4x4_f32 const* operand1,
    idlib_vector_3_f32 const* operand2
  );

static inline void
idlib_matrix_4x4_f32_add
  (
    idlib_matrix_4x4_f32* target,
    idlib_matrix_4x4_f32 const* operand1,
    idlib_matrix_4x4_f32 const* operand2
  )
{
  IDLIB_DEBUG_ASSERT(NULL != target);
  IDLIB_DEBUG_ASSERT(NULL != operand1);
  IDLIB_DEBUG_ASSERT(NULL != operand2);

  for (size_t i = 0; i < 4; ++i) {
    for (size_t j = 0; j < 4; ++j) {
      target->e[i][j] = operand1->e[i][j] + operand2->e[i][j];
    }
  }
}

static inline void
idlib_matrix_4x4_f32_subtract
  (
    idlib_matrix_4x4_f32* target,
    idlib_matrix_4x4_f32 const* operand1,
    idlib_matrix_4x4_f32 const* operand2
  )
{
  IDLIB_DEBUG_ASSERT(NULL != target);
  IDLIB_DEBUG_ASSERT(NULL != operand1);
  IDLIB_DEBUG_ASSERT(NULL != operand2);

  for (size_t i = 0; i < 4; ++i) {
    for (size_t j = 0; j < 4; ++j) {
      target->e[i][j] = operand1->e[i][j] - operand2->e[i][j];
    }
  }
}

static inline void
idlib_matrix_4x4_f32_set_zero
  (
    idlib_matrix_4x4_f32* target
  )
{
  IDLIB_DEBUG_ASSERT(NULL != target);

  // first column
  target->e[0][0] = 0.f;
  target->e[1][0] = 0.f;
  target->e[2][0] = 0.f;
  target->e[3][0] = 0.f;

  // second column
  target->e[0][1] = 0.f;
  target->e[1][1] = 0.f;
  target->e[2][1] = 0.f;
  target->e[3][1] = 0.f;

  // third column
  target->e[0][2] = 0.f;
  target->e[1][2] = 0.f;
  target->e[2][2] = 0.0f;
  target->e[3][2] = 0.f;

  // fourth column
  target->e[0][3] = 0.f;
  target->e[1][3] = 0.f;
  target->e[2][3] = 0.0f;
  target->e[3][3] = 0.f;
}

static inline void
idlib_matrix_4x4_f32_set_identity
  (
    idlib_matrix_4x4_f32* target
  )
{
  IDLIB_DEBUG_ASSERT(NULL != target);

  // first column
  target->e[0][0] = 1.f;
  target->e[1][0] = 0.f;
  target->e[2][0] = 0.f;
  target->e[3][0] = 0.f;

  // second column
  target->e[0][1] = 0.f;
  target->e[1][1] = 1.f;
  target->e[2][1] = 0.f;
  target->e[3][1] = 0.f;

  // third column
  target->e[0][2] = 0.f;
  target->e[1][2] = 0.f;
  target->e[2][2] = 1.0f;
  target->e[3][2] = 0.f;

  // fourth column
  target->e[0][3] = 0.f;
  target->e[1][3] = 0.f;
  target->e[2][3] = 0.0f;
  target->e[3][3] = 1.f;
}

static inline void
idlib_matrix_4x4_f32_set_translate
  (
    idlib_matrix_4x4_f32* target,
    idlib_vector_3_f32 const* operand
  )
{
  IDLIB_DEBUG_ASSERT(NULL != target);
  IDLIB_DEBUG_ASSERT(NULL != operand);

  // first column
  target->e[0][0] = 1.f;
  target->e[1][0] = 0.f;
  target->e[2][0] = 0.f;
  target->e[3][0] = 0.f;

  // second column
  target->e[0][1] = 0.f;
  target->e[1][1] = 1.f;
  target->e[2][1] = 0.f;
  target->e[3][1] = 0.f;

  // third column
  target->e[0][2] = 0.f;
  target->e[1][2] = 0.f;
  target->e[2][2] = 1.f;
  target->e[3][2] = 0.f;

  // column #4
  target->e[0][3] = operand->e[0];
  target->e[1][3] = operand->e[1];
  target->e[2][3] = operand->e[2];
  target->e[3][3] = 1.f;
}

static inline void
idlib_matrix_4x4_f32_set_rotation_x
  (
    idlib_matrix_4x4_f32* target,
    idlib_f32 operand
  )
{
  IDLIB_DEBUG_ASSERT(NULL != target);

  idlib_f32 a = idlib_deg_to_rad_f32(operand);
  idlib_f32 c = idlib_cos_f32(a);
  idlib_f32 s = idlib_sin_f32(a);

  // First column.
  target->e[0][0] = 1.f;
  target->e[1][0] = 0.f;
  target->e[2][0] = 0.f;
  target->e[3][0] = 0.f;

  // Second column.
  target->e[0][1] = 0.f;
  target->e[1][1] = c;
  target->e[2][1] = s;
  target->e[3][1] = 0.f;

  // Third column.
  target->e[0][2] = 0.f;
  target->e[1][2] = -s;
  target->e[2][2] = c;
  target->e[3][2] = 0.f;

  // Fourth column.
  target->e[0][3] = 0.f;
  target->e[1][3] = 0.f;
  target->e[2][3] = 0.f;
  target->e[3][3] = 1.f;
}

static inline void
idlib_matrix_4x4_f32_set_rotation_y
  (
    idlib_matrix_4x4_f32* target,
    idlib_f32 operand
  )
{
  IDLIB_DEBUG_ASSERT(NULL != target);

  idlib_f32 a = idlib_deg_to_rad_f32(operand);
  idlib_f32 c = idlib_cos_f32(a);
  idlib_f32 s = idlib_sin_f32(a);

  // First column.
  target->e[0][0] = c;
  target->e[1][0] = 0.f;
  target->e[2][0] = -s;
  target->e[3][0] = 0.f;

  // Second column.
  target->e[0][1] = 0.f;
  target->e[1][1] = 1.f;
  target->e[2][1] = 0.f;
  target->e[3][1] = 0.f;

  // Third column.
  target->e[0][2] = s;
  target->e[1][2] = 0.f;
  target->e[2][2] = c;
  target->e[3][2] = 0.f;

  // Fourth column.
  target->e[0][3] = 0.f;
  target->e[1][3] = 0.f;
  target->e[2][3] = 0.f;
  target->e[3][3] = 1.f;
}

static inline void
idlib_matrix_4x4_f32_set_rotation_z
  (
    idlib_matrix_4x4_f32* target,
    idlib_f32 operand
  )
{
  IDLIB_DEBUG_ASSERT(NULL != target);

  idlib_f32 a = idlib_deg_to_rad_f32(operand);
  idlib_f32 c = idlib_cos_f32(a);
  idlib_f32 s = idlib_sin_f32(a);

  // First column.
  target->e[0][0] = c;
  target->e[1][0] = s;
  target->e[2][0] = 0.f;
  target->e[3][0] = 0.f;

  // Second column.
  target->e[0][1] = -s;
  target->e[1][1] = c;
  target->e[2][1] = 0.f;
  target->e[3][1] = 0.f;

  // Third column.
  target->e[0][2] = 0.f;
  target->e[1][2] = 0.f;
  target->e[2][2] = 1.f;
  target->e[3][2] = 0.f;

  // Fourth column.
  target->e[0][3] = 0.f;
  target->e[1][3] = 0.f;
  target->e[2][3] = 0.f;
  target->e[3][3] = 1.f;
}

inline void
idlib_matrix_4x4_f32_set_orthographic
  (
    idlib_matrix_4x4_f32* target,
    idlib_f32 left,
    idlib_f32 right,
    idlib_f32 bottom,
    idlib_f32 top,
    idlib_f32 near,
    idlib_f32 far
  )
{
  IDLIB_DEBUG_ASSERT(NULL != target);

  idlib_f32 a = right - left;
  idlib_f32 b = top - bottom;
  idlib_f32 c = far - near;

  idlib_f32 u = -(right + left) / a;
  idlib_f32 v = -(top + bottom) / b;
  idlib_f32 w = -(far + near) / c;

  // column #1
  target->e[0][0] = 2.f / a;
  target->e[1][0] = 0.f;
  target->e[2][0] = 0.f;
  target->e[3][0] = 0.f;

  // column #2
  target->e[0][1] = 0.f;
  target->e[1][1] = 2.f / b;
  target->e[2][1] = 0.f;
  target->e[3][1] = 0.f;

  // column #3
  target->e[0][2] = 0.f;
  target->e[1][2] = 0.f;
  target->e[2][2] = -2.f / c;
  target->e[3][2] = 0.f;

  // column #4
  target->e[0][3] = u;
  target->e[1][3] = v;
  target->e[2][3] = w;
  target->e[3][3] = 1.f;
}

inline void
idlib_matrix_4x4_f32_set_perspective
  (
    idlib_matrix_4x4_f32* target,
    idlib_f32 field_of_view_y,
    idlib_f32 aspect_ratio,
    idlib_f32 near,
    idlib_f32 far
  )
{
  IDLIB_DEBUG_ASSERT(NULL != target);

  field_of_view_y = idlib_deg_to_rad_f32(field_of_view_y); // rad(x) = x / 360 * 2 * PI = x * (PI * / 180)
  idlib_f32 f = 1.f / idlib_tan_f32(field_of_view_y / 2.f); // cot(x) = 1 / tan(x)

  // column #1
  target->e[0][0] = f / aspect_ratio;
  target->e[1][0] = 0.f;
  target->e[2][0] = 0.f;
  target->e[3][0] = 0.f;

  // column #2
  target->e[0][1] = 0.f;
  target->e[1][1] = f;
  target->e[2][1] = 0.f;
  target->e[3][1] = 0.f;

  // column #3
  target->e[0][2] = 0.f;
  target->e[1][2] = 0.f;
  target->e[2][2] = (far + near) / (near - far);
  target->e[3][2] = -1.f;

  // column #4
  target->e[0][3] = 0.f;
  target->e[1][3] = 0.f;
  target->e[2][3] = (2.f * far * near) / (near - far); // - (2 far near) / (far - near)
  target->e[3][3] = 0.f;
}

#if IDLIB_COMPILER_C == IDLIB_COMPILER_C_MSVC
  #pragma pop_macro("near")
  #pragma pop_macro("far")
#endif

static inline void
idlib_matrix_4x4_f32_multiply
  (
    idlib_matrix_4x4_f32* target,
    idlib_matrix_4x4_f32 const* operand1,
    idlib_matrix_4x4_f32 const* operand2
  )
{
  IDLIB_DEBUG_ASSERT(NULL != target);
  IDLIB_DEBUG_ASSERT(NULL != operand1);
  IDLIB_DEBUG_ASSERT(NULL != operand2);

  if (target != operand1 && target != operand2) {
    for (size_t i = 0; i < 4; ++i) {
      for (size_t j = 0; j < 4; ++j) {
        target->e[i][j] = 0.f;
        for (size_t k = 0; k < 4; ++k) {
          target->e[i][j] += operand1->e[i][k] * operand2->e[k][j];
        }
      }
    }
  } else {
    idlib_f32 t[4][4];

    for (size_t i = 0; i < 4; ++i) {
      for (size_t j = 0; j < 4; ++j) {
        t[i][j] = 0.f;
        for (size_t k = 0; k < 4; ++k) {
          t[i][j] += operand1->e[i][k] * operand2->e[k][j];
        }
      }
    }
    for (size_t i = 0; i < 4; ++i) {
      for (size_t j = 0; j < 4; ++j) {
        target->e[i][j] = t[i][j];
      }
    }
  }
}

static inline void
idlib_matrix_4x4_f32_set_look_at
  (
    idlib_matrix_4x4_f32* target,
    idlib_vector_3_f32 const* operand1,
    idlib_vector_3_f32 const* operand2,
    idlib_vector_3_f32 const* operand3
  )
{
  IDLIB_DEBUG_ASSERT(NULL != target);
  IDLIB_DEBUG_ASSERT(NULL != operand1);
  IDLIB_DEBUG_ASSERT(NULL != operand2);
  IDLIB_DEBUG_ASSERT(NULL != operand3);

  idlib_vector_3_f32 forward, right, up1, up2;
  idlib_matrix_4x4_f32 r, t;

  // forward := norm(target - source)
  idlib_vector_3_f32_subtract(&forward, operand2, operand1);
  idlib_vector_3_f32_normalize(&forward, &forward);
  // right := forward x norm(up)
  idlib_vector_3_f32_normalize(&up1, operand3);
  idlib_vector_3_f32_cross(&right, &forward, &up1);
  // up' := right x forward
  idlib_vector_3_f32_cross(& up2, & right, & forward);

  // First column.
  r.e[0][0] = right.e[0];
  r.e[1][0] = up2.e[0];
  r.e[2][0] = -forward.e[0];
  r.e[3][0] = 0.f;
  // Second column.
  r.e[0][1] = right.e[1];
  r.e[1][1] = up2.e[1];
  r.e[2][1] = -forward.e[1];
  r.e[3][1] = 0.f;
  // Third column.
  r.e[0][2] = right.e[2];
  r.e[1][2] = up2.e[2];
  r.e[2][2] = -forward.e[2];
  r.e[3][2] = 0.f;
  // Fourth column.
  r.e[0][3] = 0.f;
  r.e[1][3] = 0.f;
  r.e[2][3] = 0.f;
  r.e[3][3] = 1.f;

  idlib_vector_3_f32 negae;
  negae = *operand1;
  idlib_vector_3_f32_negate(&negae, &negae);
  idlib_matrix_4x4_f32_set_translate(&t, &negae);

  idlib_matrix_4x4_f32_multiply(target, &r, &t);
}

static inline void
idlib_matrix_4x4_f32_set_scale
  (
    idlib_matrix_4x4_f32* target,
    idlib_vector_3_f32* operand
  )
{
  IDLIB_DEBUG_ASSERT(NULL != target);
  IDLIB_DEBUG_ASSERT(NULL != operand);

  // First column.
  target->e[0][0] = operand->e[0];
  target->e[1][0] = 0.f;
  target->e[2][0] = 0.f;
  target->e[3][0] = 0.f;
  // Second column.
  target->e[0][1] = 0.f;
  target->e[1][1] = operand->e[1];
  target->e[2][1] = 0.f;
  target->e[3][1] = 0.f;
  // Third column.
  target->e[0][2] = 0.f;
  target->e[1][2] = 0.f;
  target->e[2][2] = operand->e[2];
  target->e[3][2] = 0.f;
  // Fourth column.
  target->e[0][3] = 0.f;
  target->e[1][3] = 0.f;
  target->e[2][3] = 0.f;
  target->e[3][3] = 1.f;
}

static inline void*
idlib_matrix_4x4_f32_get_data
  (
    idlib_matrix_4x4_f32* operand
  )
{ return &(operand->e[0][0]); }

static inline void
idlib_matrix_4x4_f32_negate
  (
    idlib_matrix_4x4_f32* target,
    idlib_matrix_4x4_f32 const* operand
  )
{
  IDLIB_DEBUG_ASSERT(NULL != target);
  IDLIB_DEBUG_ASSERT(NULL != operand);

  for (size_t i = 0; i < 4; ++i) {
    for (size_t j = 0; j < 4; ++j) {
      target->e[i][j] = -operand->e[i][j];
    }
  }
}

static inline idlib_f32
idlib_matrix_4x4_f32_determinant
  (
    idlib_matrix_4x4_f32 const* operand
  )
{
  IDLIB_DEBUG_ASSERT(NULL != operand);
  idlib_f32 det = 0.f;
  
  #define e(i,j) operand->e[i][j]
  
  // Cofactor expansion along first row.
  det += e(0,0) * (  e(1,1)*e(2,2)*e(3,3) + e(1,2)*e(2,3)*e(3,1) + e(1,3)*e(2,1)*e(3,2)
                   - e(1,3)*e(2,2)*e(3,1) - e(1,1)*e(2,3)*e(3,2) - e(1,2)*e(2,1)*e(3,3) );
  det -= e(0,1) * (  e(1,0)*e(2,2)*e(3,3) + e(1,2)*e(2,3)*e(3,0) + e(1,3)*e(2,0)*e(3,2)
                   - e(1,3)*e(2,2)*e(3,0) - e(1,0)*e(2,3)*e(3,2) - e(1,2)*e(2,0)*e(3,3) );
  det += e(0,2) * (  e(1,0)*e(2,1)*e(3,3) + e(1,1)*e(2,3)*e(3,0) + e(1,3)*e(2,0)*e(3,1)
                   - e(1,3)*e(2,1)*e(3,0) - e(1,0)*e(2,3)*e(3,1) - e(1,1)*e(2,0)*e(3,3) );
  det -= e(0,3) * (  e(1,0)*e(2,1)*e(3,2) + e(1,1)*e(2,2)*e(3,0) + e(1,2)*e(2,0)*e(3,1)
                   - e(1,2)*e(2,1)*e(3,0) - e(1,0)*e(2,2)*e(3,1) - e(1,1)*e(2,0)*e(3,2) );
                   
  #undef e
                   
  return det;
}

static inline void
idlib_matrix_4x4_f32_transpose
  (
    idlib_matrix_4x4_f32* target,
    idlib_matrix_4x4_f32 const* operand
  )
{
  IDLIB_DEBUG_ASSERT(NULL != target);
  IDLIB_DEBUG_ASSERT(NULL != operand);

  /*
    * Given a matrix
    * (0,0)(0,1)(0,2)(0,3)
    * (1,0)(1,1)(1,2)(1,3)
    * (2,0)(2,1)(2,2)(2,3)
    * (3,0)(3,1)(3,2)(3,3)
    * the algorith below is correct as
    * i = 0, i < 4 continue
    * j = 0, 0 < 0 = false => abort
    * i = 1, i < 4 = true => swap continue
    * j = 0, 0 < 1 = true => swap(1,0)
    * j = 1, 1 < 1 = false => abort
    * i = 2, i < 4 = true => continue
    * j = 0, 0 < 2 = true => swap(2,0)
    * j = 1, 1 < 2 = true => swap(2,1)
    * j = 2, 2 < 2 = false => abort
    * i = 3, i < 4 = true => continue
    * j = 0, 0 < 3 = true => swap(3,0)
    * j = 1, 1 < 3 = true => swap(3,1)
    * j = 2, 2 < 3 = true => swap(3,2)
    * j = 3, 3 < 3 = false => abort,
    * i = 4, i < 4 = false => abort
    */
  if (target == operand) {
    // in place transpose

    #if IDLIB_COMPILER_C == IDLIB_COMPILER_C_MSVC
    #pragma push_macro("SWAP")
    #undef SWAP
    #endif

    #define SWAP(x, y) \
      { \
        idlib_f32 t = operand->e[x][y]; \
        target->e[x][y] = operand->e[y][x]; \
        target->e[y][x] = t; \
      }

    // rows from top to bottom
    for (size_t i = 0; i < 4; ++i) {
      for (size_t j = 1; j < i; ++j) {
        SWAP(i, j);
      }
    }

    #undef SWAP

    #if defined(_MSVC_VER)
    #pragma pop_macro("SWAP")
    #endif

  } else {
    // out of place transpose

    #if IDLIB_COMPILER_C == IDLIB_COMPILER_C_MSVC
    #pragma push_macro("SWAP")
    #undef SWAP
    #endif

    #define SWAP(x, y) \
      { \
        target->e[y][x] = operand->e[y][x]; \
      }

    // rows from top to bottom
    for (size_t i = 0; i < 4; ++i) {
      for (size_t j = i; j < i; ++j) {
        SWAP(i, j);
      }
    }
    
    #undef SWAP

    #if defined(_MSVC_VER)
    #pragma pop_macro("SWAP")
    #endif
  }
}

static inline void
idlib_matrix_4x4_3f_transform_point
  (
    idlib_vector_3_f32* target,
    idlib_matrix_4x4_f32 const* operand1,
    idlib_vector_3_f32 const* operand2
  )
{
  IDLIB_DEBUG_ASSERT(NULL != target);
  IDLIB_DEBUG_ASSERT(NULL != operand1);
  IDLIB_DEBUG_ASSERT(NULL != operand2);

  idlib_f32 e[3];

  e[0] = operand1->e[0][0] * operand2->e[0]
       + operand1->e[0][1] * operand2->e[1]
       + operand1->e[0][2] * operand2->e[2]
       + operand1->e[0][3] * 1.f;

  e[1] = operand1->e[1][0] * operand2->e[0]
       + operand1->e[1][1] * operand2->e[1]
       + operand1->e[1][2] * operand2->e[2]
       + operand1->e[1][3] * 1.f;

  e[2] = operand1->e[2][0] * operand2->e[0]
       + operand1->e[2][1] * operand2->e[1]
       + operand1->e[2][2] * operand2->e[2]
       + operand1->e[2][3] * 1.f;

  target->e[0] = e[0];
  target->e[1] = e[1];
  target->e[2] = e[2];
}

static inline void
idlib_matrix_4x4_3f_transform_direction
  (
    idlib_vector_3_f32* target,
    idlib_matrix_4x4_f32 const* operand1,
    idlib_vector_3_f32 const* operand2
  )
{
  IDLIB_DEBUG_ASSERT(NULL != target);
  IDLIB_DEBUG_ASSERT(NULL != operand1);
  IDLIB_DEBUG_ASSERT(NULL != operand2);

  idlib_f32 e[3];

  e[0] = operand1->e[0][0] * operand2->e[0]
       + operand1->e[0][1] * operand2->e[1]
       + operand1->e[0][2] * operand2->e[2]
       + operand1->e[0][3] * 0.f;

  e[1] = operand1->e[1][0] * operand2->e[0]
       + operand1->e[1][1] * operand2->e[1]
       + operand1->e[1][2] * operand2->e[2]
       + operand1->e[1][3] * 0.f;

  e[2] = operand1->e[2][0] * operand2->e[0]
       + operand1->e[2][1] * operand2->e[1]
       + operand1->e[2][2] * operand2->e[2]
       + operand1->e[2][3] * 0.f;

  target->e[0] = e[0];
  target->e[1] = e[1];
  target->e[2] = e[2];
}

#endif // IDLIB_MATRIX_4X4_H_INCLUDED
