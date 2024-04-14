# `Shizu_Value_getMap`

*Description*
Get the `Shizu_Map*` value stored in a `Shizu_Value` value.

*Signature*
```
Shizu_Map*
Shizu_Value_getMap
  (
    Shizu_Value const* self
  );
```

*Remarks*
The behaviour of this function is undefined if `self` does not point to an initialized `Shizu_Value` value that is storing a `Shizu_Map*` value.

*Parameters*
- `self` A pointer to a `Shizu_Value` value that is storing a `Shizu_Map*` value.

*Return Value*
The `Shizu_Map*` value stored in the `Shizu_Value` pointed to by `self`.
