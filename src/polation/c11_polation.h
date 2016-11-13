#ifndef c11_INTERPOLATION_H
#define c11_INTERPOLATION_H

#include "util.h"

#include <cmath>
#include <future>
#include <thread>
#include <vector>

namespace cEleven{


/**
 * @brief Executes the given function in parallel.
 * @param func function to be done in parallel
 * @param start index of first element that should be run through the func
 * @param num_workers how high the parallelism should be
 * @param num_elems number of elements that should be run through the func
 **/
void _execute(std::function<bool (long, long)> func, long start, int num_workers, long num_elems){
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
    futures.push_back(async(launch::async, func, start_task, end_task)); 
    
    start_task = end_task;
    end_task = start_task + chunksize;
  }
  
  for(future<bool> &f: futures){
    f.get();
  }
}


// does not scale well enough to be used for benchmarking
/**
 * @brief Polates x in respect to some given values
 * @param x_values all other x values
 * @param y_values all other y values
 * @param N size of the arrays involved
 * @param x value to be interpolated
 * @param Q helper matrix which will hold resulting y
 **/
template <typename T>
void polation(T *x_values, T *y_values, long N, T x, T **Q){
  /*==================================================================*/
  // Algorithm from Numerical Analysis
  // By Richard L. Burden, J. Douglas Faires, Annette M. Burden
  /*==================================================================*/
  auto code = [Q, y_values, N](long start_task, long end_task)->bool{
    for(long i=start_task; i<end_task; i++){
      Q[i][0] = y_values[i];
    }
    return true;
  };
  
  _execute(code, 0, get_num_threads(N), N);
  
  for(long i=1; i<N; i++){
    T x_i = x_values[i];
    
    for(long j=1; j<=i; j++){
        
      
        T dividend = ((x-x_values[i-j])*(Q[i][j-1]) - (x-x_i)*(Q[i-1][j-1]));
        T divisor =  (x_i-x_values[i-j]);
        
        Q[i][j] = dividend/divisor; 
    }
  }
}

}
#endif 
