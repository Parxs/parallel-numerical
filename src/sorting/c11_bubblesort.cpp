#include <future>
#include <thread>
#include <vector>
#include <iostream>

#include "util.h"
#include "array.h"
#include "sorting.h"

using namespace std;

// C++11 parallel variables
unsigned int num_workers;

bool _execute(std::function<bool (long, long, bool)> func, int num_workers, long num_elems, long start){
  vector<future<bool>> futures;
  int t, extra; // extra will at most be as big as num_workers-1
  long chunksize, start_task, end_task;
  bool start_at_even;
  
  // num_elems is decrease by 1 because the elements are accessed in 
  // pairs (element+succesor) so the last element may never be accessed
  // as it has no succesor
  // (it can however be accessed as a succesor!)
  num_elems--;
  
  // split work
  chunksize = num_elems / num_workers;
	extra = num_elems % num_workers; 
	start_task = start;
  end_task = chunksize; 
  
  //calculate whether even-odd or odd-even pairs are currently accessed
  start_at_even = start%2==0;
	
    
  // run threads
  for(t=0; t<num_workers; t++){
    // test whether extra work still needs to be done
    if(t < extra){
      end_task++;
    }
    
    futures.push_back(async(launch::async, func, start_task, end_task, start_at_even)); 
    
    start_task = end_task ;
    end_task = start_task + chunksize;
  }
  
  bool swapped = false;
  for(future<bool> &f: futures){
    swapped = f.get() || swapped ;
  }
  
  return swapped;
}


template <typename T>
void bubblesort(T* list, long N){
  // parallelization is similar to even-odd-sort
  // the only diffrence between even-odd and odd-even is where we start
  auto code = [&list](long start_task, long end_task, bool start_at_even){
    long i;
    bool swapped = false; 
    
    /* as not all elements are accessed there could be a potential issue
     * if the task is not split up in a perfect way as then some pairs
     * may not be accessed or possibly wrong ones are 
     * e.g. even-odd pairs, end_task is 3, start_task is 0
     * 0&1, 2&3 would be the pairs accessed by this task
     * but the next task then would look like this:
     * start_task is 3, end_task is 6
     * 3&4, 5&6 and then we would have already a race condition 
     * (2&3 and 3&4) and also we switched midways from even-odd to 
     * odd-even pairs.
     * 
     * therefor if the eveness of the start_task doesn't match 
     * start_at_even then we have to increase it by one.
     * */
    if((start_at_even && start_task%2==1 )||(!start_at_even && start_task%2==0)){
      start_task++;
    }
    
    for(i=start_task; i<end_task; i+=2){
      if(list[i] > list[i+1]){
        swap(list, i, i+1);
        swapped = true;
      }
    }
    return swapped;
  };
  
  bool swapped = true;
  while(swapped){
    swapped = _execute(code, num_workers, N, 0);
    swapped = _execute(code, num_workers, N, 1) || swapped;
    
    if(!swapped){
      break;
    }
  }
}


unsigned long sort_List(int* in_list, int* out_list, long N){
	num_workers = get_num_threads(N);  
  
  // copy to target
	copy_1D(in_list, out_list, N);
	
	unsigned long start = time_ms();

	bubblesort(out_list, N);
	
	
	return time_ms()-start;
}
