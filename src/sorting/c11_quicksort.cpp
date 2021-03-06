#include <iostream>

#include "util.h"
#include "array.h"
#include "sorting.h"
#include "qsort/c11_quicksort.h"

using namespace cEleven;



/**
 * @brief Measures the time sorting a list takes.
 * 
 * @param in_list list to be sorted
 * @param out_list place for the sorted list
 * @param N dimension
 * @return  time elapsed
 * */
unsigned long sort_List(int* in_list, int* out_list, long N){
	// copy to target
	copy(in_list, out_list, N);
  
	unsigned long start = time_ms();
	
	quicksort(out_list, N);
	
	
	return time_ms()-start;
}
