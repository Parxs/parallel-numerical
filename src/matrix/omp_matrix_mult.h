#ifndef OMP_MATRIX_MULT_H
#define OMP_MATRIX_MULT_H

namespace openMP{

/**
 * @brief Naive matrix multiplication. Multiplies two 2D matrices.
 *
 * It is assumed that A,B & C are suitable for multiplying and
 * furthermore C must be initialized with 0.
 * There are also no checks for overflows, so it is easily possible
 * that C is not the correct result if the elements itself are too big 
 * @param A array holding first matrix
 * @param B array holding second matrix
 * @param C array that should hold the result
 * @param M first dimension of the first matrix
 * @param N second dimension of the first matrix
 * @param K second dimension of the second matrix
 */
template <typename T>
void multiply_matrix(T **A, T **B, T **C, long M, long N, long K){
  T a,b;
  #pragma omp parallel for  shared(A, B, C) private(a,b)
  for(long i=0; i<M; i++){
    for(long k=0; k<K; k++){
      for(long j=0; j<N; j++){
        a = A[i][j];
        b = B[j][k];
        C[i][k] += a*b;
      }
    }
  }
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
 * @param A array holding first matrix
 * @param B array holding second matrix
 * @param C array that should hold the result
 * @param M first dimension of the first matrix
 * @param N second dimension of the first matrix
 * @param K second dimension of the second matrix
 */
template <typename T>
void multiply_matrix_optimized(T **A, T **B, T **C, long M, long N, long K){
  T a,b;
  #pragma omp parallel for  shared(A, B, C) private(a,b)
  for(long i=0; i<M; i++){
    for(long j=0; j<N; j++){
      a = A[i][j];
      for(long k=0; k<K; k++){
        b = B[j][k];
        C[i][k] += a*b;
      }
    }
  }
}

/**
 * @brief Naive matrix multiplication. Multiplies two 2D matrices saved in a 1D array.
 *
 * It is assumed that A,B & C are suitable for multiplying and
 * furthermore C must be initialized with 0.
 * There are also no checks for overflows, so it is easily possible
 * that C is not the correct result if the elements itself are too big
 * @param A array holding first matrix
 * @param B array holding second matrix
 * @param C array that should hold the result
 * @param M first dimension of the first matrix
 * @param N second dimension of the first matrix
 * @param K second dimension of the second matrix 
 */
template <typename T>
void multiply_matrix(T *A, T *B, T *C, long M, long N, long K){
  T a,b;
  long a_index, b_index, c_index;
  
  #pragma omp parallel for shared(A, B, C) private(a,b,a_index,b_index,c_index)
  for(long i=0; i<M; i++){
    for(long k=0; k<K; k++){
      for(long j=0; j<N; j++){
        a_index = i*N+j;
        b_index = j*K+k;
        c_index = i*K+k;
        a = A[a_index];
        b = B[b_index];
        C[c_index] += a*b;
      }
    }
  } 
} 

/**
 * @brief Optimized matrix multiplication. Multiplies two 2D matrices saved in a 1D array.
 *
 * It is assumed that A,B & C are suitable for multiplying and
 * furthermore C must be initialized with 0.
 * There are also no checks for overflows, so it is easily possible
 * that C is not the correct result if the elements itself are too big
 * @param A array holding first matrix
 * @param B array holding second matrix
 * @param C array that should hold the result
 * @param M first dimension of the first matrix
 * @param N second dimension of the first matrix
 * @param K second dimension of the second matrix 
 */
template <typename T>
void multiply_matrix_optimized(T *A, T *B, T *C, long M, long N, long K){
  T a,b;
  long a_index, b_index, c_index;
  
  #pragma omp parallel for shared(A, B, C) private(a,b,a_index,b_index,c_index)
  for(long i=0; i<M; i++){
    for(long j=0; j<N; j++){
      a_index = i*N+j;
      a = A[a_index];
      for(long k=0; k<K; k++){
        b_index = j*K+k;
        c_index = i*K+k;
        b = B[b_index];
        C[c_index] += a*b;
      }
    }
  }  
}

}
#endif
