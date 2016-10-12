#include "util.h"

#include <iostream>
#include <cstdlib>
#include <limits>
#include <chrono>
#include <thread>
#include <random>

using namespace std;

int num_threads = std::thread::hardware_concurrency() * FACTOR_WORK;


template <>
double get_rand<double>(double lower, double upper);
template <>
float get_rand<float>(float lower, float upper);

long* handle_Input(int argc, char* argv[]);

unsigned long time_ms();

int get_num_threads(long N);



/*====================================================================*/
// Implementations
/*====================================================================*/

// template specializations must be inside the source to avoid that the linker sees them twice
template <>
double get_rand<double>(double lower, double upper){
  bool sign = false;
  if(0 > lower){
    lower = 0;
    sign = true;
  }
  if(RAND_MAX < upper){
    upper = RAND_MAX;
    
  }
  double f = (double)rand() / RAND_MAX;
	
  f = lower+f*(upper-lower);
  if(sign){
    if(rand()%2 > 0){
      f *= -1;
    }
  }

  return f;    
}

template <>
float get_rand<float>(float lower, float upper){
  bool sign = false;
  if(0 > lower){
    lower = 0;
    sign = true;
  }
  if(RAND_MAX < upper){
    upper = RAND_MAX;
    
  }
  float f = (float)rand() / RAND_MAX;
	
  f = lower+f*(upper-lower);
  if(sign){
    if(rand()%2 > 0){
      f *= -1;
    }
  }

  return f;  
}


/**
 * @brief Parses the arguments given for possible sizes of the matrices.
 **/
long* handle_Input(int argc, char* argv[]){
  long* sizes = new long[MAX_ARGS];
  
  for(int i=0; i<MAX_ARGS; i++){
    sizes[i] = 0l;
    // as unsigned is used negative numbers are impossible
    // could become problematic as soon as the default value
    // of lower bound stops being 0 and or negative numbers 
    // get allowed 
  }

  // first argument is always the name of the program itself
  for(int i=1; i<argc; i++){
    if(argv[i][0] != '-'){
      continue;
    }
    int index;
    switch(toupper(argv[i][1])){
    case 'H':{
      cout << "\nUSAGE:" << argv[0] << "<arguments>" << "\n";
      cout << "possible arguments: (currently only unsigned integers possible!)\n";
      cout << "----------------------------------------------------------------\n";
      cout << "-H                         \t... prints help\n";
      cout << "-N<num>(, -M<num>, -K<num>)\t... set size(s - if applicable)\n";
      cout << "-R<seed>                   \t... use the given num as random seed\n";
      cout << "-F<num>                    \t... (if applicable) searches for the given number\n";
      cout << "-X<num>                    \t... (if applicable) uses this number for X\n";
      
      cout << endl;
      exit(0);
    }
    // sizes 
    case 'M':{
      index = 0;
      break;
    }
    case 'N':{
      index = 1;
      break;
    }
    case 'K':{
      index = 2;
      break;
    }
    // Lower & Upper bounds
    case 'U':{
      index = 3;
      break;
    }
    case 'L':{
      index = 4;
      break;
    }
    // random seed
    case 'R':{
      index = 5;
      break;
    }
    // number to be found 
    case 'F':{
      index = 6;
      break;
    }
    case 'X':{
      index = 7;
      break;
    }
    default :
      continue;
      // ignore all arguments which do not follow this format
    }
    sizes[index] = abs(atol(argv[i]+2));
    // offset by 2 because the first two chars are not part of the number
    // '-' + '<letter>' + "<number>" 
    // TODO: currently makes all numbers positive
  }
  return sizes;
}

/**
 * @brief Returns the passed time since a certain point in the past in miliseconds.
 **/
unsigned long time_ms(){
  unsigned long milliseconds_since_epoch = 
    std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::system_clock::now().time_since_epoch()).count();

  return milliseconds_since_epoch;
}


int get_num_threads(long N){
  /* I have yet to find more information on the optimal way
   * to split tasks for the C++11 threads.
   * As async more or less uses a threadpool it is assumed that
   * it is better to split the work in smaller chunks and let one of
   * those threads do more tasks than not splitting
   * to get a rough estimate I am using the hardware_concurrency()
   * so that I at least know how many threads there can be at most.
   * The tasks are split even more as it is probably very unlikely that
   * all of the threads can be used at the same time
   * 
   * After a bit of testing it seems like that currently no threadpool
   * is used on linux (creating a lot of threads because of divide&
   * conquer leads to stop)
   * */
  if(num_threads < 1){
#ifdef DEBUG
    cout << ">> ERROR: std::thread::hardware_concurrency() returned negative number and/or 0!\n\n";
#endif
    num_threads = 1;
  }
  if(N < num_threads){
#ifdef DEBUG
    cout << ">> WARNING: there are more threads than tasks!\n";
    cout << "-----> Only as many threads as there is work will be used.\n" << endl;
#endif

    if(N<=0){
      cout << ">> ERROR: number of task is less than 0 which makes no sense!\n>> One Thread will be used\n\n";
      
      return 1;
    }
    
    return N;
  }
  
  return num_threads;
}

