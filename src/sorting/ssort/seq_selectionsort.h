#ifndef SEQ_SELECTIONSORT_H
#define SEQ_SELECTIONSORT_H


namespace sequential{

/**
 * @brief Applies selectionsort to a given list.
 * @param list list to be sorted
 * @param N size of list
 **/
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
}


#endif
