#include <future>
#include <thread>
#include <vector>
#include <iostream>

#include "util.h"
#include "array.h"
#include "sorting.h"

using namespace std;

template <typename T>
T _pivot(T* list, long lo, long hi){
  
  return list[(lo+hi)/2];
}

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

//template <typename T>
bool quicksort(int* list, long lo, long hi){
  vector<future<bool>> futures;
  long index = _partition(list, lo, hi);
  
  if(lo < index-1){
    futures.push_back(async(launch::async, quicksort, list, lo, index-1));
  }
  if(index < hi){
    futures.push_back(async(launch::async, quicksort, list, index, hi));
  } 
  
  if(!futures.empty()){
  for(future<bool> &f: futures){
    f.get();
  }
}
  return true;
}

template <typename T>
void quicksort(T* list, long N){
  quicksort(list, 0, N-1);
}

unsigned long sort_List(int* in_list, int* out_list, long N){
	// copy to target
	copy_1D(in_list, out_list, N);
  
	unsigned long start = time_ms();
	
	quicksort(out_list, N);
	
	
	return time_ms()-start;
}
