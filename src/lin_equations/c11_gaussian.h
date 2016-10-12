#ifndef C11_GAUSSIAN_HXX
#define C11_GAUSSIAN_HXX


// needed for std::function as otherwise
// error: ‘function’ is not a member of ‘std’
#include <future>


#include <iostream>

#include "util.h"

void _execute(std::function<bool (long, long, long)> func, long start, int num_tasks, long num_elems);


template <typename T>
void c11_Gaussian(T** A, T* b, T* y, long N){
  long k;
  
  auto div_code = [&A, &b, &y](long k, long start_task, long end_task) -> bool
  {
    using namespace std;
    long i;
    // start_task(>=k) & end_task set the boundaries of the task to be 
    // done however in this case i > k
    // this can be reached by either adapting _execute (e.g. new function)
    // or by checking whether start_task is k and if that is the case 
    // increasing it by one
    
    for(i=start_task; i<end_task; i++) {		// in division step
      if(A[k][k] != 0){
        A[k][i] = A[k][i] / A[k][k];
      }else{
        A[k][i] = 0;
      }
    }
    return true;
  };
  
  
  auto elim_code = [&A, &b, &y, &N](long k, long start_task, long end_task) -> bool
  {
    long i, j;

    for(i=start_task; i<end_task; i++) {		// Gaussian elimination occurs
      T first_coeff = A[i][k];
      // the inner loop is NOT split and it should access all rows where
      // j>k+1 therefore using start_task/end_task here would lead to a 
      // wrong result
      for(j=k+1; j<N; j++){		// in all remaining rows
        A[i][j] -=  first_coeff * A[k][j];
      }
      b[i] -= first_coeff * y[k];
      A[i][k] = 0.0;
    }
    return true;
  };



  for(k=0; k<N; k++) {		// k = current row
   
    // use get_num_threads as it gets less every iteration and
    // eventually N-k < num_threads
    _execute(div_code, k+1, (long)get_num_threads(N-k), N-k-1);

    if(A[k][k] != 0){			// calculates new value
      y[k] = b[k] / A[k][k];		// for equation solution
    }else{
      y[k] = 0.0;
    }

    A[k][k] = 1.0;			// sets UTM diagonal value
 
    // only eliminate when the current pivot is NOT in the last line 
    // ( last line is N-1!)
    if((k+1) != N){
      _execute(elim_code, k+1, (long)get_num_threads(N-k-1), N-k-1);
    }
  }
}


#endif //C11_GAUSSIAN_HXX
