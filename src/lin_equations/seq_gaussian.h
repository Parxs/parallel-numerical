#ifndef SEQ_GAUSSIAN_H
#define SEQ_GAUSSIAN_H

template <typename T>
void seq_Gaussian(T** A, T* b, T* y, long N){
  long i, j, k;
  
  for(k=0; k<N; k++) {		// k = current row
    for(i=k+1; i<N;i++ ) {		// in division step
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

    A[k][k] = 1.0;			// sets UTM diagonal value

    for(i=k+1; i<N; i++) {		// Gaussian elimination occurs
      T first_coeff = A[i][k];
      for(j=k+1; j<N; j++){		// in all remaining rows
        A[i][j] -= first_coeff * A[k][j];
      }
      b[i] -= first_coeff * y[k];
      A[i][k] = 0.0;
    }
  }
}


#endif // SEQ_GAUSSIAN_H