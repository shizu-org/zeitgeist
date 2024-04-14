# idlib_matrix_4x4_f32_set_translate

**Signature**
```
void
idlib_matrix_4x4_f32_set_translate
  (
    idlib_matrix_4x4_f32* target,
    idlib_vector_3_f32 const* operand
  )
```

**Description**
Assign `target` the values of a translation matrix.
The matrix represents a translation along the x-, y-, and z-axis

```
| 1 | 0 | 0 | x |
| 0 | 1 | 0 | y |
| 0 | 0 | 1 | z |
| 0 | 0 | 0 | 1 |
```

where

```
x = idlib_vector_3_f32_get_x(operand)
y = idlib_vector_3_f32_get_y(operand)
z = idlib_vector_3_f32_get_z(operand)
```

**Parameters**
- `target` A pointer to an `idlib_matrix_4x4_f32` object. The result is assigned to that object.
- `operand` Pointer to the `idlib_vector_3_f32` object.
            Its x, y, and z component values denote the translations along the x-, y-, and z-axis, respectively.

**Remarks**
- The behavior of the function is undefined if `target` does not point to a `idlib_matrix_4x4_f32` object.
- The behavior of the function is undefined if `operand` does not point to a `idlib_vector_3_f32` object.
