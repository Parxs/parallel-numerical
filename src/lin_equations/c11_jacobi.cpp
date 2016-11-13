#include "util.h"
#include "lin_equations.h"
#include "c11_jacobi.h"


#ifndef MAX_ITERATIONS
#define MAX_ITERATIONS 10000
#endif

using namespace cEleven;


/**
 * @brief Solves the given linear equation.
 * @param A quatratic array holding the equations
 * @param b holding the rhs of the equation
 * @param y not used but necessary to allow easy switching
 * @param x place for the solutions
 * @param N dimension 
 * @return time taken
 **/
unsigned long solve(VALUE **A, VALUE *b, VALUE *y, VALUE *x, long N){
  unsigned long start = time_ms(), end;
  
  jacobi(A, b, x, EPSILON*EPSILON, MAX_ITERATIONS, N);
  
  
  end = time_ms();
  
  return end-start;
}
