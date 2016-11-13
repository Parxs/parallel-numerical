#ifndef SEQ_BUBBLESORT_H
#define SEQ_BUBBELEORT_H

namespace sequential{
  
/**
 * @brief Sorts the given list with bubblesort in place.
 * @param list to be sorted
 * @param N size of list
 **/
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
}


#endif
