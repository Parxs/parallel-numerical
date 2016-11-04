#ifndef EIGENVALUE_H
#define EIGENVALUE_H

#ifndef VALUE
#define VALUE double
#endif

// for rounding errors
#ifndef EPSILON
#define EPSILON 1.0e-9
#endif

#include "util.h"

#include <iostream>

unsigned long calculate_Eigen(VALUE **A, VALUE *d, VALUE **V, long N);

template <typename T> 
void init_Example(T **A, long N){
  A[0][0] = 3;
  A[0][1] = -1;
  A[0][2] = 0;
  
  A[1][0] = -1;
  A[1][1] = 2;
  A[1][2] = -1;
    
  A[2][0] = 0;
  A[2][1] = -1;
  A[2][2] = 3;
  
  // expected eigenvalues: 1,3,4
}

template <typename T>
bool check_Eigenvalue(T **A, T *d, T **V, long index, long N){
  bool equal = true;
  T *res = new T[N](); 
  
  // calculate A*x
  for(long i=0; i<N; i++){
    for(long j=0; j<N; j++){
        res[i] += A[i][j]*V[j][index];
    }
    
    // divide by Eigenvalue then res[i] must be equal to v[i][index]
    res[i] /= d[index];
  }
  
  // compare lambda*x (res/lambda) with x
  for(long i=0; i<N; i++){
    if(fabs(res[i]-V[i][index]) > EPSILON){
      equal = false;
      
#ifdef DEBUG
      std::cout << std::scientific;
      std::cout << "> ERROR: A*x != lambda*x! - Example: \n";
      std::cout << "Expected:\t" << V[i][index] << "\n";
      std::cout << "Got:\t\t" << res[i] << "\n";
      std::cout << "Diff:\t\t" << fabs(res[i]-V[i][index]) << "\n\n";
#endif
      break;
    }
  }
  
  delete[] res;
  
  return equal;
}


template <typename T>
bool check_Eigenvalues(T **A, T *d, T **V, long N){  
  for(long i=0; i<N; i++){
    if(!check_Eigenvalue(A, d, V, i, N)){
      return false;
    } 
  }
  
  return true;
}

#endif
