# idlib_vector_3_f32_add

**Signature**
```
void
idlib_vector_3_f32_add
  (
    idlib_vector_3_f32* target,
    idlib_vector_3_f32 const* operand1,
    idlib_vector_3_f32 const* operand2
  );
```

**Description**
Add `operand2` to `operand1` and assign the result to `target`.

*Parameters*
- `target` A pointer to the `idlib_vector_3_f32` object the result is assigned to.
- `operand1` A pointer to the `idlib_vector_3_f32` object that is the first operand (aka the augend).
- `operand2` A pointer to the `idlib_vector_3_f32` object that is the second operand (aka the addend). 


*Remarks*
- The behavior of the function is undefined if `operand1`, `operand2`, or `target` do not point to `idlib_vector_3_f32` objects.
- `operand1`, `operand2`, and `target` can all point to the same `idlib_vector_3_f32` object.
