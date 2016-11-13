#include "util.h"
#include "eigenvalue.h"
#include "omp_jacobi_eigen.h"

#include <iostream>

// arbitary number to increase the size of iterations
#ifndef MAGIC_NUMBER
#define MAGIC_NUMBER 3
#endif

using namespace openMP;

/**
 * @brief Measures the time it takes for calculating the eigenvalues of a matrix.
 * 
 * The matrix will be changed during its call but diagonal and lower-
 * triangle matrix will stay the same.
 * @param A matrix to be used for calculation
 * @param d array which should hold the eigenvalues
 * @param V array that should hold the eigenvectors
 * @param N dimension of array
 * @return time taken
 **/
unsigned long calculate_Eigen(VALUE **A, VALUE *d, VALUE **V, long N){
  unsigned long start, end;
  long iterations, max_iterations;
  
#ifndef MAX_ITERATIONS
  max_iterations = N*N*MAGIC_NUMBER;
#else
  max_iterations = MAX_ITERATIONS;
#endif
  
  start = time_ms();
  
  // EPSILON*EPSILON such that the result is very close
  iterations = jacobi_eigen(A, N, d, V, max_iterations, EPSILON*EPSILON);
  
  end = time_ms()-start;
  
  if(iterations == -1){
    std::cout << "> ERROR: did not converge even after " << max_iterations <<" iterations!\n";
  }else{
    std::cout << "> Iterations: " << iterations << "\n";
  }
  
  return end;
}
