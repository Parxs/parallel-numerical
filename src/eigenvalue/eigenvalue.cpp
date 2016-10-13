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
#ifndef REPEATS
#define REPEATS 1
#endif

int main(int argc, char* argv[]){
  unsigned long *exec_times;
  
  long* args = handle_Input(argc, argv);
  
  long N, seed, repeats;
  N = args[1] == 0 ? SIZE : args[1];
  seed = args[5] == 0 ? time(NULL) : args[5];
  repeats = args[8] == 0 ? REPEATS : args[8];

  exec_times = new unsigned long[repeats];

  srand(seed);
  delete[] args;
  
  cout << "\n=================================================\n";
  cout << "Calculating Eigenvalues:\n";
  cout << "N = " << N << "\n";
  if(MIN<MAX){
    cout << "Bounds (lower/upper) = " << MIN << "/" << MAX << "\n";
  }
  cout << "Seed = " << seed << "\n";
  if(repeats > 1){
    cout << "Sample-Size = " << repeats << "\n";
  }
  cout << 	"=================================================\n\n";

  long i;
  for(i=0; i<repeats; i++){
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
    exec_times[i] = calculate_Eigen(A, d, V, N);
    
#ifdef DEBUG
    cout << "> Execution-Count: " << i+1 << "\n";
    cout << "> time taken: " << exec_times[i] << "\n\n"; 
  
    reconstruct_Symmetric(A, N);
    
    cout << std::boolalpha;
    bool res = check_Eigenvalues(A, d, V, N);
    cout << "> Check whether A*x = lambda*x: " << res << "\n\n";
#endif

    // Finalization
    delete_2D(A, N);
    delete_2D(V, N);

    delete[] d;
    
  }

  cout << "avg. Time: \t" << get_Average(exec_times, repeats) << " ms\n\n";
  
  
  delete[] exec_times;

  return EXIT_SUCCESS;
}
