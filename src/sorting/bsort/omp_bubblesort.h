#ifndef OMP_BUBBLESORT_H
#define OMP_BUBBLESORT_H

namespace openMP{

/**
 * @brief Sorts the given list with bubblesort in place.
 * @param list to be sorted
 * @param N size of list
 **/
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
}


#endif
