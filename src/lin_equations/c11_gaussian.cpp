#include "util.h"
#include "lin_equations.h"
#include "c11_gaussian.h"

#include <iostream>
#include <future>
#include <thread>
#include <vector>

using namespace std;

void _execute(std::function<bool (long, long, long)> func, long start, int num_tasks, long num_elems){
  
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
    futures.push_back(async(launch::async, func, start-1, start_task, end_task)); 
    
    start_task = end_task;
    end_task = start_task + chunksize;
  }
  
  for(future<bool> &f: futures){
    f.get();
  }
}


unsigned long solve(VALUE **A, VALUE *b, VALUE *y, VALUE *x, long N){
 unsigned long start_time = time_ms(), time;
  
  c11_Gaussian(A, b, y, N);
  
  
  // can only be done sequentially
  do_BackSub(A, y, x, N);
  
  time = time_ms()-start_time;
  
  
  
  
  return time;
}
