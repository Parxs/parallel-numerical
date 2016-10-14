#include "util.h"
#include "array.h"
#include "search.h"

#include <boost/optional.hpp>
#include <iostream>

using namespace std;

template <typename T>
boost::optional<long> binarysearch(T* list, long start, long end, T target){
  boost::optional<long> index;
  long mid = end/2;
  while(start<=end){
    mid = (end-start)/2+start;
    T elem = list[mid];
    if(elem>target){
      end = mid-1;
    }else if(elem<target){
      start = mid+1;
    }else if(elem==target){
      //element found!
      index = mid;
      return index;
    }
  }
  
  
  
  return index;
}

template <typename T>
boost::optional<long> binarysearch(T* list, long N, T target){
  return binarysearch(list, 0, N-1, target);
  // N-1 because function assumes that list[end] is valid
}



long search(int* list, long N, int find, long *index){
  long start_time, exec_time;
  boost::optional<long> pos;
  
  start_time = time_ms();
  pos =  binarysearch(list, N, find);
  exec_time = time_ms()-start_time;
  
  if(!pos){
    *index = -1;
    return exec_time;
  }else{
    *index = *pos;
  }
  return exec_time;
}
