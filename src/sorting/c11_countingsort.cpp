
#include <iostream>
#include <future>
#include <thread>
#include <vector>

#include "util.h"
#include "array.h"
#include "sorting.h"
#include "csort/c11_countingsort.h"

using namespace cEleven;


/**
 * @brief Measures the time sorting a list takes.
 * 
 * If the given list is not fit for the algorithm then flags will be
 * set which specify what was the problem.
 * 
 * @param in_list list to be sorted
 * @param out_list place for the sorted list
 * @param N dimension
 * @return  time elapsed, if 0 is returned it is also possible that an
 *          element was bigger than the max value or one of
 *          the elements is smaller than 0
 **/
unsigned long sort_List(int* in_list, int* out_list, long N){
  unsigned long start, end;
  
  if(!initialize(in_list, N)){
    return 0lu;
  }

  start = time_ms();
	
  countingsort(in_list, out_list, N);
	
  end = time_ms();
  
  
  
  return end-start;
} 
