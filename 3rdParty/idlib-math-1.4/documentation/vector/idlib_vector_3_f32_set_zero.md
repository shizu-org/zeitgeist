# idlib_vector_3_f32_set_zero

**Signature**
```
void
idlib_vector_3_f32_set_zero
  (
    idlib_vector_3_f32* target
  )
```

**Description**
Assign `target` the values of the zero vector.

**Paramters**
- `target` A pointer to an `idlib_vector_3_f32` object. The result is assigned to that object.

**Remarks**
- The behavior of the function is undefined if `target` does not point to a `idlib_vector_3_f32` object.
