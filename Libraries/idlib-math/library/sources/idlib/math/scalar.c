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

#include "idlib/math/scalar.h"

// sqrt(f), cos(f), sin(f), tan(f)
#include <math.h>

#if _DEBUG

  // fprintf, stderr
  #include <stdio.h>

  // exit, EXIT_FAILURE
  #include <stdlib.h>

  void
  idlib_debug_assert
    (
      bool expression_value,
      char const* file,
      int line,
      char const* expression_string
    )
  {
    if (!expression_value) {
      fprintf(stderr, "%s:%d: debug assertion `%s` failed\n", file, line, expression_string);
      exit(EXIT_FAILURE);
    }

  }

#endif // _DEBUG

idlib_f32
idlib_sqrt_f32
  (
    idlib_f32 operand
  )
{ return sqrtf(operand); }

idlib_f64
idlib_sqrt_f64
  (
    idlib_f64 operand
  )
{ return sqrt(operand); }

idlib_f32
idlib_cos_f32
  (
    idlib_f32 operand
  )
{ return cosf(operand); }

idlib_f64
idlib_cos_f64
  (
    idlib_f64 operand
  )
{ return cos(operand); }

idlib_f32
idlib_sin_f32
  (
    idlib_f32 operand
  )
{ return sinf(operand); }

idlib_f64
idlib_sin_f64
  (
    idlib_f64 operand
  )
{ return sin(operand); }

idlib_f32
idlib_tan_f32
  (
    idlib_f32 operand
  )
{ return tanf(operand); }

idlib_f64
idlib_tan_f64
  (
    idlib_f64 operand
  )
{ return tan(operand); }
