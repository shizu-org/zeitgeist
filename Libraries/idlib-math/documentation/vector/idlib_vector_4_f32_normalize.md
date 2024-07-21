# idlib_vector_4_f32_normalize

**Signature**
```
bool
idlib_vector_4_f32_normalize
  (
    idlib_vector_4_f32 *target,
    idlib_vector_4_f32 const* operand
  )
```

**Description**
Compute the normalized vector of `operand` and assign the result to `target`.
If the length of `operand` is `0` then `target` is assigned `operand`.
Otherwise `target` is assigned the normalized `operand`.

**Parameters**
- `target` A pointer to an `idlib_vector_4_f32` object. The result is assigned to that object.
- `operand` A pointer to an `idlib_vector_4_f32` object. The object to be normalized.

**Return Value**
`true` if the length of `operand` was not `0`, `false` otherwise.

**Remarks**
- The behavior of the function is undefined if `operand` or `target` do not point to `idlib_vector_4_f32` objects.
- `operand` and `target` can all point to the same `idlib_vector_4_f32` object.
