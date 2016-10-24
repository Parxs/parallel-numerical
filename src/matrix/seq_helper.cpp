/** 
 * @brief Helper function to allow easier switching between the 
 * implementations.
 */
#include "seq_matrix_mult.h"
#include "matrix_mult.h"
#include "util.h"

#include <iostream>

using namespace sequential;

unsigned long matrix_mult(VALUE** A, VALUE** B, VALUE** C, long M, long N, long K){
  unsigned long start = time_ms();
  
  multiply_matrix(A, B, C, M, N, K);
  
  return time_ms() - start;
}

unsigned long matrix_mult_optimized(VALUE** A, VALUE** B, VALUE** C, long M, long N, long K){
  unsigned long start = time_ms();
  
  multiply_matrix_optimized(A, B, C, M, N, K);
  
  return time_ms() - start;
}

unsigned long matrix_mult(VALUE* A, VALUE* B, VALUE* C, long M, long N, long K){
  unsigned long start = time_ms();
  
  multiply_matrix(A, B, C, M, N, K);
  
  return time_ms() - start; 
}

unsigned long matrix_mult_optimized(VALUE* A, VALUE* B, VALUE* C, long M, long N, long K){
  unsigned long start = time_ms();
  
  multiply_matrix_optimized(A, B, C, M, N, K);
    
  return time_ms() - start;
}
