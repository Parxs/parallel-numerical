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
#ifndef ITERATIONS
#define ITERATIONS 1
#endif


int main(int argc, char* argv[]){
  // time measuring
  unsigned long *exec_times;
  
  Input_Container cont = get_Arguments(argc, argv);
  if(cont.help_needed){
    print_help(argv[0]);
    exit(0);
  }
  long N, iterations, degree;
  if(cont.seed){
    util::mt.seed(*cont.seed);
  }
  iterations = (cont.iterations) ? (*cont.iterations) : ITERATIONS;
  
  N = (cont.N) ? (*cont.N) : (SIZE);
  degree = (cont.M) ? (*cont.M) : (N-1);
  VALUE x = (cont.X) ? ((VALUE) *cont.X) : get_rand(MIN, MAX);
  
  exec_times = new unsigned long[iterations];
  
  
  cout << "\n=================================================\n";
  cout << "Inter/Extra-Polation:\n";
  cout << "N = " << N << "\n";
  cout << "Polynomial-Degree = " << degree << "\n";
  cout << "x = " << x << "\n";
  if(MIN<MAX){
    cout <<"Bounds (lower/upper) = " << MIN << "/" << MAX << "\n";
  }
  if(cont.seed){
    cout << "Seed = " << *cont.seed << "\n";
  }
  if(iterations > 1){
    cout << "Sample-Size = " << iterations << "\n";
  }
  cout << 	"=================================================\n\n";

  // Allocation
  VALUE **Q = new VALUE*[N];
  allocate(Q, N);
  
  // degree+1 as degree=1 refers to ax^1+bx^0 and not bx^0
  VALUE *polynomial = new VALUE[degree+1];
  
  VALUE *x_values, *y_values;
  x_values = new VALUE[N];
  y_values = new VALUE[N];
  
  VALUE y;

  for(long i=0; i<iterations; i++){
    
    init(polynomial, degree+1, MIN_POLYNOMIAL, MAX_POLYNOMIAL);
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
  
  cout << "avg. Time: \t" << get_Average(exec_times, iterations) << " ms\n\n";
  
  delete[] x_values;
  delete[] y_values;
  delete[] polynomial;
  delete_2D(Q, N);
  
  delete[] exec_times;

  return EXIT_SUCCESS;
}
