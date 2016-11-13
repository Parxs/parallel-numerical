#ifndef C11_GAUSSIAN_HXX
#define C11_GAUSSIAN_HXX

#include <future>
#include <iostream>
#include <thread>
#include <vector>

#include "util.h"

namespace cEleven{
  
/**
 * @brief Executes the given function in parallel.
 * @param func function to be done in parallel
 * @param start index of first element that should be run through the func
 * @param num_workers how high the parallelism should be
 * @param num_elems number of elements that should be run through the func
 **/
void _execute(std::function<bool (long, long, long)> func, long start, int num_workers, long num_elems){
  using namespace std;
  vector<future<bool>> futures;
  int t, extra; // extra will at most be as big as num_workers-1
  long chunksize, start_task, end_task;
  
  // split work
  chunksize = num_elems / num_workers;
	extra = num_elems % num_workers; 
	start_task = start;
	end_task = start+chunksize;
  
  // run threads
  for(t=0; t<num_workers; t++){
    // test whether extra work still needs to be done
    if(t < extra){
      end_task++;
    }
    
    // start-1 to tell the function which index the current pivot has
    futures.push_back(async(launch::async, func, start-1, start_task, end_task)); 
    
    start_task = end_task;
    end_task = start_task + chunksize;
  }
  
  for(future<bool> &f: futures){
    f.get();
  }
}

/**
 * @brief Use gaussian elimination on an system of linear equations.
 * @param A lhs of the system
 * @param b rhs of the system
 * @param y place to hold values necessary for backsubstitution
 * @param N dimensions
 **/
template <typename T>
void gaussian(T** A, T* b, T* y, long N){
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
    
    for(i=start_task; i<end_task; i++) {	
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

    for(i=start_task; i<end_task; i++) {		// Gaussian elimination
      T first_coeff = A[i][k];
      // the inner loop is NOT split and it should access all rows where
      // j>k+1 therefore using start_task/end_task here would lead to a 
      // wrong result
      for(j=k+1; j<N; j++){		
        A[i][j] -=  first_coeff * A[k][j];
      }
      b[i] -= first_coeff * y[k];
      A[i][k] = 0.0;
    }
    return true;
  };



  for(k=0; k<N; k++) {		// for every row
   
    // use get_num_threads as it gets less every iteration and
    // eventually N-k < num_threads
    _execute(div_code, k+1, (long)get_num_threads(N-k), N-k-1);

    if(A[k][k] != 0){			// calculates new value
      y[k] = b[k] / A[k][k];		// for equation solution
    }else{
      y[k] = 0.0;
    }

    A[k][k] = 1.0;			// set diagonal value
 
    // only eliminate when the current pivot is NOT in the last line 
    // ( last line is N-1!)
    if((k+1) != N){
      _execute(elim_code, k+1, (long)get_num_threads(N-k-1), N-k-1);
    }
  }
}

}

#endif //C11_GAUSSIAN_HXX
