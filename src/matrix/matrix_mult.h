#ifndef MATRIX_MULT_H
#define MATRIX_MULT_H

#ifndef VALUE
#define VALUE double
#endif

unsigned long matrix_mult(VALUE** A, VALUE** B, VALUE** C, long M, long N, long K);
unsigned long matrix_mult_optimized(VALUE** A, VALUE** B, VALUE** C, long M, long N, long K);

unsigned long matrix_mult(VALUE* A, VALUE* B, VALUE* C, long M, long N, long K);
unsigned long matrix_mult_optimized(VALUE* A, VALUE* B, VALUE* C, long M, long N, long K);


#endif
