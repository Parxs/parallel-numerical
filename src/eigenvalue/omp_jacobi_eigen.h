#ifndef OMP_JACOBI_EIGEN_H
#define OMP_JACOBI_EIGEN_H

#include <cmath>
#include <iostream>
#include "array.h"

#ifndef EPS
#define EPS 1.0e-36
#endif

namespace openMP{

template <typename T>
struct Container { T val; long k; long l;};

template <typename T>
struct Container<T> _init_Container(T val, long k, long l);

template <typename T>
struct Container<T> _init_Container(T val, long k, long l){
  struct Container<T> cont;
  cont.val = val;
  cont.k = k;
  cont.l = l;
  return cont;
}

template <typename T>
void copy_Diagonals_to_1D(T *d, T **A, long N){
  #pragma omp parallel for shared(A,d)
  for(long i=0; i<N; i++){
    d[i] = A[i][i];
  }
}

template <typename T>
T get_Max_Off_Diagonal(T **A, long *k, long *l, long N){
  struct Container<T> max = _init_Container(0.0, 0, 1);
  
  for(long i=0; i<N-1; i++){
    for(long j=i+1; j<N; j++){
      T tmp = fabs(A[i][j]);
      if( tmp >= max.val){
        max.val = tmp;
        max.k = i;
        max.l = j;
      }
    }
  }
  
  *k = max.k;
  *l = max.l;
  return max.val;
}

template <typename T>
void rotate(T **A, T *d, T **V, long k, long l, long N){
  T diff, phi, tau, tmp, t, c, s;
  
  diff = d[l] - d[k];
  if(fabs(A[k][l]) < fabs(diff)*EPS){
    t = A[k][l]/diff;
  }else{
    phi = diff/(2.0*A[k][l]);
    t = 1.0/(fabs(phi) + std::sqrt(phi*phi + 1.0));
    if(phi < 0.0){
        t = -t;
    }
  }
  c = 1.0/std::sqrt(t*t + 1.0); 
  s = t*c;
  tau = s/(1.0 + c);
  tmp = A[k][l];
  A[k][l] = 0.0;
  d[k] = d[k] - t*tmp;
  d[l] = d[l] + t*tmp;
  
  #pragma omp parallel shared(V, A, s, tau)
  {
    // case of i < k
    #pragma omp for nowait
    for(long i=0; i<k; i++){
      T tmp = A[i][k];
      A[i][k] = tmp - s*(A[i][l] + tau*tmp);
      A[i][l] = A[i][l] + s*(tmp - tau*A[i][l]);
    }
    // case of k < i < l
    #pragma omp for nowait 
    for(long i=k+1; i<l; i++){
      T tmp = A[k][i];
      A[k][i] = tmp - s*(A[i][l] + tau*A[k][i]);
      A[i][l] = A[i][l] + s*(tmp - tau*A[i][l]);
    }
    // case of i > l
    #pragma omp for nowait
    for(long i=l+1; i<N; i++){
      T tmp = A[k][i];
      A[k][i] = tmp - s*(A[l][i] + tau*tmp);
      A[l][i] = A[l][i] + s*(tmp - tau*A[l][i]);
    }
    // update transformation matrix
    #pragma omp for nowait
    for(long i=0; i<N; i++){
      T tmp = V[i][k];
      V[i][k] = tmp - s*(V[i][l] + tau*V[i][k]);
      V[i][l] = V[i][l] + s*(tmp - tau*V[i][l]);
    }
  }
}


template <typename T>
long jacobi_eigen(T **A, long N, T *d, T **V, long max_iterations, T epsilon){
  long i, k, l;
  T max;
  
  init_Identity(V, N);
  
  copy_Diagonals_to_1D(d, A, N);
  
  for(i=0; i<max_iterations; i++) {
    max = get_Max_Off_Diagonal(A, &k, &l, N);
    
    if(max<epsilon){
      // break when close enough
      return i;
    }
    
    rotate(A, d, V, k, l, N);
    
  }
  
  return -1;
}

}

#endif
