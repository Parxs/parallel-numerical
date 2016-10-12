#ifndef OMP_JACOBI_H
#define OMP_JACOBI_H

#include "array.h"

#include <algorithm>
#include <cmath>

#include <iostream>

template <typename T>
void omp_jacobi(T **A, T *b, T* x, T eps, long max_iterations, long N){
  long i, j, k;
  
  T *old_x = x;
  T *new_x = new T[N]();

  
  for(k=0; k<max_iterations; k++){
    #pragma omp parallel for private(j)
    for(i=0; i<N; i++){
      new_x[i] = b[i];
      // initialize/reset new_x 
      for(j=0; j<N; j++){
        if(i!=j){
          new_x[i] -= A[i][j] * old_x[j] ;
        }
      }
      
      new_x[i] /= A[i][i];
    }
    
    
    //code for stopping when rounding error is minimal enough
    bool close_enough = true;
    #pragma omp parallel for shared(old_x, new_x, eps) reduction(&& : close_enough)
    for(i=0; i<N; i++){
      if(std::fabs(old_x[i] - new_x[i]) > eps){
        close_enough = false;
        
        //break;
        // needs to be removed for OpenMP
      }
    }
    if(close_enough){
#ifdef DEBUG
      std::cout << "> Stopped because Approximation is close enough (eps: " << eps << "/ k: " << k << ")\n\n";
#endif
      // break out of the iterations
      break;
    }
    
    T *tmp = new_x;
    new_x = old_x;
    old_x = tmp;
    
  }
  
  if(new_x != x){
    std::copy(new_x, new_x+N, x);
    
    delete[] new_x;
  }else{
    delete[] old_x;
  }
}


#endif
