# `Shizu_Value_isString`

*Description*
Get if a `Shizu_Value` holds a `Shizu_String*` value.

*Signature*
```
bool
Shizu_Value_isString
  (
    Shizu_Value const* self
  );
```

*Remarks*
The behaviour of this function is undefined if `self` does not point to a `Shizu_Value` value.

*Parameters*
- `self` A pointer to a `Shizu_Value` value.

*Return Value*
`true` if the `Shizu_Value` value holds a `Shizu_String*` value. `false` otherwise.
