#ifndef POLATION_H
#define POLATION_H

#ifndef VALUE
#define VALUE double
#endif
#ifndef MIN
#define MIN -10.0
#endif
#ifndef MAX
#define MAX 10.0
#endif
#ifndef EPSILON
#define EPSILON 1e-2
#endif


#include "util.h"

#include "polation.hxx"


unsigned long polate(VALUE *x_values, VALUE *y_values, long N, VALUE x, VALUE *y, VALUE *dy);
unsigned long polate(VALUE *x_values, VALUE *y_values, long N, VALUE x, VALUE **Q);

template <typename T>
void init_Quadratic(T *x_values, T *y_values, long N);

template <typename T>
T apply_poly(T x, T *polynomial, long degree);

template <typename T>
void init_Polynomial(T *x_values, T *y_values, long N, T *polynomial, long degree);

template <typename T>
void print_XY(T *x_values, T *y_values, long N);


#endif // POLATION_H
