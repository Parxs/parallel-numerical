#include "util.h"
#include "lin_equations.h"
#include "c11_jacobi.h"


#ifndef MAX_ITERATIONS
#define MAX_ITERATIONS 10000
#endif

#include <iostream>
#include <future>
#include <thread>
#include <vector>

using namespace std;


unsigned long solve(VALUE **A, VALUE *b, VALUE *y, VALUE *x, long N){
  unsigned long start = time_ms(), end;
  
  c11_jacobi(A, b, x, EPSILON*EPSILON, MAX_ITERATIONS, N);
  
  
  end = time_ms();
  
  return end-start;
}
