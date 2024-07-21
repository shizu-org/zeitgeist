# idlib_vector_3_f32_add

**Signature**
```
void
idlib_vector_3_f32_lerp
  (
    idlib_matrix_4x4_f32* target,
    idlib_matrix_4x4_f32 const* operand1,
    idlib_matrix_4x4_f32 const* operand2,
    idlib_f32 operand3
  );
```

**Description**
Linearily interpolate between `operand1` and `operand2` based on `operand3` and assign the result to `target`.

Linear interpolation is defined as follows:
Let `x` be the start of the interpolation and `y` be the end of the interpolation.
Let `t` is the interpolation factor.
The the linear interpolation between `x` and `y` based on `t` is defined as
  ```
  (1-t') * x + t' * y
  t' = clamp(t, 0,1)
  ```

**Parameters**
- `target` Pointer to an `idlib_vector_3_f32` object. The object is assigned the result.
- `operand1` Pointer to an `idlib_vector_3_f32` object. The object is the start of the interpolation.
- `operand2` Pointer to an `idlib_vector_3_f32` object. The object is the end of the interpolation.
- `operand3` Pointer to an `idlib_vector_3-f32` object. The interpolation factor.
   The interpolation factor is clamped to `[0,1]`.

**Remarks**
- The behavior of the function is undefined if `operand1`, `operand2`, or `target` do not point to `idlib_vector_3_f32` objects.
- `operand1`, `operand2`, and `target` can all point to the same `idlib_matrix_4x4_f32` object.
