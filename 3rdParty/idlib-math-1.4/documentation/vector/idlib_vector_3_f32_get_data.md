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
- The behavior of the function is undefined if `operand1`, `operand2`, or `target` do not point to `idlib_matrix_4x4_f32` objects.
- `operand1`, `operand2`, and `target` can all point to the same `idlib_matrix_4x4_f32` object.
