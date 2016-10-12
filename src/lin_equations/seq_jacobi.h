#ifndef SEQ_JACOBI_H
#define SEQ_JACOBI_H

#include "array.h"

#include <algorithm>
#include <cmath>

#include <iostream>

template <typename T>
void seq_jacobi(T **A, T *b, T* x, T eps, long max_iterations, long N){
  long i, j, k;
  
  T *old_x = x;
  T *new_x = new T[N]();

  using namespace std;

  for(k=0; k<max_iterations; k++){
    for(i=0; i<N; i++){
      new_x[i] = b[i];
      // initialize/reset new_x 
      for(j=0; j<N; j++){
        if(i==j){
          continue;
        }
        new_x[i] -= A[i][j] * old_x[j] ;
      }
      
      new_x[i] /= A[i][i];
    }
    
    
    
    //code for stopping when rounding error is minimal enough
    bool close_enough = true;
    for(i=0; i<N; i++){
      if(std::fabs(old_x[i] - new_x[i]) > eps){
        close_enough = false;
        break;
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


#endif // SEQ_JACOBI_H
