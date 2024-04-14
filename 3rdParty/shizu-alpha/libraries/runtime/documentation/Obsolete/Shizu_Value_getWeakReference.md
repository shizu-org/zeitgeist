# `Shizu_Value_getWeakReference`

*Description*
Get the `Shizu_WeakReference*` value stored in a `Shizu_Value` value.

*Signature*
```
Shizu_WeakReference*
Shizu_Value_getWeakReference
  (
    Shizu_Value const* self
  );
```

*Remarks*
The behaviour of this function is undefined if `self` does not point to an initialized `Shizu_Value` value that is storing a `Shizu_Map*` value.

*Parameters*
- `self` A pointer to a `Shizu_Value` value that is storing a `Shizu_WeakReference*` value.

*Return Value*
The `Shizu_WeakReference*` value stored in the `Shizu_Value` pointed to by `self`.
