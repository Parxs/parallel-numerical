#ifndef SEQ_INTERPOLATION_H
#define SEQ_INTERPOLATION_H

#include <cmath>


// does not scale well enough to be used for benchmarking
template <typename T>
void seq_polation(T *x_values, T *y_values, long N, T x, T **Q){
  
  // algo from Numerical Analysis
  //  By Richard L. Burden, J. Douglas Faires
  // 123
  long i, j;
  #pragma omp parallel for
  for(i=0; i<N; i++){
    Q[i][0] = y_values[i];
  }
  
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