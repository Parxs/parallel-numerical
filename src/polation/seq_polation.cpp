#include "seq_polation.h"
#include "polation.h"
#include "util.h"

using namespace sequential;


/**
 * @brief Measures the execution time of the polation.
 * @param x_values all other x values
 * @param y_values all other y values
 * @param N size of the arrays involved
 * @param x value to be interpolated
 * @param Q helper matrix which will hold resulting y
 * @return time measured
 **/
unsigned long polate(VALUE *x_values, VALUE *y_values, long N, VALUE x, VALUE **Q){
  unsigned long start = time_ms();
  
  polation(x_values, y_values, N, x, Q);
  
  
  return time_ms()-start;
}
