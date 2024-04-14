# idlib_vector_3_f32_subtract

**Signature**
```
void
idlib_vector_3_f32_subtract
  (
    idlib_vector_3_f32* target,
    idlib_vector_3_f32 const* operand1,
    idlib_vector_3_f32 const* operand2
  );
```

**Description**
Subtract `operand2` from `operand1` and assign the result to `target`.

**Parameters**
- `target` Pointer to the `idlib_vector_3_f32` object the result is assigned to.
- `operand1` Pointer to the first `idlib_vector_3_f32` object which is the first operand (aka the minuend).
- `operand2` Pointer to the second `idlib_vector_3_f32` object which is the second operand (aka the subtrahend). 

**Remarks**
- The behavior of the function is undefined if `operand1`, `operand2`, or `target` do not point to `idlib_vector_3_f32` objects.
- `operand1`, `operand2`, and `target` can all point to the same `idlib_vector_3_f32` object.
