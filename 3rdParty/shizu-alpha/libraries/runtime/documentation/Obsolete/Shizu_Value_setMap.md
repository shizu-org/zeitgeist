# `Shizu_Value_setMap`

*Description*
Store a `Shizu_Map*` object in a `Shizu_Value` object/value.
This function can be used to initialize an uninitialized `Shizu_Value` object.

*Signature*
```
void
Shizu_Value_setMap
  (
    Shizu_Value* self,
    Shizu_Map* mapValue
  );
```

*Remarks*
The behaviour of this function is undefined if `self` does not point to a `Shizu_Value` object/value.

*Parameters*
- `self` A pointer to a `Shizu_Value` object/value to store the `Shizu_Map*` object in.
- `mapValue` The `Shizu_Map*` object to store.
