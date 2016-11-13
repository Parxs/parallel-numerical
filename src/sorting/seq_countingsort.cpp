
#include "util.h"
#include "array.h"
#include "sorting.h"

// flags
bool NEG_VALUE_ENCOUNTERED = false;
bool BIGGER_THAN_MAX_ENCOUNTERED = false;
bool WRONG_PREFIX_SUM = false;

using namespace std;

template <typename T>
T _prefix_Sum(T* array,  long length){
	T sum = array[0], tmp;
  
  long i;
  array[0] = 0;
	for (i=1; i<length; i++){
		tmp = array[i];
    array[i] = sum;
		sum += tmp;
	}
  
  return sum;
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
