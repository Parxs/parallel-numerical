#ifndef MATRIX_MULT_H
#define MATRIX_MULT_H

#ifndef VALUE
#define VALUE double
#endif

#ifndef EPSILON
#define EPSILON 1e-10
#endif

/* matrix multiplication */
unsigned long multiply_matrix2D(VALUE** A, VALUE** B, VALUE** C, long M, long N, long K);
unsigned long multiply_matrix2D_optimized(VALUE** A, VALUE** B, VALUE** C, long M, long N, long K);

unsigned long multiply_matrix1D(VALUE* A, VALUE* B, VALUE* C, long M, long N, long K);
unsigned long multiply_matrix1D_optimized(VALUE* A, VALUE* B, VALUE* C, long M, long N, long K);


#endif //MATRIX_MULT_H
