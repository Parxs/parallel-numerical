#include <iostream>
#include <cstdlib>
#include <boost/optional.hpp>

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
#ifndef ITERATIONS
#define ITERATIONS 1
#endif

int main(int argc, char* argv[]){
  unsigned long *exec_times;
  
  Input_Container cont = get_Arguments(argc, argv);
  if(cont.help_needed){
    print_help(argv[0]);
    exit(0);
  }
  long N, seed, iterations;
  seed = (cont.seed) ? (*cont.seed) : (time(NULL));
  srand(seed);
  iterations = (cont.iterations) ? (*cont.iterations) : ITERATIONS;
  
  N = (cont.N) ? (*cont.N) : (SIZE);
  
  exec_times = new unsigned long[iterations];
  
  
  cout << "\n=================================================\n";
  cout << "Calculating Eigenvalues:\n";
  cout << "N = " << N << "\n";
  if(MIN<MAX){
    cout << "Bounds (lower/upper) = " << MIN << "/" << MAX << "\n";
  }
  cout << "Seed = " << seed << "\n";
  if(iterations > 1){
    cout << "Sample-Size = " << iterations << "\n";
  }
  cout << 	"=================================================\n\n";

  VALUE **A, **V;
  VALUE *d;
  
 // Allocation
  A = new VALUE*[N];
  V = new VALUE*[N];
  allocate_2D(A, N);
  allocate_2D(V, N);
  
  d = new VALUE[N];
  
  for(long i=0; i<iterations; i++){
    // Initialization
    init_Symmetric(A, N, (VALUE)MIN, (VALUE)MAX);
    
    // Execution
    exec_times[i] = calculate_Eigen(A, d, V, N);
    
#ifdef DEBUG
    reconstruct_Symmetric(A, N);
    
    bool res = check_Eigenvalues(A, d, V, N);
    if(!res){
      cout << "> ERROR: trying A*x = lambda*x leads to wrong result\n\n";
    }
#endif
  }
  
  cout << "avg. Time: \t" << get_Average(exec_times, iterations) << " ms\n\n";
  
  // Finalization
  delete_2D(A, N);
  delete_2D(V, N);

  delete[] d;
  delete[] exec_times;

  return EXIT_SUCCESS;
}
