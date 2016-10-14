#include "util.h"
#include "lin_equations.h"
#include "c11_gaussian.h"

unsigned long solve(VALUE **A, VALUE *b, VALUE *y, VALUE *x, long N){
 unsigned long start_time = time_ms(), time;
  
  c11_Gaussian(A, b, y, N);
  
  
  // can only be done sequentially
  do_BackSub(A, y, x, N);
  
  time = time_ms()-start_time;
  
  
  
  
  return time;
}
