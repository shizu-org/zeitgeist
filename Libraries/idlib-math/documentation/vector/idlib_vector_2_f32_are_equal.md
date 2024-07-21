# idlib_vector_2_f32_are_equal

**Signature**
```
bool
idlib_vector_2_f32_are_equal
  (
    idlib_vector_2_f32 const* operand1,
    idlib_vector_2_f32 const* operand2
  )
```

**Description**
Get if `operand1` and `operand2` are equal.

**Parameters**
- `operand1` A pointer to an `idlib_vector_2_f32` object. The object is the first operand.
- `operand2` A pointer to an `idlib_vector_2_f32` object. The object is the second operand.

**Remarks**
- The behavior of the function is undefined if `operand1` or `operand2` does not point to a `idlib_vector_2_f32` object.
