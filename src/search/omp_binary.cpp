#include "util.h"
#include "array.h"
#include "search.h"

#include <omp.h>

#include <boost/optional.hpp>
#include <iostream>
#include <future>
#include <thread>
#include <vector>

using namespace std;

unsigned int num_workers;

template <typename T>
boost::optional<long> binary(T* list, long start, long end, T target){
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
boost::optional<long> binarysearch(T* list, long start, long end, T target){
  return binary(list, start, end-1, target);
}

template <typename T>
boost::optional<long> binarysearch(T* list, long N, T target){
  unsigned int t;
  long chunk_size, extra;
  
  chunk_size = N/num_workers;
  extra = N%num_workers;
  
  
  long index = -1;
  // index is used for the reduction so that a default reduction can be used
  #pragma omp parallel for shared(list) reduction(max:index)
  for(t=0; t<num_workers; t++){
    long start, end;
    start = chunk_size*t;
    if(t<extra){
      // offset by the extra work already done which is conveniently the
      // id of the current thread
      start += t;
    }else{
      // offset by extra because this work is done by the threads before
      start += extra;
    }
    end = start+chunk_size;
    
    boost::optional<long> tmp = binarysearch(list, start, end, target);
    
    if(tmp){
      index = *tmp;
    }else{
      index = -1;
    }
  }
  
  boost::optional<long> result;
  result = index;
  return result;
}


long search(int* list, long N, int find, long *index){
  num_workers = omp_get_num_threads();
  
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

