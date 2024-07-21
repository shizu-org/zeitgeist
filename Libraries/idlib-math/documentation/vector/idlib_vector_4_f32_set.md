# idlib_vector_4_f32_set

**Signature**
```
void
idlib_vector_4_f32_set
  (
    idlib_vector_4_f32* target,
    idlib_f32 operand1,
    idlib_f32 operand2,
    idlib_f32 operand3,
    idlib_f32 operand4
  )
```

**Description**
Assign `target` the specified values:
`operand1` is assigned to the *x* component,
`operand2` is assigned to the *y* component,
`operand3` is assigned to the *z* component, and
`operand4` is assigned to the *w* component
of that `idlib_vector_4_f32` object.

**Parameters**
- `target` A pointer to an `idlib_vector_4_f32` object. The result is assigned to that object.
- `operand1` The value to assign to the *x* component.
- `operand2` The value to assign to the *y* component.
- `operand3` The value to assign to the *z* component.
- `operand4` The value to assign to the *w* component.

**Remarks**
- The behavior of the function is undefined if `target` does not point to an `idlib_vector_4_f32` object.
