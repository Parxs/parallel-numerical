#include <future>
#include <thread>
#include <vector>
#include <iostream>

#include "util.h"
#include "array.h"
#include "sorting.h"


using namespace std;

// flags
bool NEG_VALUE_ENCOUNTERED = false;
bool BIGGER_THAN_MAX_ENCOUNTERED = false;
bool WRONG_PREFIX_SUM = false;

// C++11 parallel variables
unsigned int num_workers;


void _execute(function<bool (long, long, int)> func, int num_tasks, long num_elems){
  vector<future<bool>> futures;
  int t, extra; // extra will at most be as big as num_tasks-1
  long chunksize, start_task, end_task;
  
  // split work
  chunksize = num_elems / num_tasks;
	extra = num_elems % num_tasks; 
	start_task = 0;
	end_task = chunksize;
  
  // run threads
  for(t=0; t<num_tasks; t++){
    // test whether extra work still needs to be done
    if(t < extra){
      end_task++;
    }
    
    futures.push_back(async(launch::async, func, start_task, end_task, t)); 
    
    start_task = end_task;
    end_task = start_task + chunksize;
  }
  
  for(future<bool> &f: futures){
    f.get();
  }
}


template <typename T>
T _prefix_Sum(T* array,  long length){
	T sum = 0, tmp;
  long i;
	for (i=0; i<length; i++){
		tmp = array[i];
		if(i==0){
			array[i] = 0;
		}else{
			array[i] = sum;
		}
		sum += tmp;
	}
  return sum;
}


template <typename T>
void _calculate_Histogram(T* list, long N, long* histogram){
  
  long i;
  
  // allocate memory for local histograms
  long** local_histo = new long*[num_workers];
  for(i=0; i<num_workers; i++){
    // initialize with 0
    local_histo[i] = new long[MAX]();
  }
  
  // thread codes
  auto histo_code = [&list, &local_histo](long start_task, long end_task, int thid){
    long i;
    T val;
    for(i=start_task; i<end_task; i++){
        val = list[i];
        local_histo[thid][val]++; 
    }
    return true;
  };
  
  auto reduce_code = [&histogram, &local_histo](long start_task, long end_task, int thid){
    long i, j;
    long sum;
    for(i=start_task; i<end_task; i++){
      sum = 0;
      for(j=0; j<num_workers; j++){
        sum += local_histo[j][i];
      }
      histogram[i] = sum;
    }
    return true;
  };
 
  // execute on threads
  _execute(histo_code, num_workers, N);
  _execute(reduce_code, num_workers, MAX);
  
  // deallocate
  for(i=0; i<num_workers; i++){
    delete [] local_histo[i];
  }
  delete [] local_histo;
}


template <typename T>
void _slow_sort(T* in_list, T* out_list, unsigned long N, long *histogram){
  /* there is not a really good way to parallelize this code as setting 
   * in every case you have to let every thread run through the whole
   * array so one thread alone can do the same
   * the only possible speedup is in the case that moving the values
   * from the unsorted to the sorted list is rather slow
   * in that case letting more than one thread move elements could be
   * a good idea
   * however that will only work with very big numbers and when the 
   * numbers are not clustered together as then one thread would have to
   * do far more work.
   * 
   * therefore in almost all cases this will be slower than a sequential
   * code
   * */
  auto code = [&in_list, &out_list, &histogram, N](long start_task, long end_task, int thid){
    long i, pos;
    T elem;
    for(i=0; i<N; i++){
      elem = in_list[i];
      if(start_task <= elem &&  end_task >= elem){
        pos = histogram[elem];
        
        out_list[pos] = elem;
        
        histogram[elem]++;
      }
    }
    return true;
  };
  
  
  
  _execute(code, num_workers, MAX);
  
}


template <typename T>
void _sort(T* in_list, T* out_list, long N, long *histogram){
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

bool _initialize(int* list, long N){
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
 * @brief Sorts a given list with a given length into a new array.
 * 
 * 
 * @return  time elapsed, if 0 is returned it is also possible that an
 *          element was bigger than the max value or one of
 *          the elements is smaller than 0
 * */
unsigned long sort_List(int* in_list, int* out_list, long N){
	long *histogram;
  unsigned long start, end;
  
  if(!_initialize(in_list, N)){
    return 0lu;
  }
  
  num_workers = get_num_threads(N);
  
  // initialize with 0
  histogram = new long[MAX]();

  start = time_ms();
	
  _calculate_Histogram(in_list, N, histogram);
  
  if(_prefix_Sum(histogram, MAX) != N){
    WRONG_PREFIX_SUM = true;
#ifdef DEBUG
cout << endl << "> ERROR: prefixsum has the wrong result - possibly at least one element occurs too often" << endl << endl;
#endif
    delete[](histogram);
    return 0lu; 
  }
	
	_sort(in_list, out_list, N, histogram);
	
  end = time_ms();
  
  delete[](histogram);
  
  return end-start;
} 
