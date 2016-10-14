#ifndef OMP_JACOBI_H
#define OMP_JACOBI_H

#include "array.h"

#include <iostream>
#include <future>
#include <thread>
#include <vector>
#include <algorithm>
#include <cmath>

bool _execute(std::function<bool (long, long)> func, long start, int num_workers, long num_elems){
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
    if(!f.get())
      return false;
  }
  return true;
}


template <typename T>
void c11_jacobi(T **A, T *b, T* x, T eps, long max_iterations, long N){
  long k;
  
  T *old_x = x;
  T *new_x = new T[N]();
  
  auto check_closeness = [A, new_x, old_x, N, eps](long start_task, long end_task) -> bool
  {
    //code for stopping when rounding error is minimal enough
    long i;
    for(i=start_task; i<end_task; i++){
      if(std::fabs(old_x[i] - new_x[i]) > (eps*0.1)){
        
        return false;
        // needs to be removed for OpenMP
      }
    }
    return true;
  };
  
 
  
  for(k=0; k<max_iterations; k++){
    auto approx_code = [A, new_x, old_x, b, N](long start_task, long end_task) -> bool
    {
      long i, j;
      for(i=start_task; i<end_task; i++){
        new_x[i] = b[i];
        // initialize/reset new_x 
        for(j=0; j<N; j++){
          if(i!=j){
            new_x[i] -= A[i][j] * old_x[j] ;
          }
        }
        
        new_x[i] /= A[i][i];
      }
      return true;
    };
    _execute(approx_code, 0, get_num_threads(N), N);

    
    
    bool close_enough = _execute(check_closeness, 0, get_num_threads(N), N);
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


#endif
