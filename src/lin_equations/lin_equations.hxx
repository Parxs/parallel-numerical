#ifndef LIN_EQUATIONS_HXX
#define LIN_EQUATIONS_HXX

#include <iostream>

/**
 * @brief Initialize the equations such that the there is a solution.
 * @param A array for the lhs of the equations
 * @param b array for the rhs of the equations
 * @param N dimension
 * @param lower lower boundary for the random numbers
 * @param upper upper boundary for the random numbers
 **/
template <typename T>
void init_Equations(T **A, T *b, long N, T lower, T upper){
  // to avoid that one equation is the scalar of another equation the 
  // matrices will be filled such that there is a solution
  long i, j;
  for(i=0; i<N; i++) {	
    b[i] = 0.0;			
    for(j=0; j<N; j++) {	
      T r = get_rand(lower, upper);	
      A[i][j] = r;
      
      if(i==j){
        // needed to allow jacobi to converge
        // jacobi only works if |a_ii| > |sum_i(a_ij)|
        A[i][j] *= upper*N;
      }
      b[i] += j*r;
    }
  }
}

/**
 * @brief Calculate the backsubstitution.
 * 
 * Cannot be done efficiently in parallel as there are dependencies.
 * @param A holding the matrix
 * @param y holding the result
 * @param x place for the solutions
 **/
template <typename T>
void do_BackSub(T **A, T *y, T* x, long N){
  long i, j;

  for( i=N-1; i>=0; i-- ) {
    x[i] = y[i];
    for( j=N-1; j>i; j-- ) 

      x[i] -= x[j] * A[i][j];
  }
}


/**
 * @brief Checks whether the given solution is correct.
 * @param A array holding the lhs of the equation
 * @param b array holding the rhs of the equation
 * @param x array holding the solutions
 * @param epsilon maximal allowed error
 * @param N dimension 
 * @return true if the solutions are correct, false otherwise
 **/
template <typename T>
bool check_Solution(T **A, T *b, T *x, T epsilon, long N){
  using namespace std;
  long i, j;
  for(i=0; i<N; i++){
    T res = 0;
    for(j=0; j<N; j++){
      res += A[i][j]*x[j];
    }
    
    if(fabs(res-b[i]) > epsilon){
#ifdef DEBUG
      cout << scientific << showpos;
      cout << "> Expected:\t" << b[i] << "\n";
      cout << "> Got:\t\t" << res << "\n";
      cout << "> Diff:\t\t" << fabs(res-b[i]) << "\n\n";
#endif
      return false;
    }
    
  }
  
  return true;
}



#endif // LIN_EQUATIONS_HXX
