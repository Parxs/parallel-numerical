#ifndef C11_SELECTIONSORT_H
#define C11_SELECTIONSORT_H

#include <future>
#include <thread>
#include <vector>

namespace cEleven{



#ifndef SEQ_THRESHOLD
#define SEQ_THRESHOLD 1024
#endif

using namespace std;

struct Container { int val; long index; };

struct Container _init_Container(int val, long index);
#pragma omp declare reduction(minimum : struct Container : \
    omp_out = omp_in.val < omp_out.val ? omp_in : omp_out) \
    initializer (omp_priv=_init_Container(MAX, 0))

struct Container _init_Container(int val, long index){
  struct Container cont;
  cont.val = val;
  cont.index = index;
  return cont;
}


// C++11 parallel variables
unsigned int num_workers;

 /**
 * @brief Executes the given function in parallel.
 * @param func function to be done in parallel
 * @param num_elems number of elements that should be run through the func
 * @param start index of first element that should be run through the func
 **/
long _execute(function<struct Container (long, long)> func, long num_elems, long start){
  using namespace std;
  if(num_elems<num_workers || num_elems<SEQ_THRESHOLD){
    // if there are more workers than elements then some task would be 
    // done more than once - so it is better executed sequentally
    // similar problem is that if N is rather small it is better to do
    // the searching for the smallest element sequentally
    return func(start, start+num_elems).index;
  }
  
  vector<future<struct Container>> futures;
  // extra will at most be as big as num_workers-1
  long t, extra, chunksize, start_task, end_task;
  
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
    
    futures.push_back(async(launch::async, func, start_task, end_task)); 
    
    start_task = end_task ;
    end_task = start_task + chunksize;
  }
  
  
  struct Container min = _init_Container(MAX, 0), tmp;
  for(future<struct Container> &f: futures){
    tmp = f.get();
    if(tmp.val < min.val){
      min = tmp;
    }
  }
  
  
  return min.index;
}


// does not allow negative numbers
/**
 * @brief Applies selectionsort to a given list.
 * @param list list to be sorted
 * @param N size of list
 **/
void selectionsort(int* list, long N){
  long i, index;
  
  auto code = [&list](long start_task, long end_task){
    struct Container min;
    long i;
    int tmp;
    min.val = list[start_task];
    
    min.index = start_task;
    
    
    start_task++;
    for(i=start_task; i<end_task; i++){
      tmp = list[i];
      if(tmp < min.val){
        min.val = tmp;
        min.index = i;
      }
    }
    
    
    
    return min;
  };
  
  
  for(i=0; i<N; i++){
    // the searchspace gets smaller the bigger i is as then the first
    // i elements are already in order
    index = _execute(code, N-i, i);
    swap(list, i, index);
  }
}
  
}


#endif
