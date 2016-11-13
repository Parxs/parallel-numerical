#include "c11_matrix_mult.h"
#include "matrix_mult.h"
#include "util.h"

using namespace cEleven;

/**
 * @brief Multiplies two 2D matrices.
 * @param A array holding first matrix
 * @param B array holding second matrix
 * @param C array that should hold the result
 * @param M first dimension of the first matrix
 * @param N second dimension of the first matrix
 * @param K second dimension of the second matrix
 * @return time taken
 **/
unsigned long matrix_mult(VALUE** A, VALUE** B, VALUE** C, long M, long N, long K){
  unsigned long start = time_ms();
  
  multiply_matrix(A, B, C, M, N, K);
  
  return time_ms() - start;
}

/**
 * @brief Multiplies two 2D matrices in a optimized way.
 * @param A array holding first matrix
 * @param B array holding second matrix
 * @param C array that should hold the result
 * @param M first dimension of the first matrix
 * @param N second dimension of the first matrix
 * @param K second dimension of the second matrix
 * @return time taken
 **/
unsigned long matrix_mult_optimized(VALUE** A, VALUE** B, VALUE** C, long M, long N, long K){
  unsigned long start = time_ms();
  
  multiply_matrix_optimized(A, B, C, M, N, K);
  
  return time_ms() - start;
}

/**
 * @brief Multiplies two 1D matrices.
 * @param A array holding first matrix
 * @param B array holding second matrix
 * @param C array that should hold the result
 * @param M first dimension of the first matrix
 * @param N second dimension of the first matrix
 * @param K second dimension of the second matrix
 * @return time taken
 **/
unsigned long matrix_mult(VALUE* A, VALUE* B, VALUE* C, long M, long N, long K){
  unsigned long start = time_ms();
  
  multiply_matrix(A, B, C, M, N, K);
  
  return time_ms() - start; 
}

/**
 * @brief Multiplies two 1D matrices in a optimized way.
 * @param A array holding first matrix
 * @param B array holding second matrix
 * @param C array that should hold the result
 * @param M first dimension of the first matrix
 * @param N second dimension of the first matrix
 * @param K second dimension of the second matrix
 * @return time taken
 **/
unsigned long matrix_mult_optimized(VALUE* A, VALUE* B, VALUE* C, long M, long N, long K){
  unsigned long start = time_ms();
  
  multiply_matrix_optimized(A, B, C, M, N, K);
    
  return time_ms() - start;
}
