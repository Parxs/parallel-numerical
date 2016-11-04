#ifndef UTIL_H
#define UTIL_H

#include <boost/optional.hpp>
#include <random>

#include "util.hxx"


// as probably not all threads can do work split work even more to avoid one doing much more than the others
#ifndef FACTOR_WORK
#define FACTOR_WORK 2
#endif

#define MAX_ARGS 9

/**
 * @brief Container for holding the input-parameters.
 * 
 * All the parameters can be saved in this class and it is easy to check
 * whether a value was set because "boost::optional" is used as type.
 **/
class Input_Container{
public:
  boost::optional<long> N;
  boost::optional<long> M;
  boost::optional<long> K;
  
  boost::optional<long> X;
  
  boost::optional<long> seed;
  
  boost::optional<long> iterations;
  
  bool help_needed;
};

/* holds the number of threads that should be used to get a good performance*/
extern int num_threads;

namespace util{
  extern std::mt19937 mt;
}

int get_rand(int lower, int upper);
long get_rand(long lower, long upper);
float get_rand(float lower, float upper);
double get_rand(double lower, double upper);
long double get_rand(long double lower, long double upper);


template <typename T>
T get_Average(T *list, long sample_size);


template <typename T>
void log_Difference(std::ostream& os, T expected, T gotten);

void print_help(char *program_Name);

Input_Container get_Arguments(int argc, char *argv[]);

unsigned long time_ms();

int get_num_threads(long N);



#endif // UTIL_H
