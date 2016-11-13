#ifndef C11_COUNTINGSORT_H
#define C11_COUNTINGSORT_H

#include <future>
#include <thread>
#include <vector>

#include "util.h"
#include "array.h"

namespace cEleven{


// flags
bool NEG_VALUE_ENCOUNTERED = false;
bool BIGGER_THAN_MAX_ENCOUNTERED = false;
bool WRONG_PREFIX_SUM = false;

/**
 * @brief Executes the given function in parallel.
 * @param func function to be done in parallel
 * @param i current i
 * @param start index of first element that should be run through the func
 * @param num_workers how high the parallelism should be
 * @param num_elems number of elements that should be run through the func
 **/
void _execute(std::function<void (long, long, long)> func, long i, int num_workers, long num_elems){
  using namespace std;
  vector<future<void>> futures;
  int t, extra; // extra will at most be as big as num_workers-1
  long chunksize, start_task, end_task;
  
  // split work
  chunksize = num_elems / num_workers;
	extra = num_elems % num_workers; 
  // i-1 because the list starts at 0 not 1
	start_task = i-1;
	end_task = start_task+chunksize*i;
  // run threads
  for(t=0; t<num_workers; t++){
    // test whether extra work still needs to be done
    if(t < extra){
      end_task +=i;
    }
    
    // start-1 to tell the function which index the current pivot has
    futures.push_back(async(launch::async, func, i, start_task, end_task)); 
    
    start_task = end_task;
    end_task = start_task + chunksize*i;
  }
  
  for(future<void> &f: futures){
    f.get();
  }
}

/**
 * @brief Calculates the prefixsum of a array in place with Blelloch scan.
 * @param array whose prefixsum is wanted
 * @param length size of array
 **/
template <typename T>
void _prefix_Sum(T* array,  long length){
	long i;
  
  auto reduce_code = [array](long i, long start, long end){
    for(long j=start; j<end; j+=i){
       array[j] += array[j - i/2];
    }
  };
  
  auto downsweep_code = [array](long i, long start, long end){
    for(long j=start; j<end; j+=i){
       T tmp = array[j - i/2];
      array[j - i/2] = array[j];
      array[j] += tmp;
    }
  };
  
  
  // reduce
  for(i=2; i<=length; i<<=1){
    long num_elem = length/i;
    _execute(reduce_code, i, get_num_threads(num_elem), num_elem);
  }
  array[length-1] = 0;
  //downsweep
  // get i to the last value that was used for the reduction
  for(i>>=1; i>1; i>>=1){
    long num_elem = length/i;
    _execute(downsweep_code, i, get_num_threads(num_elem), num_elem);
  }
}


template <typename T>
void _calculate_Histogram(T* list, long N, long* histogram){
  T val;
  long i;
  for(i=0; i<N; i++){
    val = list[i];
    histogram[val]++;
  }
}

/**
 * @brief Sorts the given list.
 * @param in_list list to be sorted
 * @param out_list place for the sorted list
 * @param N size of list
 * @param histogram histogram of the in_list
 **/
template <typename T>
void _sort(T* in_list, T* out_list, long N, long *histogram){
  /* there is not a really good way to parallelize this code as 
   * in every case you have to let every thread run through the whole
   * array so one thread alone can do the same
   * the only possible speedup is in the case that moving the values
   * from the unsorted to the sorted list is rather slow
   * in that case letting more than one thread move elements could be
   * a good idea
   * however that will only work with very big numbers and when the 
   * numbers are not clustered together as then one thread would have to
   * do far more work.
   * */
  long i, pos;
  T elem;
  
	for(i=0; i<N; i++){
		//get position
		elem = in_list[i];
		pos = histogram[elem];
		
		//put into correct position
		out_list[pos] = elem;
		
		//increase position of current elem
		histogram[elem]++;
	}
}

/**
 * @brief Initializes counting sort
 * @param list list to be sorted
 * @param N size of list
 * @return whether sorting that list can be done
 **/
bool initialize(int* list, long N){
  using namespace std;
  // reset flags
  NEG_VALUE_ENCOUNTERED = false;
  BIGGER_THAN_MAX_ENCOUNTERED = false;
  WRONG_PREFIX_SUM = false;
  
  // test for possible issues
  long i;
  for(i=0; i<N; i++){
    if(list[i]>MAX){
      BIGGER_THAN_MAX_ENCOUNTERED = true;
#ifdef DEBUG
cout << endl << "> ERROR: element (i:" << i <<") is bigger than the maximal possible value" << endl << endl;
#endif
      return false;
    }
    if(list[i] < 0){
      NEG_VALUE_ENCOUNTERED = true;
#ifdef DEBUG
cout << endl << "> ERROR: element (i:" << i <<") is negative" << endl << endl;
#endif
      return false;
    } 
  }
  return true;
}

/**
 * @brief Uses Countingsort to sort a list
 * @param in_list list to be sorted
 * @param out_list place for the sorted list
 * @param N dimension
 **/
void countingsort(int* in_list, int* out_list, long N){
  long *histogram;
  // initialize with 0
  int size =1;
  while(size<=MAX){
    size <<=1;
  }
  histogram = new long[size]();
  
  _calculate_Histogram(in_list, N, histogram);
  
  _prefix_Sum(histogram, size);
	
	_sort(in_list, out_list, N, histogram);
  
  delete[](histogram);
}

}

#endif
