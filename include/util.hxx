#ifndef UTIL_HXX
#define UTIL_HXX

#include <cstdlib>
#include <limits>
#include <random>

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
T get_Average(T *exec_times, long sample_size){
  T avg_time = 0;
  
  for(long i=0; i<sample_size; i++){
    avg_time += exec_times[i]/sample_size;
  }
  
  return avg_time;
}


#endif //UTIL_HXX



