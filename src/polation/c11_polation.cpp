#include "c11_polation.h"
#include "polation.h"
#include "util.h"


#include <future>
#include <thread>
#include <vector>

using namespace std;

void _execute(std::function<bool (long, long)> func, long start, int num_tasks, long num_elems){
  
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
    f.get();
  }
}


unsigned long polate(VALUE *x_values, VALUE *y_values, long N, VALUE x, VALUE **Q){
  unsigned long start = time_ms();
  
  c11_polation(x_values, y_values, N, x, Q);
  
  
  return time_ms()-start;
}
