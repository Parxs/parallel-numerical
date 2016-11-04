/**
 * @brief Main implementation for matrix multiplication.
 * 
 * It is important to remember that for this implementation
 * the multiplied matrices have the dimensions MxN, NxK & MxK which
 * differs from others where M & N are switched.
 **/

#include <iostream>
#include <cstdlib>
#include <boost/optional.hpp>

#include "util.h"
#include "array.h"
#include "matrix_mult.h"

using namespace std;

#ifndef SIZE
#define SIZE 1024
#endif
#ifndef MIN
#define MIN -100.0
#endif
#ifndef MAX
#define MAX 100.0
#endif
#ifndef ITERATIONS
#define ITERATIONS 1
#endif
#ifndef EPSILON
#define EPSILON 1e-10
#endif

int main(int argc, char* argv[]){
  // time measuring
  unsigned long *exec_times_2D,  *exec_times_1D;
  
  Input_Container cont = get_Arguments(argc, argv);
  if(cont.help_needed){
    print_help(argv[0]);
    exit(0);
  }
  long M, N, K, seed, iterations;
  seed = (cont.seed) ? (*cont.seed) : (time(NULL));
  srand(seed);
  iterations = (cont.iterations) ? (*cont.iterations) : ITERATIONS;
  
  N = (cont.N) ? (*cont.N) : (SIZE);
  if(cont.N && !cont.M && !cont.K){
    M = N;
    K = N;
  }else{
    M = (cont.M) ? (*cont.M) : (SIZE);
    K = (cont.K) ? (*cont.K) : (SIZE);
  }
  
  exec_times_2D = new unsigned long[iterations*2];
  exec_times_1D = new unsigned long[iterations*2];
  
  
  VALUE **A_2d, **B_2d, **C1_2d, **C2_2d;
  VALUE *A_1d, *B_1d, *C1_1d, *C2_1d;
  
  cout << "\n=================================================\n";
  cout << "Matrix-Multiplication:\n";
  cout << "M = " << M << "| N = " << N << "| K = " << K << "\n";
  if(MIN<MAX){
    cout <<"Bounds (lower/upper) = " << MIN << "/" << MAX << "\n";
  }
  cout << "Seed = " << seed << "\n";
  if(iterations > 1){
    cout << "Sample-Size = " << iterations << "\n";
  }
  cout << 	"=================================================\n\n";


  for(long i=0; i<iterations; i++){
    // Allocation 2D
    A_2d = new VALUE*[M];
    B_2d = new VALUE*[N];
    
    C1_2d = new VALUE*[M];
    C2_2d = new VALUE*[M];
    
    allocate(A_2d, M, N);
    allocate(B_2d, N, K);
    allocate(C1_2d, M, K);
    allocate(C2_2d, M, K);

    // Initialization 2D
    init(A_2d, M, N, MIN, MAX);
    init(B_2d, N, K, MIN, MAX);
    
    fill(C1_2d, M, K, 0.0);
    fill(C2_2d, M, K, 0.0);
    
    // Execution 2D
    exec_times_2D[i] = matrix_mult(A_2d, B_2d, C1_2d, M, N, K);
    
    exec_times_2D[i+iterations] = matrix_mult_optimized(A_2d, B_2d, C2_2d, M, N, K);
    
#ifdef DEBUG
    if(!compare(C1_2d, C2_2d, EPSILON, M, K)){
      cout << "> ERROR: Different results for 2D matrix multiplications\n";
    }
#endif
    // Finalization 2D
    delete_2D(C1_2d, M);
    delete_2D(C2_2d, M);
    
    // Allocating 1D
    A_1d = new VALUE[M*N];
    B_1d = new VALUE[N*K];
    
    // initializes with 0
    C1_1d = new VALUE[M*K]();
    C2_1d = new VALUE[M*K]();
    
    // Initialization 1D
    copy_to_1D(A_2d, A_1d, M, N);
    delete_2D(A_2d, M);
    copy_to_1D(B_2d, B_1d, N, M);
    delete_2D(B_2d, N);
    
    // Execution 1D
    exec_times_1D[i] = matrix_mult(A_1d, B_1d, C1_1d, M, N, K);
    
    
    exec_times_1D[i+iterations] = matrix_mult_optimized(A_1d, B_1d, C2_1d, M, N, K);
    
    
    
#ifdef DEBUG
    if(!compare(C1_1d, C2_1d, EPSILON, M*K)){
      cout << "> ERROR: Different results for 1D matrix multiplications\n";
    }
#endif

    // Finalization 1D
    delete[] A_1d;
    delete[] B_1d;
    
    delete[] C1_1d;
    delete[] C2_1d;
    
  }
  
  
  // for getting the optimized average time an offset of iterations is needed
  cout << "avg. Time (Naive, 2D):    \t" << get_Average(exec_times_2D, iterations) << " ms\n";
  cout << "avg. Time (Optimized, 2D):\t" << get_Average(exec_times_2D+iterations, iterations) << " ms\n";
  cout << "avg. Time (Naive, 1D):    \t" << get_Average(exec_times_1D, iterations) << " ms\n";
  cout << "avg. Time:(Optimized, 1D):\t" << get_Average(exec_times_1D+iterations, iterations) << " ms\n\n";
  
  
  delete[] exec_times_2D;
  delete[] exec_times_1D;


  return EXIT_SUCCESS;
}
