#include "util.h"
#include "array.h"
#include "search.h"
#include "binary.h"

#include <boost/optional.hpp>
#include <iostream>


using namespace std;
using namespace cEleven;


/**
 * @brief Measures the time it takes to execute binary search on a list
 * @param list the list to be searched through
 * @param N the size of the list
 * @param find the element that should be found
 * @param index place for the index of the found element
 * @return time taken
 **/
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

