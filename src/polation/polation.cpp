#include <iostream>
#include <cstdlib>

#include "util.h"
#include "array.h"
#include "polation.h"
#include "seq_polation.h"

using namespace std;

#ifndef SIZE
#define SIZE 1024
#endif
#ifndef MIN_POLYNOMIAL
#define MIN_POLYNOMIAL -10.0
#endif
#ifndef MAX_POLYNOMIAL
#define MAX_POLYNOMIAL -10.0
#endif
#ifndef REPEATS
#define REPEATS 1
#endif


int main(int argc, char* argv[]){
  // time measuring
  unsigned long *exec_times;
  
  long* args = handle_Input(argc, argv);
  
  VALUE x;
  long N, degree, seed, repeats;
  N = args[1] == 0 ? SIZE : args[1];
  degree = args[0] == 0 ? N-1 : args[0];
  
  seed = args[5] == 0 ? time(NULL) : args[5];
  srand(seed);
  
  repeats = args[8] == 0 ? REPEATS : args[8];

  exec_times = new unsigned long[repeats];
  
  // TODO better input handling
  // the cast here is necessary but rather stupid because it should be
  // possible to give floating point numbers as argument
  x = args[7] == 0 ? get_rand(MIN, MAX) : (VALUE) args[7];

  
  delete[] args;
  
  
  cout << "\n=================================================\n";
  cout << "Inter/Extra-Polation:\n";
  cout << "N = " << N << "\n";
  cout << "Polynomial-Degree = " << degree << "\n";
  cout << "x = " << x << "\n";
  if(MIN<MAX){
    cout <<"Bounds (lower/upper) = " << MIN << "/" << MAX << "\n";
  }
  cout << "Seed = " << seed << "\n";
  cout << 	"=================================================\n\n";

  // Allocation
  VALUE **Q = new VALUE*[N];
  allocate_2D(Q, N);
  
  // degree+1 as degree=1 refers to ax^1+bx^0 and not bx^0
  VALUE *polynomial = new VALUE[degree+1];
  
  VALUE *x_values, *y_values;
  x_values = new VALUE[N];
  y_values = new VALUE[N];
  
  VALUE y;

  for(long i=0; i<repeats; i++){
    
    init_1D(polynomial, degree+1, MIN_POLYNOMIAL, MAX_POLYNOMIAL);
    init_Polynomial(x_values, y_values, N, polynomial, degree);
    
    // Execution
    exec_times[i] = polate(x_values, y_values, N, x, Q);

    y = Q[N-1][N-1];
    
#ifdef DEBUG
    VALUE y_test = apply_poly(x, polynomial, degree);
    
    // get max for relative & absolute tolerance test
    VALUE max = (VALUE) 1e1;
    if(fabs(y) > max){
      max = fabs(y);
    }else if(fabs(y_test)){
      max = fabs(y_test);
    } 
    
    if(fabs(y-y_test) > EPSILON * max){
      std::cout << "ERROR: interpolated y is not equal to real y!\n";
      log_Difference(std::cout, y_test, y);
    }
#endif
  }
  
  cout << "avg. Time: \t" << get_Average(exec_times, repeats) << " ms\n\n";
  
  delete[] x_values;
  delete[] y_values;
  delete[] polynomial;
  delete_2D(Q, N);
  
  delete[] exec_times;

  return EXIT_SUCCESS;
}
