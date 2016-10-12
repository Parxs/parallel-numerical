#include <iostream>
#include <cstdlib>

#include "util.h"
#include "array.h"
#include "eigenvalue.h"
#include "seq_jacobi_eigen.h"

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
  unsigned long exec_time;
  
  long* args = handle_Input(argc, argv);
  
  long N, seed;
  N = args[1] == 0 ? SIZE : args[1];
  seed = args[5] == 0 ? time(NULL) : args[5];

  srand(seed);
  delete[] args;
  
  
  cout << "\n=================================================\n";
  cout << "Calculating Eigenvalues:\n";
  cout << "N = " << N << "\n";
  if(MIN<MAX){
    cout <<"Bounds (lower/upper) = " << MIN << "/" << MAX << "\n";
  }
  cout << "Seed = " << seed << "\n";
  cout << 	"=================================================\n\n";

  VALUE **A, **V;
  VALUE *d;
  
  // Allocation
  A = new VALUE*[N];
  V = new VALUE*[N];
  allocate_2D(A, N);
  allocate_2D(V, N);
  
  
  d = new VALUE[N];
  

  // Initialization
  init_Symmetric(A, N, (VALUE)MIN, (VALUE)MAX);
  
  // Execution
  exec_time = calculate_Eigen(A, d, V, N);
  cout << "Time: \t" << exec_time << " ms\n\n";
  
#ifdef DEBUG
  reconstruct_Symmetric(A, N);
  
  cout << std::boolalpha;
  bool res = check_Eigenvalues(A, d, V, N);
  cout << "> Check whether A*x = lambda*x: " << res << "\n\n";
  
  
#endif

  // Finalization
  delete_2D(A, N);
  delete_2D(V, N);

  delete[] d;


  return EXIT_SUCCESS;
}
