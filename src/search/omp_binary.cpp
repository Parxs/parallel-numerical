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
  unsigned int t;
  long start, end, chunk_size, extra;
  vector<future<boost::optional<long>>> futures;
  
  start = 0;
  // N-1 because function assumes that list[end] is valid
  chunk_size = (N-1)/num_workers;
  extra = (N-1)%num_workers;
  end = chunk_size;
  
  
  
  #pragma omp parallel for
  for(t=0; t<=num_workers; t++){
    if(t<extra){
      end++;
    }
    // using lambda to call binarysearch as otherwise wouldn't know 
    // which instantiation to use of the template
    auto code = [&list, start, end, target]{
      return binarysearch(list, start, end, target);
    };
    
    futures.push_back(async(launch::async, code));
    
    // +1 because the function does not want the size of the array but 
    // instead the last index which should be accessed!
    // so the next task must start one step higher
    start = end+1;
    end += 1+chunk_size;
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


long search(int* list, long N, int find){
  num_workers = omp_get_num_threads();
  
  long start_time, exec_time;
  boost::optional<long> index;
  
  start_time = time_ms();
  index =  binarysearch(list, N, find);
  exec_time = time_ms()-start_time;
  
  if(!index){
    cout << "Element was not found\n" << endl;
#ifdef DEBUG
  long i;
  bool found = false;
  for(i=0; i<N; i++){
    if(list[i]==find){
      found = true; 
      break;
    }
  }
  if(found){
    cout << "> ERROR: list holds element!\n" << endl;
  }else{
    cout << "> list does not hold element!\n" << endl;
  }

#endif
    
    return exec_time;
  }
  
#ifdef DEBUG
  cout << "> Element found at index: " << *index << '\n' << endl;
  if((list[*index]!=find)){
    cout << "> ERROR: the found element was not the same as the element searched for\n" << endl;
  }
#endif
  return exec_time;
}

