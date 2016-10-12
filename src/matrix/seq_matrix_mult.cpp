/** 
 * @brief Sequential implementation of matrix multiplication.
 * @date 02.02.2016
 */

#include <iostream>
#include <cstdlib>

#include "matrix_mult.h"
#include "util.h"


/**
 * @brief Naive matrix multiplication. Multiplies two 2D matrices.
 *
 * It is assumed that A,B & C are suitable for multiplying and
 * furthermore C must be initialized with 0.
 * There are also no checks for overflows, so it is easily possible
 * that C is not the correct result if the elements itself are too big 
 */
unsigned long multiply_matrix2D(VALUE** A, VALUE** B, VALUE** C, long M, long N, long K){
  unsigned long start = time_ms();
  VALUE a,b;
  long i, j, k;
  for(i=0; i<M; i++){
    for(k=0; k<K; k++){
      for(j=0; j<N; j++){
        a = A[i][j];
        b = B[j][k];
        C[i][k] += a*b;
      }
    }
  }
  
  return time_ms() - start;
}


/**
 * @brief Optimized matrix multiplication. Multiplies two 2D matrices.
 *
 * It is assumed that A,B & C are suitable for multiplying and
 * furthermore C must be initialized with 0.
 * This algorithm has been optimized by accessing the elements which are 
 * saved closed to already accessed ones first.
 * There are also no checks for overflows, so it is easily possible
 * that C is not the correct result if the elements itself are too big 
 */
unsigned long multiply_matrix2D_optimized(VALUE** A, VALUE** B, VALUE** C, long M, long N, long K){
  unsigned long start = time_ms();
  VALUE a,b;
  long i, j, k;
  for(i=0; i<M; i++){
    for(j=0; j<N; j++){
      a = A[i][j];
      for(k=0; k<K; k++){
        b = B[j][k];
        C[i][k] += a*b;
      }
    }
  }
  return time_ms() - start;
}


/**
 * @brief Naive matrix multiplication. Multiplies two 2D matrices saved in a 1D array.
 *
 * It is assumed that A,B & C are suitable for multiplying and
 * furthermore C must be initialized with 0.
 * There are also no checks for overflows, so it is easily possible
 * that C is not the correct result if the elements itself are too big 
 */
unsigned long multiply_matrix1D(VALUE* A, VALUE* B, VALUE* C, long M, long N, long K){
  unsigned long start = time_ms();
  VALUE a,b;
  long a_index, b_index, c_index;
  long i, j, k;
  for(i=0; i<M; i++){
    for(k=0; k<K; k++){
      for(j=0; j<N; j++){
        a_index = i*N+j;
        b_index = j*K+k;
        c_index = i*K+k;
        a = A[a_index];
        b = B[b_index];
        C[c_index] += a*b;
      }
    }
  } 
  return time_ms() - start; 
}


/**
 * @brief Optimized matrix multiplication. Multiplies two 2D matrices saved in a 1D array.
 *
 * It is assumed that A,B & C are suitable for multiplying and
 * furthermore C must be initialized with 0.
 * There are also no checks for overflows, so it is easily possible
 * that C is not the correct result if the elements itself are too big 
 */
unsigned long multiply_matrix1D_optimized(VALUE* A, VALUE* B, VALUE* C, long M, long N, long K){
  unsigned long start = time_ms();
  VALUE a,b;
  long a_index, b_index, c_index;
  long i, j, k;
  for(i=0; i<M; i++){
    for(j=0; j<N; j++){
      a_index = i*N+j;
      a = A[a_index];
      for(k=0; k<K; k++){
        b_index = j*K+k;
        c_index = i*K+k;
        b = B[b_index];
        C[c_index] += a*b;
      }
    }
  }  
  return time_ms() - start;
}
