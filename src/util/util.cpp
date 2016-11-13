#include "util.h"

#include <iostream>
#include <cstdlib>
#include <limits>
#include <chrono>
#include <thread>
#include <random>

using namespace std;

int num_threads = std::thread::hardware_concurrency() - THREAD_CORRECTION;

std::random_device rd;
std::mt19937 util::mt(rd());

Input_Container get_Arguments(int argc, char *argv[]);
long* handle_Input(int argc, char *argv[]);

unsigned long time_ms();

int get_num_threads(long N);


/*====================================================================*/
// Implementations
/*====================================================================*/
int get_rand(int lower, int upper){
  std::uniform_int_distribution<int> dist(lower, upper);
  
  return dist(util::mt);
}

long get_rand(long lower, long upper){
  std::uniform_int_distribution<long> dist(lower, upper);
  
  return dist(util::mt);
}

float get_rand(float lower, float upper){
  std::uniform_real_distribution<float> dist(lower, upper);
  
  return dist(util::mt);
}

double get_rand(double lower, double upper){
  std::uniform_real_distribution<double> dist(lower, upper);
  
  return dist(util::mt);
}

long double get_rand(long double lower, long double upper){
  std::uniform_real_distribution<long double> dist(lower, upper);
  
  return dist(util::mt);
}

/**
 * @brief Prints which arguments can be given to the executable.
 * @param program_Name name of the program 
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
 * @param argc number of arguments 
 * @param argv arguments
 * @return the arguments parsed into an Input_Container
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
 * @return time since a certain point in the past
 **/
unsigned long time_ms(){
  unsigned long milliseconds_since_epoch = 
    std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::system_clock::now().time_since_epoch()).count();

  return milliseconds_since_epoch;
}


/**
 * @brief Returns the numbers of threads that should be used to get the most out of the machine.
 * 
 * I have not found information on the optimal way
 * to split tasks for the C++11 threads. In general, there will always
 * be processes in the background so just using the provided value
 * of std::hardware_concurrency is not a good idea. To still allow
 * some sort of portability THREAD_CORRECTION is subtracted from the
 * gotten value and this should make it more likely that the threads 
 * used will always be available to the program.
 * 
 * @param N size of the problem
 * @return number of threads that can be used
 **/
int get_num_threads(long N){
  /*
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

