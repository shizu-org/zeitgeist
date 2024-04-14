# idlib_vector_3_f32_negate

**Signature**
```
void
idlib_vector_3_f32_negate
  (
    idlib_vector_3_f32 *target,
    idlib_vector_3_f32 const* operand
  )
```

**Description**
Negate `operand` and assign the result to `target`.

**Parameters**
- `target` A pointer to the `idlib_vector_3_f32` object. The result is assigned to that object.

**Remarks**
- The behavior of the function is undefined if `operand` or `target` does not point to a `idlib_vector_3_f32` object.
- `operand` and `target` can all point to the same `idlib_vector_3_f32` object.