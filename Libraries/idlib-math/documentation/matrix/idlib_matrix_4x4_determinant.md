# idlib_matrix_4x4_f32_determinant

**Signature**
```
idlib_f32
idlib_matrix_4x4_f32_determinant
  (
    idlib_matrix_4x4_f32 const* operand
  );
```

**Description**
Compute the determinant of `operand`.

**Parameters**
- `operand` A pointer to the `idlib_matrix_4x4_f32` object.

**Return Value**
The determinant of the `idlib_matrix_4x4_f32` object pointed to by `operand`.

**Remarks**
- The behavior of the function is undefined if `operand` does not point to an `idlib_matrix_4x4_f32` objects.
