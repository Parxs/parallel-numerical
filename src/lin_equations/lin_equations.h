#ifndef LIN_EQUATIONS_H
#define LIN_EQUATIONS_H

#ifndef VALUE
#define VALUE double
#endif

// for rounding errors
#ifndef EPSILON
#define EPSILON 1.0e-8
#endif

#include "util.h"

#include "lin_equations.hxx"


unsigned long solve(VALUE **A, VALUE *b, VALUE *y, VALUE *x, long N);

template <typename T>
void init_Equations(T **A, T *b, long N, T lower, T upper);

template <typename T>
void do_BackSub(T **A, T *y, T *x, long N);

template <typename T>
bool check_Solution(T **A, T *B, T *x, T epsilon, long N);


template <typename T>
void init_Example(T **A, T *b){
  A[0][0] = 4;
  A[0][1] = -1;
  A[0][2] = -1;
  
  A[1][0] = -2;
  A[1][1] = 6;
  A[1][2] = 1;
  
  A[2][0] = -1;
  A[2][1] = 1;
  A[2][2] = 7;
  
  
  b[0] = 3;
  b[1] = 9;
  b[2] = -6;
  
  // x = {1, -2, -1} 
}

#endif // LIN_EQUATIONS_H
