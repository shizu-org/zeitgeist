# `Shizu_Value_getString`

*Description*
Get the `Shizu_String*` value stored in a `Shizu_Value` value.

*Signature*
```
Shizu_String*
Shizu_Value_getString
  (
    Shizu_Value const* self
  );
```

*Remarks*
The behaviour of this function is undefined if `self` does not point to an initialized `Shizu_Value` value that is storing a `Shizu_String*` value.

*Parameters*
- `self` A pointer to a `Shizu_Value` value that is storing a `Shizu_String*` value.

*Return Value*
The `Shizu_String*` value stored in the `Shizu_Value` pointed to by `self`.
