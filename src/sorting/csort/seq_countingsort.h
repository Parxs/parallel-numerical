#ifndef SEQ_COUNTINGSORT_H
#define SEQ_COUNTINGSORT_H

#include <iostream>

namespace sequential{
  
// flags
bool NEG_VALUE_ENCOUNTERED = false;
bool BIGGER_THAN_MAX_ENCOUNTERED = false;
bool WRONG_PREFIX_SUM = false;

/**
 * @brief Calculates the prefixsum of a array in place with Blelloch scan.
 * @param array whose prefixsum is wanted
 * @param length size of array
 **/
template <typename T>
void _prefix_Sum(T* array,  long length){
  long i;
  // reduce
  for(i=2; i<=length; i<<=1){
    // i-1 because the list starts at 0 not 1
    for(long j=(i-1); j<length; j+=i){
      array[j] += array[j - i/2];
    }
  }
  array[length-1] = 0;
  //downsweep
  // get i to the last value that was used for the reduction
  for(i>>=1; i>1; i>>=1){
    // i-1 because the list starts at 0 not 1 
    for(long j=(i-1); j<length; j+=i){
      
      T tmp = array[j - i/2];
      array[j - i/2] = array[j];
      array[j] += tmp;
    }
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
}



#endif
