# idlib_vector_3_f32_cross

**Signature**
```
bool
idlib_vector_3_f32_cross
  (
    idlib_vector_3_f32 *target,
    idlib_vector_3_f32 const* operand1,
    idlib_vector_3_f32 const* operand2
  )
```

**Description**
Compute the cross product of `operand1` with `operand2` and assign the result to `target`.

**Parameters**
- `target` A pointer to an `idlib_vector_3_f32` object. The result is assigned to that object.
- `operand1` A pointer to an `idlib_vector_3_f32` object. the object is the multiplier.
- `operand1` A pointer to an `idlib_vector_3_f32` object. the object is the multiplicand.

**Remarks**
- The behavior of the function is undefined if `operand1`, `operand2`, or `target` do not point to `idlib_vector_3_f32` objects.
- `operand1`, `operand2`, and `target` can all point to the same `idlib_vector_3_f32` object.