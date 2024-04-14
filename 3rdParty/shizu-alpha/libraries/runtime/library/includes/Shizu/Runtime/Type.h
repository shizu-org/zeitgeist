/*
  Shizu Runtime
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

#if !defined(SHIZU_RUNTIME_TYPE_H_INCLUDED)
#define SHIZU_RUNTIME_TYPE_H_INCLUDED

#include "Shizu/Runtime/Configure.h"

typedef struct Shizu_Dll Shizu_Dll;
typedef struct Shizu_State Shizu_State;
typedef struct Shizu_Object Shizu_Object;
typedef struct Shizu_Type Shizu_Type;
typedef struct Shizu_TypeDescriptor Shizu_TypeDescriptor;

/// @since 1.0
/// The type of an "onStaticInitialize" callback function.
typedef void (Shizu_OnStaticInitializeCallback)(Shizu_State* state);

/// @since 1.0
/// The type of an "onStaticUninitialize" callback function.
typedef void (Shizu_OnStaticFinalizeCallback)(Shizu_State* state);

/// @since 1.0
/// The type of an "onStaticVisit" callback function.
typedef void (Shizu_OnStaticVisitCallback)(Shizu_State* state);

/// @since 1.0
/// The type of a "onVisit" callback function.
typedef void (Shizu_OnVisitCallback)(Shizu_State* state, Shizu_Object* object);

/// @since 1.0
/// The type of a "onFinalize" callback function.
typedef void (Shizu_OnFinalizeCallback)(Shizu_State* state, Shizu_Object* object);

/// @since 1.0
/// The type of a "onTypeDestroyed" callback function.
typedef void (Shizu_OnTypeDestroyedCallback)(Shizu_State* state);

struct Shizu_TypeDescriptor {
  Shizu_OnStaticInitializeCallback* staticInitialize;
  Shizu_OnStaticFinalizeCallback* staticFinalize;
  Shizu_OnStaticVisitCallback* staticVisit;
  Shizu_OnVisitCallback* visit;
  Shizu_OnFinalizeCallback* finalize;
};

Shizu_Type*
Shizu_State_getTypeByName
  (
    Shizu_State* state,
    char const* name
  );

Shizu_Type*
Shizu_State_createType
  (
    Shizu_State* self,
    char const* name,
    Shizu_Type* parentType,
    Shizu_Dll* dll,
    Shizu_OnTypeDestroyedCallback* typeDestroyed,
    Shizu_TypeDescriptor const* descriptor
  );

#define Shizu_declareDlType(Name) \
  typedef struct Name Name; \
\
  __declspec(dllexport) Shizu_Type* \
  Name##_getType \
    ( \
      Shizu_State* state \
    );

#define Shizu_declareType(Name) \
  typedef struct Name Name; \
\
  Shizu_Type* \
  Name##_getType \
    ( \
      Shizu_State* state \
    );

#define Shizu_defineDlType(Name, ParentName) \
  static void \
  Name##_typeDestroyed \
    ( \
      Shizu_State* state \
    ); \
  \
  Shizu_Type* \
  Name##_getType \
    ( \
      Shizu_State* state \
    ); \
  \
  static void \
  Name##_typeDestroyed \
    ( \
      Shizu_State* state \
    ) \
  {/*Intentionally empty.*/} \
  \
  __declspec(dllexport) Shizu_Type* \
  Name##_getType \
    ( \
      Shizu_State* state \
    ) \
  { \
    Shizu_Type* type = Shizu_State_getTypeByName(state, #Name); \
    if (!type) { \
      Shizu_Dll* dll = Shizu_State_getDlByAdr(state, &Name##_getType); \
      type = Shizu_State_createType(state, #Name, ParentName##_getType(state), dll, &Name##_typeDestroyed, &Name##_Type); \
    } \
    return type; \
  }

// Under Windows, when we are loading DLLs at runtime then both the
// executable and the DLL link to the same static library have
// different instances of this variable.
// 
// This is why we cannot rely on g_type to detect if a type is registered or not:
// - if g_type is null:
//  - invoke "getTypeByName" to obtain the type if it exists already
//  - register and obtain the type if it does not exist
//  - register an "on type destroyed" callback for the variable
#define Shizu_defineType(Name, ParentName) \
  static void \
  Name##_typeDestroyed \
    ( \
      Shizu_State* state \
    ); \
  \
  Shizu_Type* \
  Name##_getType \
    ( \
      Shizu_State* state \
    ); \
  \
  static void \
  Name##_typeDestroyed \
    ( \
      Shizu_State* state \
    ) \
  {/*Intentionally empty.*/} \
  \
  Shizu_Type* \
  Name##_getType \
    ( \
      Shizu_State* state \
    ) \
  { \
    Shizu_Type* type = Shizu_State_getTypeByName(state, #Name); \
    if (!type) { \
      type = Shizu_State_createType(state, #Name, ParentName##_getType(state), NULL, &Name##_typeDestroyed, &Name##_Type); \
    } \
    return type; \
  }

#endif // SHIZU_RUNTIME_TYPE_H_INCLUDED
