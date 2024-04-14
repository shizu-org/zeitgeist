// Copyright (c) 2024 Michael Heilmann. All rights reserved.

#if !defined(ZEITGEIST_OBJECT_H_INCLUDED)
#define ZEITGEIST_OBJECT_H_INCLUDED

#include <stddef.h>
#include "Zeitgeist/Gc.h"
#include "Zeitgeist/Value.h"

typedef struct Zeitgeist_ObjectType Zeitgeist_ObjectType;

struct Zeitgeist_ObjectType {
	
	char const* name;
	
	Zeitgeist_ObjectType const* parentType;
	
	/**
   * @brief Pointer to the "destruct" function or the null pointer.
	 * @param state A pointer to the state.
	 * @param self A pointer to the object to destruct.
	 */
	void (*destruct)(Zeitgeist_State* state, Zeitgeist_Object* self);

	/**
	 * @brief Pointer to the "visit" function or the null pointer.
	 * @param state A pointer to the state.
	 * @param self A pointer to the object to destruct.
	 */
	void (*visit)(Zeitgeist_State* state, Zeitgeist_Object* self);

};

#define Zeitgeist_ObjectType_Declare(name) \
	extern Zeitgeist_ObjectType const g_##name##_Type; \
	typedef struct name name;

extern Zeitgeist_ObjectType const g_Zeitgeist_Object_Type;

struct Zeitgeist_Object {
	Zeitgeist_Gc_Object _parent;
	/** For being added to the "gray" list during the "mark" phase of the garbage collector. */
	Zeitgeist_Gc_Object* gclist;
	/*
	 * A pointer to the Zeitgeist_ObjectType of this object.
	 * After each (successful) call to a constructor of type, this pointer is set to the Zeitgeist_ObjectType object of that type.
	 */
	Zeitgeist_ObjectType const* type;
	void (*visit)(Zeitgeist_State* state, Zeitgeist_Object* self);
	void (*finalize)(Zeitgeist_State* state, Zeitgeist_Object* self);
};

/**
 * @since 0.1
 * @brief Allocate an object of the specified size.
 * The object is managed by the GC.
 * @undefined size is smaller than sizeof(Zeitgeist_Object).
 * @undefined state does not point to a Zeitgeist_State object.
 * @undefined finalize is not null and does not point to a finalize function.
 * @undefined visit is not null and does not point to a visit function.
 * @warning
 * Specified finalize or visit callbacks will be invoked regardless of if the fields of the object are properly initialized.
 * It is recommended not to set those via this function.
 */
void*
Zeitgeist_allocateObject
	(
		Zeitgeist_State* state,
		size_t size,
		void (*finalize)(Zeitgeist_State*, Zeitgeist_Object*),
		void (*visit)(Zeitgeist_State*, Zeitgeist_Object*)
	);

void
Zeitgeist_Object_visit
	(
		Zeitgeist_State* state,
		Zeitgeist_Object* foreignObject
	);

void
Zeitgeist_Object_finalize
	(
		Zeitgeist_State* state,
		Zeitgeist_Object* foreignObject
	);

#endif // ZEITGEIST_OBJECT_H_INCLUDED
