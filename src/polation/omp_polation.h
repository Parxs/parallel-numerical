#ifndef OMP_INTERPOLATION_H
#define OMP_INTERPOLATION_H

namespace openMP{
// does not scale well enough to be used for benchmarking
/**
 * @brief Polates x in respect to some given values
 * @param x_values all other x values
 * @param y_values all other y values
 * @param N size of the arrays involved
 * @param x value to be interpolated
 * @param Q helper matrix which will hold resulting y
 **/
template <typename T>
void polation(T *x_values, T *y_values, long N, T x, T **Q){
  /*==================================================================*/
  // Algorithm from Numerical Analysis
  // By Richard L. Burden, J. Douglas Faires, Annette M. Burden
  /*==================================================================*/
  #pragma omp parallel for
  for(long i=0; i<N; i++){
    Q[i][0] = y_values[i];
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

}
#endif 
