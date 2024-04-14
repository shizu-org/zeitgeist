# `Shizu_Value_setString`

*Description*
Store a `Shizu_String*` object in a `Shizu_Value` object/value.
This function can be used to initialize an uninitialized `Shizu_Value` object.

*Signature*
```
void
Shizu_Value_setString
  (
    Shizu_Value* self,
    Shizu_String* stringValue
  );
```

*Remarks*
The behaviour of this function is undefined if `self` does not point to a `Shizu_Value` object/value.

*Parameters*
- `self` A pointer to a `Shizu_Value` object/value to store the `Shizu_Integer32` object in.
- `stringValue` The `Shizu_String*` object to store.
