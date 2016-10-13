#include "util.h"
#include "lin_equations.h"
#include "seq_jacobi.h"


#ifndef MAX_ITERATIONS
#define MAX_ITERATIONS 10000
#endif

unsigned long solve(VALUE **A, VALUE *b, VALUE *y, VALUE *x, long N){
  unsigned long start = time_ms(), end;
  
  seq_jacobi(A, b, x, EPSILON*EPSILON, MAX_ITERATIONS, N);
  
  
  end = time_ms();
  
  return end-start;
  
  
}
