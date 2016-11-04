#include "util.h"
#include "array.h"
#include "sorting.h"

 
struct Container { int val; long index; };

struct Container _init_Container(int val, long index);

#pragma omp declare reduction(minimum : struct Container : \
    omp_out = omp_in.val < omp_out.val ? omp_in : omp_out) \
    initializer (omp_priv=_init_Container(MAX, 0))

struct Container _init_Container(int val, long index){
  struct Container cont;
  cont.val = val;
  cont.index = index;
  return cont;
}


// does not allow negative numbers
void selectionsort(int* list, long N){
  long i, j, start;
  int tmp;
  struct Container min;
  for(i=0; i<N; i++){
    min.val = list[i];
    min.index = i;
    start = i+1;
    #pragma omp parallel for private(tmp) reduction(minimum:min)
    for(j=start; j<N; j++){
      tmp = list[j];
      if(tmp < min.val){
        min.val = tmp;
        min.index = j;
      }
    }
    swap(list, i, min.index);
  }
}

/**
 * @brief Sorts a given list with a given length into a new array.
 * 
 * 
 * @return  time elapsed
 * */
unsigned long sort_List(int* in_list, int* out_list, long N){
  unsigned long start, end;
  
  copy(in_list, out_list, N);
  
  
  start = time_ms();
  
	selectionsort(out_list, N);
	
  end = time_ms();
  
  
  return end-start;
} 
