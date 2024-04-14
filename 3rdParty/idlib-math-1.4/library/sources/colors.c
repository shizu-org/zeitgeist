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

#include "colors.h"

#if IDLIB_COMPILER_C == IDLIB_COMPILER_C_MSVC
  #define STATIC_ASSERT(CONDITION, MESSAGE) static_assert(CONDITION, MESSAGE)
#elif IDLIB_COMPILER_C == IDLIB_COMPILER_C_GCC
  #define STATIC_ASSERT(CONDITION, MESSAGE) _Static_assert(CONDITION, MESSAGE)
#else
  #error("compiler not (yet) supported")
#endif

#define DEFINE(NAME, R, G, B) \
  STATIC_ASSERT(0 <= R && R <= 255, "<internal error>"); \
  STATIC_ASSERT(0 <= G && G <= 255, "<internal error>"); \
  STATIC_ASSERT(0 <= B && B <= 255, "<internal error>"); \
  idlib_color_3_u8 const idlib_colors_##NAME##_3_u8 = { .r = R, .g = G, .b = B };

#include "colors.i"

#undef DEFINE
