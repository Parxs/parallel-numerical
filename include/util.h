#ifndef UTIL_H
#define UTIL_H

#include "util.hxx"

// as probably not all threads can do work split work even more to avoid one doing much more than the others
#ifndef FACTOR_WORK
#define FACTOR_WORK 2
#endif

#define MAX_ARGS 9

extern int num_threads;

template <typename T>
T get_rand(T lower, T upper);
template <>
double get_rand<double>(double lower, double upper);
template <>
float get_rand<float>(float lower, float upper);

template <typename T>
T get_Average(T *list, long sample_size);


template <typename T>
void log_Difference(std::ostream& os, T expected, T gotten);



long* handle_Input(int argc, char* argv[]);


unsigned long time_ms();

int get_num_threads(long N);



#endif // UTIL_H
