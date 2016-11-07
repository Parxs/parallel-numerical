#ifndef SEQ_JACOBI_EIGEN_H
#define SEQ_JACOBI_EIGEN_H


#include <cmath>
#include <iostream>
#include "array.h"

#ifndef EPS
#define EPS 1.0e-36
#endif

namespace sequential{
  
/**
 * @brief Copies the diagonals of a symmetric matrix into an 1D array
 * @param d array which should hold the diagonal
 * @param A array from which the diagonal should be copied
 * @param N dimension
 **/
template <typename T>
void copy_Diagonals_to_1D(T *x, T **A, long N){
  for(long i=0; i<N; i++){
    x[i] = A[i][i];
  }
}

/**
 * @brief Calculates the biggest element not in the diagonal of a symmetric matrix.
 *
 * Because the matrix is symmetric only the upper-triangle matrix needs
 * to be searched through.
 * @param A matrix whose biggest off-diagonal element is needed
 * @param k place for first index of biggest element
 * @param j place for second index of biggest element
 * @param N dimension
 * @return biggest element
 **/
template <typename T>
T get_Max_Off_Diagonal(T **A, long *k, long *l, long N){
  *l = 0;
  *k = 0;
  T max = 0.0;
  
  for(long i=0; i<N-1; i++){
    for(long j=i+1; j<N; j++){
      if(fabs(A[i][j]) >= max){
        max = fabs(A[i][j]);
        *k = i;
        *l = j;
      }
    }
  }
  
  return max;
}

/**
 * @brief Rotates the given symmetric matrix such that the element_kl becomes close to 0.
 * 
 * The two important things to remember when calling this function is 
 * that d must have the current diagonal of the matrix and that the 
 * elements of the matrix itself will only be changed if they are in its
 * upper-triangle matrix.
 * This works because the matrix is symmetric, therefore the lower-
 * triangle matrix is always the same as the upper-triangle.
 * By not changing the lower one it is always possible to reconstruct
 * the symmetric matrix before all rotations.
 * The diagonal is also not changed as for that d is used.
 * @param A matrix to be rotated
 * @param d array holding the diagonal of that matrix
 * @param V place for the eigenvectors
 * @param k first index for the rotation
 * @param j second index for the rotation
 **/
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
  
  // case of i < k
  for(long i=0; i<k; i++){
    tmp = A[i][k];
    A[i][k] = tmp - s*(A[i][l] + tau*tmp);
    A[i][l] = A[i][l] + s*(tmp - tau*A[i][l]);
  }
  // case of k < i < l
  for(long i=k+1; i<l; i++){
    tmp = A[k][i];
    A[k][i] = tmp - s*(A[i][l] + tau*A[k][i]);
    A[i][l] = A[i][l] + s*(tmp - tau*A[i][l]);
  }
  // case of i > l
  for(long i=l+1; i<N; i++){
    tmp = A[k][i];
    A[k][i] = tmp - s*(A[l][i] + tau*tmp);
    A[l][i] = A[l][i] + s*(tmp - tau*A[l][i]);
  }
  // update transformation matrix
  for(long i=0; i<N; i++){
    tmp = V[i][k];
    V[i][k] = tmp - s*(V[i][l] + tau*V[i][k]);
    V[i][l] = V[i][l] + s*(tmp - tau*V[i][l]);
  }
}

/**
 * @brief Calculate the eigen-value and -vectors of a given symmetric matrix.
 * 
 * Only the upper-triangle matrix of this matrix will be changed.
 * The resulting diagonal and also eigenvalues can be found in d.
 * Epsilon is used to determine how close off-diagonal elements should 
 * be to 0. This is not the same as how close the eigenvalues should be
 * to the correct ones. Therefore epsilon needs to smaller than the
 * epsilon used for comparison.
 * @param A matrix to be used
 * @param N dimension
 * @param d place to save the final diagonal/eigenvalues
 * @param V place to save the eigenvectors
 * @param max_iterations the maximal iterations the algorithm should run
 * @param epsilon how close the off-diagonal elements should be to 0
 * @return number of iterations until values were clos enough
 **/
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
