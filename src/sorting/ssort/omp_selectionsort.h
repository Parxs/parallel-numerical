#ifndef OMP_SELECTIONSORT_H
#define OMP_SELECTIONSORT_H


namespace openMP{

 
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
/**
 * @brief Applies selectionsort to a given list.
 * @param list list to be sorted
 * @param N size of list
 **/
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
}


#endif
