# `idlib_color_3_u8_set`

**Signature**
```
void
idlib_color_3_u8_set
  (
    idlib_color_3_u8 *target,
    idlib_u8 operand1,
    idlib_u8 operand2,
    idlib_u8_operand3
  )
```

**Description**
Assign `target` the specified values:
`operand1` is assigned to the *r* component,
`operand2` is assigned to the *g* component, and
`operand3` is assigned to the *b* component
of that `idlib_color_3_u8` object.

**Parameters**
- `target` A pointer to an `idlib_color_3_u8` object. The result is assigned to that object.
- `operand1` The value to assign to the *r* component.
- `operand2` The value to assign to the *g* component.
- `operand3` The value to assign to the *b* component.

**Remarks**
- The behavior of the function is undefined if `target` does not point to a `idlib_color_3_u8_set` object.