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

Input_Container get_Arguments(int argc, char *argv[]);
long* handle_Input(int argc, char *argv[]);

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
 * @brief Prints which arguments can be given to the executable.
 **/
void print_help(char program_Name[]){
  cout << "\n> USAGE:" << program_Name << "<arguments>" << "\n";
  cout << "----------------------------------------------------------------\n";
  cout << "> possible arguments: (be kind - only minimal error handling!)\n";
  cout << "-H                         \t... prints help\n";
  cout << "-N<num>(, -M<num>, -K<num>)\t... set size(s - if applicable)\n";
  cout << "-S<seed>                   \t... use the given num as random seed\n";
  cout << "-X<num>                    \t... (if applicable) uses this number for X (e.g. search)\n";
  cout << "-I<num>                    \t... sample size of executions for average time\n";
  
  cout << endl;
  exit(0);
}

/**
 * @brief Parses the arguments given and returns their values inside
 * an Input_Container
 **/
Input_Container get_Arguments(int argc, char *argv[]){
  Input_Container cont = Input_Container();
  cont.help_needed = false;
  
  // first argument is name of program
  for(int i=1; i<argc; i++){
   
    //ignore arguments with the wrong formatting
    if(argv[i][0] != '-'){
      continue;
    }
    
    long tmp;
    switch(toupper(argv[i][1])){
      case 'H' :{
        cont.help_needed = true;
        break;
      }
      case 'N' :{
        tmp = abs(std::stol(argv[i]+2));
        cont.N = (tmp==0) ? 1 : tmp;
        break;
      }
      case 'M' :{
        tmp = abs(std::stol(argv[i]+2));
        cont.M = (tmp==0) ? 1 : tmp;
        break;
      }
      case 'K' :{
        tmp = abs(std::stol(argv[i]+2));
        cont.K = (tmp==0) ? 1 : tmp;
        break;
      }
      case 'X' :{
        cont.X = std::stol(argv[i]+2);
        break;
      }
      case 'I' :{
        tmp = abs(std::stol(argv[i]+2));
        cont.iterations = (tmp==0) ? 1 : tmp;
        break;
      }
      case 'S' :{
        tmp = abs(std::stol(argv[i]+2));
        cont.seed = tmp;
        break;
      }
    }
  }
  return cont;
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

