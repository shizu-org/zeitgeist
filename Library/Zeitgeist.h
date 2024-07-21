// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#if !defined(ZEITGEIST_H_INCLUDED)
#define ZEITGEIST_H_INCLUDED

// malloc, free
#include <malloc.h>

// fprintf, stderr
#include <stdio.h>

// exit, EXIT_FAILURE
#include <stdlib.h>

#include "Shizu/Runtime/Include.h"

#if defined(Shizu_WarnUnusedReturnValue)
  #error("Shizu_WarnUnusedReturnValue() already defined")
#endif
#if Shizu_Configuration_CompilerC_Gcc == Shizu_Configuration_CompilerC || \
    Shizu_Configuration_CompilerC_Clang == Shizu_Configuration_CompilerC
  #define Shizu_WarnUnusedReturnValue() __attribute__((warn_unused_result))
#elif Shizu_Configuration_CompilerC_Msvc == Shizu_Configuration_CompilerC
  #include <sal.h>
  #define Shizu_WarnUnusedReturnValue() _Check_return_
#else
  #define Shizu_WarnUnusedReturnValue()
#endif

#endif // ZEITGEIST_H_INCLUDED
