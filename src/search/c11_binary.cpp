#include "util.h"
#include "array.h"
#include "search.h"

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
  long start, end, chunk_size, extra;
  vector<future<boost::optional<long>>> futures;
  
  chunk_size = N/num_workers;
  extra = N%num_workers;
  
  for(t=0; t<num_workers; t++){
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
    
    // using lambda to call binarysearch as otherwise wouldn't know 
    // which instantiation to use of the template
    auto code = [&list, start, end, target]{
      return binarysearch(list, start, end, target);
    };
    
    futures.push_back(async(launch::async, code));
  }
  
  
  boost::optional<long> result, tmp;
  for(future<boost::optional<long>> &f: futures){
    // as soon as a result is gotten we do not care anymore whether 
    // there are more results - those are easy to find just by
    // increasing/decreasing *result until an element is reached which 
    // is not equal to the target
    // (binary search does not necessarily find the first occurrence!
    // e.g target=1 & {1,1,2} -> the second 1 is found not the first
    if(!result){
      result = f.get();
    }else{
      f.get();
    }
  }
  
  return result;
}


long search(int* list, long N, int find, long *index){
  num_workers = get_num_threads(N);
  
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

