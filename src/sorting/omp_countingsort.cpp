#include "util.h"
#include "array.h"
#include "sorting.h"

// flags
bool NEG_VALUE_ENCOUNTERED = false;
bool BIGGER_THAN_MAX_ENCOUNTERED = false;
bool WRONG_PREFIX_SUM = false;

using namespace std;

template <typename T>
void _prefix_Sum(T* array, long length){
  int i;
  // reduce
  for(i=2; i<=length; i<<=1){
    // i-1 because the list starts at 0 not 1
    #pragma omp parallel for 
    for(int j=(i-1); j<length; j+=i){
      array[j] += array[j - i/2];
    }
  }
  array[length-1] = 0;
  //downsweep
  // get i to the last value that was used for the reduction
  for(i>>=1; i>1; i>>=1){
    // i-1 because the list starts at 0 not 1
    #pragma omp parallel for 
    for(int j=(i-1); j<length; j+=i){
      
      T tmp = array[j - i/2];
      array[j - i/2] = array[j];
      array[j] += tmp;
    }
  }
}

template <typename T>
void seq_calculate_Histogram(T* list, long N, long* histogram){
  T val;
  long i;
  for(i=0; i<N; i++){
    val = list[i];
    histogram[val]++;
  }
}


template <typename T>
void _calculate_Histogram2(T* list, long N, long* histogram){
  T val;
  long i;
  
  /*
   * There are three ways to handle calculating the histogram in parallel
   * 1) is by using atomic operations or a critical section
   * 2) by giving every thread a private histogram and then merging them in a critical section
   * 3) by using OpenMP 4.5
   * 
   * the first option is probably the slowest but has the smallest memory requirement
   * second and third both need enough memory to hold MAX*work_size
   * 
   * */
#pragma omp parallel shared(list, histogram)
  {
    long *local_histo = new long[MAX]();
    #pragma omp for nowait
    for(i=0; i<N; i++){
      val = list[i];
      local_histo[val]++;
    }
    #pragma omp critical
    {
      for(i=0; i<MAX; i++){
        histogram[i] += local_histo[i];
      }
    }
    delete[] local_histo;
  }
}

template <typename T>
void _calculate_Histogram(T* list, long N, long* histogram){
  T val;
  long i;
  for(i=0; i<N; i++){
    val = list[i];
    // offset by one to allow easier implementation of exclude prefix-sum
    histogram[val]++;
  }
}

template <typename T>
void _sort(T* in_list, T* out_list, long N, long *histogram){
  /* for the same reason as in c11_countingsort.cpp there is no point in
   * parallelizing this part as every thread would have to run through
   * the whole list at least once which is as often as it would happen 
   * when done sequentially.
   * The only case in which a speedup could be possible is if moving
   * the elements is the most time consuming part
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
 * @brief Measures the time sorting a list takes.
 * 
 * @param in_list list to be sorted
 * @param out_list place for the sorted list
 * @param N dimension
 * @return  time elapsed, if 0 is returned it is also possible that an
 *          element was bigger than the max value or one of
 *          the elements is smaller than 0
 **/
unsigned long sort_List(int* in_list, int* out_list, long N){
	long* histogram;
  unsigned long start, end;
  
  if(!_initialize(in_list, N)){
    return 0lu;
  }
  
  // initialize with 0
  // and include offset
  int size =1;
  while(size<MAX){
    size <<=1;
  }
  histogram = new long[size]();
  
  start = time_ms();
	
  _calculate_Histogram(in_list, N, histogram);
  
  
  _prefix_Sum(histogram, size);
  // last element of the prefixsum histogram will not be used!
  /*
  if(_prefix_Sum(histogram, MAX) != N){
    WRONG_PREFIX_SUM = true;
#ifdef DEBUG
cout << endl << "> ERROR: prefixsum has the wrong result - possibly at least one element occurs too often" << endl << endl;
#endif
    delete[](histogram);
    return 0lu;
  }*/
  
	_sort(in_list, out_list, N, histogram);
	
  end = time_ms();
  
  delete[](histogram);
  
  return end-start;
} 
