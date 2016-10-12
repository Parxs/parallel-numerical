#include "util.h"
#include "lin_equations.h"
#include "c11_jacobi.h"


#ifndef MAX_ITERATIONS
#define MAX_ITERATIONS 10000
#endif

#include <iostream>
#include <future>
#include <thread>
#include <vector>

using namespace std;

bool _execute(std::function<bool (long, long)> func, long start, int num_tasks, long num_elems){
  
  vector<future<bool>> futures;
  int t, extra; // extra will at most be as big as num_tasks-1
  long chunksize, start_task, end_task;
  
  // split work
  chunksize = num_elems / num_tasks;
	extra = num_elems % num_tasks; 
	start_task = start;
	end_task = start+chunksize;
  
  // run threads
  for(t=0; t<num_tasks; t++){
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


unsigned long solve(VALUE **A, VALUE *b, VALUE *y, VALUE *x, long N){
  unsigned long start = time_ms(), end;
  
  c11_jacobi(A, b, x, EPSILON, MAX_ITERATIONS, N);
  
  
  end = time_ms();
  
  return end-start;
}
