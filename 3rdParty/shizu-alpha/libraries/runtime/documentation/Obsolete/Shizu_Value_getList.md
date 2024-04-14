# `Shizu_Value_getList`

*Description*
Get the `Shizu_List*` value stored in a `Shizu_Value` value.

*Signature*
```
Shizu_List*
Shizu_Value_getList
  (
    Shizu_Value const* self
  );
```

*Remarks*
The behaviour of this function is undefined if `self` does not point to an initialized `Shizu_Value` value that is storing a `Shizu_List*` value.

*Parameters*
- `self` A pointer to a `Shizu_Value` value that is storing a `Shizu_List*` value.

*Return Value*
The `Shizu_List*` value stored in the `Shizu_Value` pointed to by `self`.
