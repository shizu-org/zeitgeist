# `Shizu_Value_setList`

*Description*
Store a `Shizu_List*` object in a `Shizu_Value` object/value.
This function can be used to initialize an uninitialized `Shizu_Value` object.

*Signature*
```
void
Shizu_Value_setList
  (
    Shizu_Value* self,
    Shizu_List* listValue
  );
```

*Remarks*
The behaviour of this function is undefined if `self` does not point to a `Shizu_Value` object.

*Parameters*
- `self` A pointer to a `Shizu_Value` object/value to store the `Shizu_List*` object in.
- `listValue` The `Shizu_List*` object to store.
