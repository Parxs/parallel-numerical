#ifndef OMP_QUICKSORT_H
#define OMP_QUICKSORT_H


namespace openMP{


/**
 * @brief Calculates a pivot for a given list part.
 * @param list the list
 * @param lo start of part
 * @param hi end of part
 * @return index of pivot
 **/
template <typename T>
T _pivot(T* list, long lo, long hi){
  
  return list[(lo+hi)/2];
}

/**
 * @brief Partition part of list into smaller and bigger than a pivot
 * @param list to be partitioned
 * @param lo start of part
 * @param hi end of part
 * @return
 **/
template <typename T>
long _partition(T* list, long lo, long hi){
  long i=lo, j=hi;
  T pivot = _pivot(list, lo, hi);
  
  while(i<=j && j<=hi){
    while(i<=hi && list[i]<pivot){
      i++;
    } 
    while(j<=hi && list[j]>pivot){
      j--;
    }
    
    // the following can probably be solve in a different way as
    // the first condition is always checked in the while loop
    // but those solutions need either a bigger refactoring, are 
    // harder to understand or even slower
    if(i>j){
      break;
    }else if(i<j){
      swap(list, i, j);
    } 
    
    i++;
    j--;
  }
  
  return i;
}

/**
 * @brief Applies quicksort to a given part of the list.
 * @param list list whose part should be quicksorted
 * @param lo start of the part
 * @param hi end of the part
 **/
template <typename T>
void quicksort(T* list, long lo, long hi){
  long index = _partition(list, lo, hi);
  
#pragma omp parallel sections
{
  #pragma omp section
  {
    if(lo < index-1){
      quicksort(list, lo, index-1);
    }
  }
  #pragma omp section
  {
    if(index < hi){
      quicksort(list, index, hi);
    }
  }
} 
}

/**
 * @brief Applies quicksort to a given list.
 * @param list the list that should be ordered
 * @param N size of the list
 **/
template <typename T>
void quicksort(T* list, long N){
  quicksort(list, 0, N-1);
}
}


#endif
