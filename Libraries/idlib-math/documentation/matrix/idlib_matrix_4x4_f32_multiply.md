# idlib_matrix_4x4_f32_multiply

**Signature**
```
void
idlib_matrix_4x4_f32_multiply
  (
    idlib_matrix_4x4_f32* target,
    idlib_matrix_4x4_f32 const* operand1,
    idlib_matrix_4x4_f32 const* operand2
  );
```

**Description**
Multiply `operand1` and `operand2` and assign the result to `target`.

**Parameters**
- `target` A pointer to an `idlib_matrix_4x4_f32` object. The result is assigned to that object.
- `operand1` A pointer to an `idlib_matrix_4x4_f32` object. The object is the multiplier.
- `operand2` A pointer to an `idlib_matrix_4x4_f32` object. The object is the multiplicand.

**Remarks**
- The behavior of the function is undefined if `operand1`, `operand2`, or `target` do not point to `idlib_matrix_4x4_f32` objects.
- `operand1`, `operand2`, and `target` can all point to the same `idlib_matrix_4x4_f32` object.
