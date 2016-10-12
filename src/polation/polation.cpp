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


int main(int argc, char* argv[]){
  // time measuring
  unsigned long exec_time;
  
  long* args = handle_Input(argc, argv);
  
  VALUE x;
  long N, degree, seed;
  N = args[1] == 0 ? SIZE : args[1];
  degree = args[0] == 0 ? N-1 : args[0];
  
  seed = args[5] == 0 ? time(NULL) : args[5];
  srand(seed);
  
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
  VALUE *x_values, *y_values;
  VALUE y;

  x_values = new VALUE[N];
  y_values = new VALUE[N];
  allocate_2D(Q, N);

  // Initialization 2D
  // degree+1 as degree=1 refers to ax^1+bx^0 and not bx^0
  VALUE *polynomial = new VALUE[degree+1];
  init_1D(polynomial, degree+1, MIN_POLYNOMIAL, MAX_POLYNOMIAL);
  init_Polynomial(x_values, y_values, N, polynomial, degree);
  
  // Execution 2D
  //exec_time = polate(x_values, y_values, N, x, &y, &dy); 
  exec_time = polate(x_values, y_values, N, x, Q);
  
  y = Q[N-1][N-1];
  cout << "Time: \t" << exec_time << " ms\n\n";
  
#ifdef DEBUG
  cout << "> x : " << x << "\n";
  cout << "> interpolated y : " << y << "\n";

  VALUE y_test = apply_poly(x, polynomial, degree);
  
  cout << "> correct y : " << y_test << "\n";
  
  cout << "> Difference : " << y_test-y << "\n\n";
  
#endif
  delete[] x_values;
  delete[] y_values;

  delete[] polynomial;
  delete_2D(Q, N);

  return EXIT_SUCCESS;
}
