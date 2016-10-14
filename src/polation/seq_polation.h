#ifndef SEQ_INTERPOLATION_H
#define SEQ_INTERPOLATION_H

#include <cmath>
#include <iostream>

// does not scale well enough to be used for benchmarking
template <typename T>
void seq_polation(T *x_values, T *y_values, long N, T x, T **Q){
  /*==================================================================*/
  // Algorithm from Numerical Analysis
  // By Richard L. Burden, J. Douglas Faires
  /*==================================================================*/
  #pragma omp parallel for
  for(long i=0; i<N; i++){
    T tmp = y_values[i];
    Q[i][0] = tmp;
  }
  
  for(long i=1; i<N; i++){
    T x_i = x_values[i];
    
    for(long j=1; j<=i; j++){
        
      
        T dividend = ((x-x_values[i-j])*(Q[i][j-1]) - (x-x_i)*(Q[i-1][j-1]));
        T divisor =  (x_i-x_values[i-j]);
        
        Q[i][j] = dividend/divisor; 
    }
  }
}
#endif 
