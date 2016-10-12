#include "util.h"
#include "array.h"
#include "sorting.h"

template <typename T>
void selectionsort(T* list, long N){
  long i, j, index_min, start;
  T min, tmp;
  for(i=0; i<N; i++){
    min = list[i];
    index_min = i;
    start = i+1;
    for(j=start; j<N; j++){
      tmp = list[j];
      if(tmp < min){
        min = tmp;
        index_min = j;
      }
    }
    swap(list, i, index_min);
  }
}



/**
 * @brief Sorts a given list with a given length into a new array.
 * 
 * @return  time elapsed, if 0 is returned it is also possible that an
 *          element was bigger than the maximal possible value
 * */
unsigned long sort_List(int* in_list, int* out_list, long N){
	
  unsigned long start;
 
	
  copy_1D(in_list, out_list, N);
  
  start = time_ms();
  
  selectionsort(out_list, N);
  
  return time_ms()-start;
}
