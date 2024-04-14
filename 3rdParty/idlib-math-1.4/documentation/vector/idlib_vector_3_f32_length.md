# idlib_vector_3_f32_length

**Signature**
```
idlib_f32
idlib_vector_3_f32_length
  (
    idlib_vector_3_f32 const* operand
  );
```

**Description**
Compute the length of `operand`.

**Parameters**
- `operand` A pointer to the `idlib_vector_3_f32` object.

**Return Value**
The length of the `idlib_vector_3_f32` object pointed to by `operand`.

**Remarks**
- The behavior of the function is undefined if `operand` does not point to `idlib_vector_3_f32` objects.
