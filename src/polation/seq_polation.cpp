#include "seq_polation.h"
#include "polation.h"
#include "util.h"


unsigned long polate(VALUE *x_values, VALUE *y_values, long N, VALUE x, VALUE **Q){
  unsigned long start = time_ms();
  
  seq_polation(x_values, y_values, N, x, Q);
  
  
  return time_ms()-start;
}
