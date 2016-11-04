#include <iostream>
#include <cstdlib>
#include <boost/optional.hpp>

#include "util.h"
#include "array.h"
#include "lin_equations.h"
#include "seq_gaussian.h"

using namespace std;

#ifndef SIZE
#define SIZE 1024
#endif
#ifndef MIN
#define MIN -10.0
#endif
#ifndef MAX
#define MAX 10.0
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
  long N, iterations;
  if(cont.seed){
    util::mt.seed(*cont.seed);
  }
  iterations = (cont.iterations) ? (*cont.iterations) : ITERATIONS;
  
  N = (cont.N) ? (*cont.N) : (SIZE);
  
  exec_times = new unsigned long[iterations];
  
  
  VALUE **A;
  VALUE *b, *y, *x;
  
  cout << "\n=================================================\n";
  cout << "Solving Linear Equations:\n";
  cout << "N = " << N << "\n";
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

  for(long i=0; i<iterations; i++){
    // Allocation
    A = new VALUE*[N];
    allocate(A, N, N);
    
    b = new VALUE[N];
    y = new VALUE[N];
    x = new VALUE[N](); // needs to be initialized with something

    // Initialization
    init_Equations(A, b, N, (VALUE)MIN, (VALUE)MAX);

#ifdef DEBUG
    // prepare testing
    VALUE **A_test;
    VALUE *b_test;
    
    A_test = new VALUE*[N];
    allocate(A_test, N, N);
    copy(A, A_test, N, N);
    
    b_test = new VALUE[N];
    copy(b, b_test, N);
#endif 
    
    // Execution
    exec_times[i] = solve(A, b, y, x, N);
    
#ifdef DEBUG

    bool res = check_Solution(A_test, b_test, x, EPSILON, N);
    if(!res){
      cout << "> ERROR: the gotten x is not a valid solution!\n\n";
    }

    delete_2D(A_test, N);
    
    delete[] b_test;
#endif

    // Finalization
    delete_2D(A, N);

    delete[] b;
    delete[] y;
    delete[] x;
    
  }

  cout << "avg. Time: \t" << get_Average(exec_times, iterations) << " ms\n\n";
  delete[] exec_times;


  return EXIT_SUCCESS;
}
