# Idlib
Welcome to *IdLib*.

*IdLib* is a C library providing functionality for
- numeric and trigonometric functions,
- matrix and vector math as well as
- color math

## *IdLib* intended use
The library is designed as *backing* library, that is, you can define your own matrix, vector, color types and simply
*embed* the types provided by IdLib into them.

The following example demonstrates this concept.
```
struct MyMatrix {
  idlib_matrix_4x4_f32 m;
};

void MyMatrix_initialize(MyMatrix* myMatrix) {
  idlib_matrix_4x4_f32_set_identity(&myMatrix->m);
}
```

## Notations
For convenience, this manual refers to a variable, which refers to an object, when it actually refers to the pointed object.

For example, in the description of the function
```
void
idlib_vector_3_f32_add
  (
    idlib_vector_3_f32* target,
    idlib_vector_3_f32 const* operand1,
    idlib_vector_3_f32 const* operand2
  );
```
we say
*add `operand2` to `operand1`*
which actually means
*add the `idlib_vector_3_f32` object pointed to by `operand1` to the `idlib_vector_3_f32` object pointed to by `operand2`*.

## Modules
- The *vector* module provides functionality related to vectors.
  [vector.md](vector.md)
- The *matrix* module provides functionality related to matrices.
  [matrix.md](matrix.md)
- The *color* module provides functionality related to colors.
  [color.md](matrix.md)
 