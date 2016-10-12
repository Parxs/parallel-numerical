#include <iostream>
#include <cstdlib>

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


int main(int argc, char* argv[]){
  // time measuring
  unsigned long exec_time;
  
  long* args = handle_Input(argc, argv);
  
  long N, seed;
  N = args[1] == 0 ? SIZE : args[1];
  seed = args[5] == 0 ? time(NULL) : args[5];

  srand(seed);
  delete[] args;
  
  
  VALUE **A;
  VALUE *b, *y, *x;
  
  cout << "\n=================================================\n";
  cout << "Solving Linear Equations:\n";
  cout << "N = " << N << "\n";
  if(MIN<MAX){
    cout <<"Bounds (lower/upper) = " << MIN << "/" << MAX << "\n";
  }
  cout << "Seed = " << seed << "\n";
  cout << 	"=================================================\n\n";

  // Allocation
  A = new VALUE*[N];
  allocate_2D(A, N, N);
  
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
  allocate_2D(A_test, N, N);
  copy_2D(A, A_test, N, N);
  
  b_test = new VALUE[N];
  copy_1D(b, b_test, N);
  
  
#endif 
  
  // Execution
  exec_time = solve(A, b, y, x, N);
  cout << "Time: \t" << exec_time << " ms\n\n";
  
#ifdef DEBUG
  cout << boolalpha;
  cout << "> Gotten x holds (correct) solutions: " << check_Solution(A_test, b_test, x, EPSILON, N) << "\n\n";

  delete_2D(A_test, N);
  
  delete[] b_test;
#endif

  // Finalization
  delete_2D(A, N);

  delete[] b;
  delete[] y;
  delete[] x;


  return EXIT_SUCCESS;
}
