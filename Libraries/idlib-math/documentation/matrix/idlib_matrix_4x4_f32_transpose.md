# idlib_matrix_4x4_f32_zero

**Signature**
```
void
idlib_matrix_4x4_f32_transpose
  (
    idlib_matrix_4x4_f32* target,
    idlib_matrix_4x4_f32* operand
  )
```

**Description**
Transpose `operand` and assign the result to `target`.

**Parameters**
- `target` A pointer to an `idlib_matrix_4x4_f32` object. The result is assigned to that object.
- `operand` Pointer to an `idlib_matrix_4x4_f32` to be transposed.

**Remarks**
- The behavior of the function is undefined if `target` or `operand` do not point to `idlib_matrix_4x4_f32` objects.
- `operand` and `target` can all point to the same `idlib_matrix_4x4_f32` object.
