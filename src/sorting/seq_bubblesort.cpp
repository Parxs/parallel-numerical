#include "util.h"
#include "array.h"
#include "sorting.h"
#include "bsort/seq_bubblesort.h"

using namespace sequential;

/**
 * @brief Measures the time sorting a list takes.
 * 
 * @param in_list list to be sorted
 * @param out_list place for the sorted list
 * @param N dimension
 * @return time elapsed
 * */
unsigned long sort_List(int* in_list, int* out_list, long N){
	// copy to target
	copy(in_list, out_list, N);
	
	unsigned long start = time_ms();
	
	bubblesort(out_list, N);
	
	
	return time_ms()-start;
}
