#include "util.h"
#include "eigenvalue.h"
#include "c11_jacobi_eigen.h"

#include <iostream>
#include <future>
#include <thread>
#include <vector>

#ifndef MAGIC_NUMBER
#define MAGIC_NUMBER 3
#endif

using namespace cEleven;


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
