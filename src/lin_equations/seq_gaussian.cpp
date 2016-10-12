#include "util.h"
#include "lin_equations.h"
#include "seq_gaussian.h"

unsigned long solve(VALUE** A, VALUE* b, VALUE* y, VALUE *x, long N){
  unsigned long start_time = time_ms(), time;
  
  seq_Gaussian(A, b, y, N);
  
  // can only done sequentially 
  do_BackSub(A, y, x, N);
  
  time = time_ms()-start_time;
  

  
  return time;
}


