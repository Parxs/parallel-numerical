#include "util.h"
#include "array.h"
#include "sorting.h"

template <typename T>
void bubblesort(T* list, long N){
  bool swapped = true;
  long i;
  // similar to odd-even sort
  //#pragma omp parallel
  { 
  while(swapped){
    swapped = false;
      // even-odd
    #pragma omp parallel for
    for(i=0; i<N; i+=2){
      if(list[i] > list[i+1]){
        swap(list, i, i+1);
        swapped = true;
      }
    }
    // odd-even
    #pragma omp parallel for
    for(i=1; i<N; i+=2){
      if(list[i]>list[i+1]){
        swap(list, i, i+1);
        swapped = true;
      }
    }
    if(!swapped){
      break;
    }
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
