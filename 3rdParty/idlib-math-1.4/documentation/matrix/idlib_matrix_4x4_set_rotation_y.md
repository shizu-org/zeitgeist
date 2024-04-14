# idlib_matrix_4x4_f32_set_rotation_y

**Signature**
```
void
idlib_matrix_4x4_f32_set_rotation_y
  (
    idlib_matrix_4x4_f32* target,
    idlib_f32 operand
  )
```

**Description**
Assign `target` the values of a rotation matrix.
The matrix represents a counter-clockwise rotation around the y-axis.

```
|  c | 0 | s | 0 |
|  0 | 1 | 0 | 0 |
| -s | 0 | c | 0 |
|  0 | 0 | 0 | 1 |
```

where

```
c = cos(2 * π * operand / 360)
s = sin(2 * π * operand / 360)
```

**Parameters**
- `target` A pointer to an `idlib_matrix_4x4_f32` object. The result is assigned to that object.
- `operand` The angle, in degrees, of the rotation.

**Remarks**
- The behavior of the function is undefined if `target` does not point to a `idlib_matrix_4x4_f32` object.
