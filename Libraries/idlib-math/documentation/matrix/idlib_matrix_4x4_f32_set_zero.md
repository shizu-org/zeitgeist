# idlib_matrix_4x4_f32_set_zero

**Signature**
```
void
idlib_matrix_4x4_f32_set_zero
  (
    idlib_matrix_4x4_f32* target
  )
```

**Description**
Assign `target` the values of the zero matrix.

**Parameters**
- `target` A pointer to an `idlib_matrix_4x4_f32` object. The result is assigned to that object.

**Remarks**
- The behavior of the function is undefined if `target` does not point to an `idlib_matrix_4x4_f32` object.
