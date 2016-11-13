#include "util.h"
#include "lin_equations.h"
#include "c11_gaussian.h"

using namespace cEleven;

/**
 * @brief Solves the given linear equation.
 * @param A quatratic array holding the equations
 * @param b holding the rhs of the equation
 * @param y place to hold values necessary for backsubstitution
 * @param x place for the solutions
 * @param N dimension 
 * @return time taken
 **/
unsigned long solve(VALUE **A, VALUE *b, VALUE *y, VALUE *x, long N){
 unsigned long start_time = time_ms(), time;
  
  gaussian(A, b, y, N);
  
  
  // can only be done sequentially
  do_BackSub(A, y, x, N);
  
  time = time_ms()-start_time;
  
  return time;
}
