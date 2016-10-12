#ifndef c11_INTERPOLATION_H
#define c11_INTERPOLATION_H

#include "util.h"

#include <cmath>


// does not scale well enough to be used for benchmarking
template <typename T>
void c11_polation(T *x_values, T *y_values, long N, T x, T **Q){
  
  // algo from Numerical Analysis
  //  By Richard L. Burden, J. Douglas Faires
  // 123
  
  auto code = [Q, y_values, N](long start_task, long end_task)->bool{
    long i;
    for(i=start_task; i<end_task; i++){
      Q[i][0] = y_values[i];
    }
    return true;
  };
  
  _execute(code, 0, get_num_threads(N), N);
  
  long i, j;
  for (i=1; i<N; i++){
    T x_i = x_values[i];
    
    for (j=1; j<=i; j++){
        
      
        T dividend = ((x-x_values[i-j])*(Q[i][j-1]) - (x-x_i)*(Q[i-1][j-1]));
        T divisor =  (x_i-x_values[i-j]);
        
        Q[i][j] = dividend/divisor; 
    }
  }
}







#endif //SEQ_POLATION_H
