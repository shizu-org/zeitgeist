# idlib_vector_3_f32_get_data

**signature**
```
void*
idlib_vector_3_f32_get_data
  (
    idlib_vector_3_f32* operand
  );
```

**Description**
Get a pointer to the data of an `idlib_vector_3_f32` object.

**Parameters**
- `operand` Pointer to the `idlib_vector_3_f32` object.

**Return Value**
A pointer to the data of the `idlib_vector_3_f32` object.
The pointer remains valid as long as the object remains valid and is not modified.

**Remarks**
- The behavior of the function is undefined if `operand` does not point to an `idlib_vector_3_f32` object.
