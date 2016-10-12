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



long search(int* list, long N, int find){
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
