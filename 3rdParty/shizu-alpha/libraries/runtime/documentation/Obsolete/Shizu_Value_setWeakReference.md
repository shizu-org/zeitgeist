# `Shizu_Value_setWeakReference`

*Description*
Store a `Shizu_WeakReference*` object in a `Shizu_Value` object/value.
This function can be used to initialize an uninitialized `Shizu_Value` object.

*Signature*
```
void
Shizu_Value_setWeakReference
  (
    Shizu_Value* self,
    Shizu_WeakReference* weakReferenceValue
  );
```

*Remarks*
The behaviour of this function is undefined if `self` does not point to a `Shizu_Value` object/value.

*Parameters*
- `self` A pointer to a `Shizu_Value` object/value to store the `Shizu_WeakReference*` object in.
- `weakReferenceValue` The `Shizu_WeakReference*` value to store.
