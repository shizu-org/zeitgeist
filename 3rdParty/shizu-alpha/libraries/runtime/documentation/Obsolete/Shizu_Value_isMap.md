# `Shizu_Value_isMap`

*Description*
Get if a `Shizu_Value` holds a `Shizu_Map*` value.

*Signature*
```
bool
Shizu_Value_isMap
  (
    Shizu_Value const* self
  );
```

*Remarks*
The behaviour of this function is undefined if `self` does not point to a `Shizu_Value` value.

*Parameters*
- `self` A pointer to a `Shizu_Value` value.

*Return Value*
`true` if the `Shizu_Value` value holds a `Shizu_Map*` value. `false` otherwise.
