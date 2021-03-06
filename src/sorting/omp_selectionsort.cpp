#include "util.h"
#include "array.h"
#include "sorting.h"
#include "ssort/omp_selectionsort.h"

using namespace openMP;

/**
 * @brief Measures the time sorting a list takes.
 * 
 * @param in_list list to be sorted
 * @param out_list place for the sorted list
 * @param N dimension
 * @return time elapsed
 * */
unsigned long sort_List(int* in_list, int* out_list, long N){
  unsigned long start, end;
  
  copy(in_list, out_list, N);
  
  
  start = time_ms();
  
	selectionsort(out_list, N);
	
  end = time_ms();
  
  
  return end-start;
} 
