#ifndef UTIL_HXX
#define UTIL_HXX

#include <cstdlib>
#include <limits>
#include <random>
#include <cmath>
#include <iostream>

/*====================================================================*/
// Implementations
/*====================================================================*/

/**
 * @brief Generates a biased random number between a given upper and lower bound.
 * 
 * Should lower be under 0, then the returned number will be between negative and
 * positive upper.
 * Range: [lower, upper)
 */
template <typename T>
T get_rand(T lower, T upper){
  bool sign = false;
  if(0 > lower){
    lower = 0;
    sign = true;
  }
  if(RAND_MAX < upper){
    upper = RAND_MAX;
    
  }
  T range = upper-lower;
  T rand_num = std::rand() % range;
	
  if(sign){
    if(std::rand()%2 > 0){
      rand_num *= -1;
    }
  }

  return rand_num+lower;  
}


template <typename T>
T get_Average(T *list, long sample_size){
  // double is used for higher precision when dividing
  double avg_time = 0;
  
  for(long i=0; i<sample_size; i++){
    double tmp = (double) list[i];
    
    avg_time += tmp/sample_size;
  }
  
  // add results together to get average
  avg_time = std::round(avg_time);
  return (T) avg_time;
}


template <typename T>
void log_Difference(std::ostream& os, T expected, T gotten){
  using namespace std;
  os << scientific << showpos;
  os << "Expected:\t" << expected << "\n";
  os << "Got:     \t" << gotten << "\n";
  os << "Diff:    \t" << expected-gotten << "\n\n";
}


#endif




