#ifndef BINARY_H
#define BINARY_H

#include <omp.h>
#include <thread>
#include <future>
#include <vector>
#include <iostream>
#include <boost/optional.hpp>


namespace sequential{
  


/**
 * @brief Searches for an element in the specified portion of the list.
 * @param list list to be searched through
 * @param start where the portion starts
 * @param end where the portion ends
 * @param target element to be searched for
 * @return an boost::optional that holds the index when the element was found
 **/
template <typename T>
boost::optional<long> binarysearch(T *list, long start, long end, T target){
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

/**
 * @brief Searches for a given element in the given list.
 * @param list list to be searched through
 * @param N size of the list
 * @param target element to be found
 * @return an boost::optional that holds the index when the element was found
 **/
template <typename T>
boost::optional<long> binarysearch(T* list, long N, T target){
  return binarysearch(list, 0, N-1, target);
  // N-1 because function assumes that list[end] is valid
}  
}




namespace openMP{

/**
 * @brief Searches for an element in the specified portion of the list.
 * @param list list to be searched through
 * @param start where the portion starts
 * @param end where the portion ends
 * @param target element to be searched for
 * @return an boost::optional that holds the index when the element was found
 **/
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

/**
 * @brief Wrapper for the call coorect call to binary.
 * @param list list to be searched through
 * @param start where the portion starts
 * @param end where the portion ends
 * @param target element to be searched for
 * @return an boost::optional that holds the index when the element was found
 **/
template <typename T>
boost::optional<long> binarysearch(T* list, long start, long end, T target){
  return binary(list, start, end-1, target);
}

/**
 * @brief Searches for a given element in the given list.
 * @param list list to be searched through
 * @param N size of the list
 * @param target element to be found
 * @return an boost::optional that holds the index when the element was found
 **/
template <typename T>
boost::optional<long> binarysearch(T* list, long N, T target){
  int num_workers = omp_get_num_threads();
  long chunk_size, extra;
  
  chunk_size = N/num_workers;
  extra = N%num_workers;
  
  
  long index = -1;
  // index is used for the reduction so that a default reduction can be used
  #pragma omp parallel for shared(list) reduction(max:index)
  for(int t=0; t<num_workers; t++){
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
  
}


namespace cEleven{

/**
 * @brief Searches for an element in the specified portion of the list.
 * @param list list to be searched through
 * @param start where the portion starts
 * @param end where the portion ends
 * @param target element to be searched for
 * @return an boost::optional that holds the index when the element was found
 **/
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

/**
 * @brief Wrapper for the call coorect call to binary.
 * @param list list to be searched through
 * @param start where the portion starts
 * @param end where the portion ends
 * @param target element to be searched for
 * @return an boost::optional that holds the index when the element was found
 **/
template <typename T>
boost::optional<long> binarysearch(T* list, long start, long end, T target){
  return binary(list, start, end-1, target);
  // end-1 because function assumes that last given index is valid
}



/**
 * @brief Searches for a given element in the given list.
 * @param list list to be searched through
 * @param N size of the list
 * @param target element to be found
 * @return an boost::optional that holds the index when the element was found
 **/
template <typename T>
boost::optional<long> binarysearch(T* list, long N, T target){
  int num_workers = get_num_threads(N);
  long start, end, chunk_size, extra;
  std::vector<std::future<boost::optional<long>>> futures;
  
  chunk_size = N/num_workers;
  extra = N%num_workers;
  
  for(int t=0; t<num_workers; t++){
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
    
    futures.push_back(std::async(std::launch::async, code));
  }
  
  
  boost::optional<long> result, tmp;
  for(std::future<boost::optional<long>> &f: futures){
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
}


#endif
