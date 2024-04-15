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

#define SHIZU_RUNTIME_PRIVATE (1)
#include "Shizu/Runtime/State.h"

#include "Shizu/Runtime/State1.h"
#include "Shizu/Runtime/Configure.h"
#include "Shizu/Runtime/Gc.private.h"
#include "Shizu/Runtime/Locks.private.h"
#include "Shizu/Runtime/Stack.private.h"
#include "Shizu/Runtime/Objects/WeakReference.private.h"
#include "Shizu/Runtime/Type.private.h"

// malloc, free
#include <malloc.h>

// fprintf, stderr
#include <stdio.h>

// exit, EXIT_FAILURE
#include <stdlib.h>

// strcmp
#include <string.h>

#include "idlib/process.h"

typedef struct NamedStorageNode NamedStorageNode;

struct NamedStorageNode {
  NamedStorageNode* next;
  char* name;
  void* p;
  size_t n;
};

/// @brief Replacement for C global variables / singletons.
typedef struct NamedStorageService {
  NamedStorageNode* nodes;
} NamedStorageService;

int
NamedStorageService_initialize
  (
    NamedStorageService* self
  )
{
  self->nodes = NULL;
  return 0;
}

int
NamedStorageService_uninitialize
  (
    NamedStorageService* self
  )
{
  if (self->nodes) {
    fprintf(stderr, "%s:%d: warning: storage service still contains storage nodes\n", __FILE__, __LINE__);
  }
  return 0;
}

int
NamedStorageService_allocate
  (
    NamedStorageService* self,
    char const* name,
    size_t n
  )
{
  NamedStorageNode* node = NULL;

  node = self->nodes;
  while (node) {
    if (!strcmp(node->name, name)) {
      return 1;
    }
    node = node->next;
  }

  node = malloc(sizeof(NamedStorageNode));
  if (!node) {
    return 1;
  }
  node->name = strdup(name);
  if (!node->name) {
    free(node);
    return 1;
  }
  node->p = malloc(n > 0 ? n : 1);
  if (!node->p) {
    free(node->name);
    free(node);
    return 1;
  }
  node->n = n;
  node->next = self->nodes;
  self->nodes = node;
  return 0;
}

int
NamedStorageService_deallocate
  (
    NamedStorageService* self,
    char const* name
  )
{
  NamedStorageNode** previous = &self->nodes;
  NamedStorageNode* current = self->nodes;
  while (current) {
    if (!strcmp(current->name, name)) {
      NamedStorageNode* node = current;
      *previous = current->next;
      current = current->next;
      free(node->name);
      node->name = NULL;
      free(node->p);
      node->p = NULL;
      free(node);
    } else {
      previous = &current->next;
      current = current->next;
    }
  }
  return 0;
}

int
NamedStorageService_get
  (
    NamedStorageService* self,
    char const* name,
    void** p
  )
{
  NamedStorageNode* current = self->nodes;
  while (current) {
    if (!strcmp(current->name, name)) {
      *p = current->p;
      return 0;
    } else {
      current = current->next;
    }
  }
  return 1;
}




void
Shizu_debugAssertionFailed
  (
    char const* file,
    int line,
    char const* expression
  )
{
  fprintf(stderr, "%s:%d: debug assertion `%s` failed\n", file, line, expression);
  exit(EXIT_FAILURE);
}




Shizu_Boolean
Shizu_Value_getBoolean
  (
    Shizu_Value const* self
  )
{
  Shizu_debugAssert(Shizu_Value_isBoolean(self));
  return self->booleanValue;
}

bool
Shizu_Value_isBoolean
  (
    Shizu_Value const* self
  )
{
  Shizu_debugAssert(NULL != self);
  return Shizu_Value_Tag_Boolean == self->tag;
}

void
Shizu_Value_setBoolean
  (
    Shizu_Value* self,
    Shizu_Boolean booleanValue
  )
{
  Shizu_debugAssert(NULL != self);
  self->booleanValue = booleanValue;
  self->tag = Shizu_Value_Tag_Boolean;
}

Shizu_Reference(Shizu_CxxFunction)
Shizu_Value_getCxxFunction
  (
    Shizu_Value const* self
  )
{
  Shizu_debugAssert(Shizu_Value_isCxxFunction(self));
  return self->cxxFunctionValue;
}

bool
Shizu_Value_isCxxFunction
  (
    Shizu_Value const* self
  )
{
  Shizu_debugAssert(NULL != self);
  return Shizu_Value_Tag_CxxFunction == self->tag;
}

void
Shizu_Value_setCxxFunction
  (
    Shizu_Value* self,
    Shizu_Reference(Shizu_CxxFunction) cxxFunctionValue
  )
{
  Shizu_debugAssert(NULL != self);
  self->cxxFunctionValue = cxxFunctionValue;
  self->tag = Shizu_Value_Tag_CxxFunction;
}

Shizu_Float32
Shizu_Value_getFloat32
  (
    Shizu_Value const* self
  )
{
  Shizu_debugAssert(Shizu_Value_isFloat32(self));
  return self->float32Value;
}

bool
Shizu_Value_isFloat32
  (
    Shizu_Value const* self
  )
{
  Shizu_debugAssert(NULL != self);
  return Shizu_Value_Tag_Float32 == self->tag;
}

void
Shizu_Value_setFloat32
  (
    Shizu_Value* self,
    Shizu_Float32 float32Value
  )
{
  Shizu_debugAssert(NULL != self);
  self->float32Value = float32Value;
  self->tag = Shizu_Value_Tag_Float32;
}

Shizu_Integer32
Shizu_Value_getInteger32
  (
    Shizu_Value const* self
  )
{
  Shizu_debugAssert(Shizu_Value_isInteger32(self));
  return self->integer32Value;
}

bool
Shizu_Value_isInteger32
  (
    Shizu_Value const* self
  )
{
  Shizu_debugAssert(NULL != self);
  return Shizu_Value_Tag_Integer32 == self->tag;
}

void
Shizu_Value_setInteger32
  (
    Shizu_Value* self,
    Shizu_Integer32 integer32Value
  )
{
  Shizu_debugAssert(NULL != self);
  self->integer32Value = integer32Value;
  self->tag = Shizu_Value_Tag_Integer32;
}

Shizu_Reference(Shizu_Object)
Shizu_Value_getObject
  (
    Shizu_Value const* self
  )
{
  Shizu_debugAssert(Shizu_Value_isObject(self));
  return self->objectValue;
}

bool
Shizu_Value_isObject
  (
    Shizu_Value const* self
  )
{
  Shizu_debugAssert(NULL != self);
  return Shizu_Value_Tag_Object == self->tag;
}

void
Shizu_Value_setObject
  (
    Shizu_Value* self,
    Shizu_Reference(Shizu_Object) objectValue
  )
{
  Shizu_debugAssert(NULL != self);
  self->objectValue = objectValue;
  self->tag = Shizu_Value_Tag_Object;
}

Shizu_Void
Shizu_Value_getVoid
  (
    Shizu_Value const* self
  )
{
  Shizu_debugAssert(Shizu_Value_isVoid(self));
  return self->voidValue;
}

bool
Shizu_Value_isVoid
  (
    Shizu_Value const* self
  )
{
  Shizu_debugAssert(NULL != self);
  return Shizu_Value_Tag_Void == self->tag;
}

void
Shizu_Value_setVoid
  (
    Shizu_Value* self,
    Shizu_Void voidValue
  )
{
  Shizu_debugAssert(NULL != self);
  self->voidValue = voidValue;
  self->tag = Shizu_Value_Tag_Void;
}

struct Shizu_State {
  int referenceCount;
  Shizu_State1* state1;
  NamedStorageService namedStorageService;
  Shizu_Gc* gc;
  Shizu_Locks* locks;
  Shizu_Stack* stack;
  struct {
    Shizu_Type** elements;
    size_t size;
    size_t capacity;
  } types;
};

static Shizu_State* g_singleton = NULL;

#define NAME "Shizu.State.Singleton"

int
Shizu_State_create
  (
    Shizu_State** RETURN
  )
{
  if (!RETURN) {
    return 1;
  }
  if (!g_singleton) {
    Shizu_State* self = malloc(sizeof(Shizu_State));
    if (!self) {
      return 1;
    }
    if (Shizu_State1_acquire(&self->state1)) {
      free(self);
      self = NULL;
      return 1;
    }
    if (NamedStorageService_initialize(&self->namedStorageService)) {
      Shizu_State1_relinquish(self->state1);
      self->state1 = NULL;
      free(self);
      self = NULL;
      return 1;
    }
    self->gc = NULL;
    self->locks = NULL;
    self->stack = NULL;

    Shizu_JumpTarget jumpTarget;
    //
    Shizu_State_pushJumpTarget(self, &jumpTarget);
    if (!setjmp(jumpTarget.environment)) {
      self->gc = Shizu_Gc_startup(self);
      Shizu_State_popJumpTarget(self);
    } else {
      Shizu_State_popJumpTarget(self);
      NamedStorageService_uninitialize(&self->namedStorageService);
      Shizu_State1_relinquish(self->state1);
      self->state1 = NULL;
      free(self);
      self = NULL;
      return 1;
    }
    //
    Shizu_State_pushJumpTarget(self, &jumpTarget);
    if (!setjmp(jumpTarget.environment)) {
      self->locks = Shizu_Locks_startup(self);
      Shizu_State_popJumpTarget(self);
    } else {
      Shizu_State_popJumpTarget(self);
      Shizu_Gc_shutdown(self, self->gc);
      self->gc = NULL;
      NamedStorageService_uninitialize(&self->namedStorageService);
      Shizu_State1_relinquish(self->state1);
      self->state1 = NULL;
      free(self);
      self = NULL;
      return 1;
    }
    //
    Shizu_State_pushJumpTarget(self, &jumpTarget);
    if (!setjmp(jumpTarget.environment)) {
      self->stack = Shizu_Stack_startup(self);
      Shizu_State_popJumpTarget(self);
    } else {
      Shizu_State_popJumpTarget(self);
      Shizu_Locks_shutdown(self, self->locks);
      self->locks = NULL;
      Shizu_Gc_shutdown(self, self->gc);
      self->gc = NULL;
      NamedStorageService_uninitialize(&self->namedStorageService);
      Shizu_State1_relinquish(self->state1);
      self->state1 = NULL;
      free(self);
      self = NULL;
      return 1;
    }
    //
    Shizu_State_pushJumpTarget(self, &jumpTarget);
    if (!setjmp(jumpTarget.environment)) {
      Shizu_Types_initialize(self);
      Shizu_State_popJumpTarget(self);
    } else {
      Shizu_State_popJumpTarget(self);
      Shizu_Stack_shutdown(self, self->stack);
      self->stack = NULL;
      Shizu_Locks_shutdown(self, self->locks);
      self->locks = NULL;
      Shizu_Gc_shutdown(self, self->gc);
      self->gc = NULL;
      NamedStorageService_uninitialize(&self->namedStorageService);
      Shizu_State1_relinquish(self->state1);
      self->state1 = NULL;
      free(self);
      self = NULL;
      return 1;
    }
    self->referenceCount = 0;
    g_singleton = self;
  }
  g_singleton->referenceCount++;
  *RETURN = g_singleton;
  return 0;
}

static size_t
Shizu_Type_getChildCount
  (
    Shizu_Type* self
  )
{
  size_t count = 0;
  while (count < self->children.capacity && self->children.elements[count]) {
    count++;
  }
  return count;
}

// The type must already be removed from the state.types hash table.
// Furthermore, the type must have zero children.
static void
Shizu_Type_destroy
  (
    Shizu_State* self,
    Shizu_Type* type
  )
{
  // Invoke the finalizer.
  if (Shizu_TypeFlags_StaticallyInitialized == (Shizu_TypeFlags_StaticallyInitialized & type->flags) && type->descriptor->staticFinalize) {
    type->descriptor->staticFinalize(self);
  }
  // Remove this type from the array of references to child types of its parent type.
  if (type->parentType) {
    size_t index1 = (size_t)-1;
    for (size_t i = 0, n = type->parentType->children.capacity; i < n; ++i) {
      if (type->parentType->children.elements[i] == type) {
        index1 = i;
        break;
      }
    }
    size_t index2 = (size_t)-1;
    for (size_t i = type->parentType->children.capacity; i > 0; --i) {
      if (type->parentType->children.elements[i - 1]) {
        index2 = i - 1;
        break;
      }
    }
    // if (index1 != index2) {
    type->parentType->children.elements[index1] = type->parentType->children.elements[index2];
    type->parentType->children.elements[index2] = NULL;
    // }
  }
  // Deallocate array of references to children.
  free(type->children.elements);
  type->children.elements = NULL;
  // Deallocate the name.
  free(type->name.bytes);
  type->name.bytes = NULL;
  //
  if (type->dl) {
    Shizu_Dl_unref(self, type->dl);
    type->dl = NULL;
  }
  // Deallocate the value.
  free(type);
}

void
Shizu_Types_initialize
  (
    Shizu_State* self
  )
{
  self->types.elements = malloc(sizeof(Shizu_Type) * 8);
  if (!self->types.elements) {
    Shizu_State_setError(self, 1);
    Shizu_State_jump(self);
  }
  for (size_t i = 0, n = 8; i < n; ++i) {
    self->types.elements[i] = NULL;
  }
  self->types.size = 0;
  self->types.capacity = 8;
}

void
Shizu_Types_uninitialize
  (
    Shizu_State* self
  )
{
  while (self->types.size) {
    // Worst-case is k^n where n is the number of types and k is the capacity.
    // Do *no*u use this in performance critical code.
    for (size_t i = 0, n = self->types.capacity; i < n; ++i) {
      Shizu_Type** previous = &(self->types.elements[i]);
      Shizu_Type* current = self->types.elements[i];
      while (current) {
        if (!Shizu_Type_getChildCount(current)) {
          Shizu_Type* type = current;
          Shizu_Types_ensureDispatchUninitialized(self, type);
          *previous = current->next;
          current = current->next;
          self->types.size--;
          if (type->typeDestroyed) {
            type->typeDestroyed(self);
          }
          Shizu_Type_destroy(self, type);
        } else {
          previous = &current->next;
          current = current->next;
        }
      }
    }
  }
}

int
Shizu_State_destroy
  (
    Shizu_State* self
  )
{
  if (!self) {
    return 1;
  }
  if (0 == --self->referenceCount) {
    Shizu_Gc_run(self);
    Shizu_Stack_shutdown(self, self->stack);
    self->stack = NULL;
    Shizu_Locks_shutdown(self, self->locks);
    self->locks = NULL;
    Shizu_Gc_shutdown(self, self->gc);
    self->gc = NULL;
    Shizu_Types_uninitialize(self);
    NamedStorageService_uninitialize(&self->namedStorageService);
    Shizu_State1_relinquish(self->state1);
    self->state1 = NULL;
    g_singleton = NULL;
    free(self);
  }
  return 0;
}

void
Shizu_State_pushJumpTarget
  (
    Shizu_State* self,
    Shizu_JumpTarget* jumpTarget
  )
{
  Shizu_debugAssert(NULL != self);
  Shizu_State1_pushJumpTarget(self->state1, jumpTarget);
}

void
Shizu_State_popJumpTarget
  (
    Shizu_State* self
  )
{
  Shizu_debugAssert(NULL != self);
  Shizu_State1_popJumpTarget(self->state1);
}

Shizu_NoReturn() void
Shizu_State_jump
  (
    Shizu_State* self
  )
{
  Shizu_debugAssert(NULL != self);
  Shizu_State1_jump(self->state1);
}

void
Shizu_State_setError
  (
    Shizu_State* self,
    int error
  )
{
  Shizu_debugAssert(NULL != self);
  Shizu_State1_setError(self->state1, error);
}

int
Shizu_State_getError
  (
    Shizu_State* self
  )
{
  Shizu_debugAssert(NULL != self);
  return Shizu_State1_getError(self->state1);
}

void
Shizu_State_setProcessExitRequested
  (
    Shizu_State* self,
    bool processExitRequested
  )
{
  Shizu_debugAssert(NULL != self);
  Shizu_State1_setProcessExitRequested(self->state1, processExitRequested);
}

bool
Shizu_State_getProcessExitRequested
  (
    Shizu_State* self
  )
{
  Shizu_debugAssert(NULL != self);
  return Shizu_State1_getProcessExitRequested(self->state1);
}

Shizu_Type*
Shizu_State_getTypeByName
  (
    Shizu_State* self,
    char const* name
  )
{
  size_t numberOfBytes = strlen(name) + 1;
  size_t hashValue = numberOfBytes;
  for (size_t i = 0, n = numberOfBytes; i < n; ++i) {
    hashValue = hashValue * 37 + (size_t)name[i];
  }
  size_t hashIndex = hashValue % self->types.capacity;
  for (Shizu_Type* type = self->types.elements[hashIndex]; NULL != type; type = type->next) {
    if (type->name.hashValue == hashValue && type->name.numberOfBytes == numberOfBytes) {
      if (!strcmp(type->name.bytes, name)) {
        return type;
      }
    }
  }
  return NULL;
}

void
Shizu_Types_ensureDispatchUninitialized
  (
    Shizu_State* state,
    Shizu_Type* type
  )
{
  if (0 == (Shizu_TypeFlags_DispatchInitialized & type->flags)) {
    return;
  }
  if (type->descriptor->dispatchUninitialize) {
    type->descriptor->dispatchUninitialize(state, type->dispatch);
  }
  free(type->dispatch);
  type->dispatch = NULL;
  type->flags = ~Shizu_TypeFlags_DispatchInitialized & type->flags;
}

void
Shizu_Types_ensureDispatchInitialized
  (
    Shizu_State* state,
    Shizu_Type* type
  )
{
  if (Shizu_TypeFlags_DispatchInitialized == (Shizu_TypeFlags_DispatchInitialized & type->flags)) {
    return;
  }
  if (type->parentType) {
    if (type->descriptor->dispatchSize < type->parentType->descriptor->dispatchSize) {
      Shizu_State_setError(state, 1);
      Shizu_State_jump(state);
    }
    Shizu_Types_ensureDispatchInitialized(state, type->parentType);
  }
  if (type->descriptor->dispatchSize < sizeof(Shizu_Object_Dispatch)) {
    Shizu_State_setError(state, 1);
    Shizu_State_jump(state);
  }
  type->dispatch = malloc(type->descriptor->dispatchSize);
  if (!type->dispatch) {
    Shizu_State_setError(state, 1);
    Shizu_State_jump(state);
  }
  memset(type->dispatch, 0, type->descriptor->dispatchSize);
  if (type->parentType) {
    memcpy(type->dispatch, type->parentType->dispatch, type->parentType->descriptor->dispatchSize);
  }
  if (type->descriptor->dispatchInitialize) {
    Shizu_JumpTarget jumpTarget;
    Shizu_State_pushJumpTarget(state, &jumpTarget);
    if (!setjmp(jumpTarget.environment)) {
      type->descriptor->dispatchInitialize(state, type->dispatch);
      Shizu_State_popJumpTarget(state);
    } else {
      Shizu_State_popJumpTarget(state);
      free(type->dispatch);
      type->dispatch = NULL;
      Shizu_State_jump(state);
    }
  }
  type->flags |= Shizu_TypeFlags_DispatchInitialized;
}

Shizu_Type*
Shizu_State_createType
  (
    Shizu_State* self,
    char const* name,
    Shizu_Type* parentType,
    Shizu_Dl* dl,
    Shizu_OnTypeDestroyedCallback* typeDestroyed,
    Shizu_TypeDescriptor const* typeDescriptor
  )
{
  size_t numberOfBytes = strlen(name) + 1;
  size_t hashValue = numberOfBytes;
  for (size_t i = 0, n = numberOfBytes; i < n; ++i) {
    hashValue = hashValue * 37 + (size_t)name[i];
  }
  size_t hashIndex = hashValue % self->types.capacity;
  for (Shizu_Type* type = self->types.elements[hashIndex]; NULL != type; type = type->next) {
    if (type->name.hashValue == hashValue && type->name.numberOfBytes == numberOfBytes) {
      if (!strcmp(type->name.bytes, name)) {
        fprintf(stderr, "%s:%d: a type of name `%s` was already registered\n", __FILE__, __LINE__, name);
        Shizu_State_setError(self, 1);
        Shizu_State_jump(self);
      }
    }
  }
  Shizu_Type* type = malloc(sizeof(Shizu_Type));
  if (!type) {
    fprintf(stderr, "%s:%d: allocation of `%zu` Bytes failed\n", __FILE__, __LINE__, sizeof(Shizu_Type));
    Shizu_State_setError(self, 1);
    Shizu_State_jump(self);
  }
  type->name.bytes = malloc(numberOfBytes);
  if (!type->name.bytes) {
    free(type);
    type = NULL;
    Shizu_State_setError(self, 1);
    Shizu_State_jump(self);
  }
  memcpy(type->name.bytes, name, numberOfBytes);
  type->flags = 0;
  type->dispatch = NULL;
  type->parentType = parentType;
  type->descriptor = typeDescriptor;
  type->name.hashValue = hashValue;
  type->name.numberOfBytes = numberOfBytes;
  type->next = NULL;
  type->typeDestroyed = typeDestroyed;
  type->dl = dl;
  // Allocate array for references to children.
  type->children.capacity = 1;
  type->children.elements = malloc(sizeof(Shizu_Type*) * type->children.capacity);
  if (!type->children.elements) {
    fprintf(stderr, "%s:%d: allocation of `%zu` Bytes failed\n", __FILE__, __LINE__, sizeof(Shizu_Type*) * type->children.capacity);
    free(type->name.bytes);
    type->name.bytes = NULL;
    free(type);
    Shizu_State_setError(self, 1);
    Shizu_State_jump(self);
  }
  for (size_t i = 0, n = type->children.capacity; i < n; ++i) {
    type->children.elements[i] = NULL;
  }
  // Add this type to the array of references to children of its parent type.
  if (parentType) {
    size_t n = Shizu_Type_getChildCount(parentType);
    if (parentType->children.capacity == n) {
      if (parentType->children.capacity == 65536) {
        fprintf(stderr, "%s:%d: unable to add `%s` as child type of type `%s`.\n", __FILE__, __LINE__, name, parentType->name.bytes);
        free(type->children.elements);
        type->children.elements = NULL;
        free(type->name.bytes);
        type->name.bytes = NULL;
        free(type);
        Shizu_State_setError(self, 1);
        Shizu_State_jump(self);
      }
      size_t newCapacity = parentType->children.capacity * 2;
      Shizu_Type** newElements = realloc(parentType->children.elements, sizeof(Shizu_Type*) * newCapacity);
      if (!newElements) {
        fprintf(stderr, "%s:%d: allocation of `%zu` Bytes failed\n", __FILE__, __LINE__, sizeof(Shizu_Type*) * newCapacity);
        free(type->children.elements);
        type->children.elements = NULL;
        free(type->name.bytes);
        type->name.bytes = NULL;
        free(type);
        Shizu_State_setError(self, 1);
        Shizu_State_jump(self);
      }
      for (size_t i = n; i < newCapacity; ++i) {
        newElements[i] = NULL;
      }
      parentType->children.capacity = newCapacity;
      parentType->children.elements = newElements;
    }
    parentType->children.elements[n] = type;
  }

  type->next = self->types.elements[hashIndex];
  self->types.elements[hashIndex] = type;
  self->types.size++;

  Shizu_JumpTarget jumpTarget;
  Shizu_State_pushJumpTarget(self, &jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    if (type->descriptor->staticInitialize) {
      type->descriptor->staticInitialize(self);
    }
    type->flags |= Shizu_TypeFlags_StaticallyInitialized;
    Shizu_Types_ensureDispatchInitialized(self, type);
  }
  Shizu_State_popJumpTarget(self);
  return type;
}

Shizu_Gc*
Shizu_State_getGc
  (
    Shizu_State* self
  )
{
  return self->gc;
}

Shizu_Locks*
Shizu_State_getLocks
  (
    Shizu_State* self
  )
{ return self->locks;}

Shizu_Stack*
Shizu_State_getStack
  (
    Shizu_State* self
  )
{ return self->stack; }

int
Shizu_State_allocateNamedMemory
  (
    Shizu_State* state,
    char const* name,
    size_t n
  )
{ return NamedStorageService_allocate(&state->namedStorageService, name, n); }

int
Shizu_State_deallocateNamedMemory
  (
    Shizu_State* state,
    char const* name
  )
{ return NamedStorageService_deallocate(&state->namedStorageService, name); }

int
Shizu_State_getNamedMemory
  (
    Shizu_State* state,
    char const* name,
    void** p
  )
{ return NamedStorageService_get(&state->namedStorageService, name, p); }

Shizu_Dl*
Shizu_State_getOrLoadDl
  (
    Shizu_State* state,
    char const* path,
    bool load
  )
{ return Shizu_State1_getOrLoadDl(state->state1, path, load); }

Shizu_Dl*
Shizu_State_getDlByName
  (
    Shizu_State* state,
    char const* name
  )
{ return Shizu_State1_getDlByName(state->state1, name); }

Shizu_Dl*
Shizu_State_getDlByAdr
  (
    Shizu_State* state,
    void *p
  )
{ return Shizu_State1_getDlByAdr(state->state1, p); }

void
Shizu_Dl_ref
  (
    Shizu_State* state,
    Shizu_Dl* dl
  )
{ Shizu_State1_refDl(state->state1, dl); }

void
Shizu_Dl_unref
  (
    Shizu_State* state,
    Shizu_Dl* dl
  )
{ Shizu_State1_unrefDl(state->state1, dl); }

void*
Shizu_Dl_getSymbol
  (
    Shizu_State* state,
    Shizu_Dl* dl,
    char const* name
  )
{ return Shizu_State1_getDlSymbol(state->state1, dl, name); }
