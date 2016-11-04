#include "util.h"
#include "array.h"
#include "sorting.h"

template <typename T>
void bubblesort(T* list, long N){
	bool swapped;
  long i, j;
	for(i=0; i<N; i++){
		swapped = false;
		
		// after the ith iterations the last i elements will be 
		// sorted therefore no need to traverse them
		// this bubblesort ignores the first element and compares
		// the current element with the element before
		long end = (N-i);
		for(j=1; j<end; j++){
			if(list[j-1] > list[j]){
				swap(list, j-1, j);
				swapped = true;
			}
			
		}
		if (!swapped){
			break;
		}
	}
}


unsigned long sort_List(int* in_list, int* out_list, long N){
	// copy to target
	copy(in_list, out_list, N);
	
	unsigned long start = time_ms();
	
	bubblesort(out_list, N);
	
	
	return time_ms()-start;
}
