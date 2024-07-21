# idlib_matrix_4x4_f32_subtract

**Signature**
```
void
idlib_matrix_4x4_f32_subtract
  (
    idlib_matrix_4x4_f32* target,
    idlib_matrix_4x4_f32 const* operand1,
    idlib_matrix_4x4_f32 const* operand2
  );
```

**Description**
Subtract `operand2` from `operand1` and assign the result to `target`.

**Parameters**
- `target` A pointer to the `idlib_matrix_4x4_f32` object. The result is assigned to that object.
- `operand1` A pointer to the `idlib_matrix_4x4_f32` object. The object is the minuend (aka first term).
- `operand2` A pointer to the `idlib_matrix_4x4_f32` object. The object is the subtrahend (aka second term).

**Remarks**
- The behavior of the function is undefined if `operand1`, `operand2`, or `target` do not point to `idlib_matrix_4x4_f32` objects.
- `operand1`, `operand2`, and `target` can all point to the same `idlib_matrix_4x4_f32` object.
