#ifndef SEQ_GAUSSIAN_H
#define SEQ_GAUSSIAN_H

namespace sequential{

/**
 * @brief Use gaussian elimination on an system of linear equations.
 * @param A lhs of the system
 * @param b rhs of the system
 * @param y place to hold values necessary for backsubstitution
 * @param N dimensions
 **/
template <typename T>
void gaussian(T** A, T* b, T* y, long N){
  long i, j, k;
  
  for(k=0; k<N; k++) {		// for every row
    for(i=k+1; i<N;i++ ) {	
      if( A[k][k] != 0){
        A[k][i] = A[k][i] / A[k][k];
      }else{
        A[k][i] = 0;
      }
    }

    if(A[k][k] != 0){			// calculates new value
      y[k] = b[k] / A[k][k];		// for equation solution
    }else{
      y[k] = 0.0;
    }

    A[k][k] = 1.0;			// set diagonal value

    for(i=k+1; i<N; i++) {		// Gaussian elimination
      T first_coeff = A[i][k];
      for(j=k+1; j<N; j++){	
        A[i][j] -= first_coeff * A[k][j];
      }
      b[i] -= first_coeff * y[k];
      A[i][k] = 0.0;
    }
  }
}

}
#endif // SEQ_GAUSSIAN_H
