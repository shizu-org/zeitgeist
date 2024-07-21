# idlib_vector_2_f32_set

**Signature**
```
void
idlib_vector_2_f32_set
  (
    idlib_vector_3_f32* target,
    idlib_f32 operand1,
    idlib_f32 operand2
  )
```

**Description**
Assign `target` the specified values:
`operand1` is assigned to the *x* component and
`operand2` is assigned to the *y* component
of that `idlib_vector_2_f32` object.

**Parameters**
- `target` A pointer to an `idlib_vector_2_f32` object. The result is assigned to that object.
- `operand1` The value to assign to the *x* component.
- `operand2` The value to assign to the *y* component.

**Remarks**
- The behavior of the function is undefined if `target` does not point to an `idlib_vector_2_f32` object.
