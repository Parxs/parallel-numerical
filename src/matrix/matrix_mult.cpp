/**
 * @brief Main implementation for matrix multiplication.
 * 
 * It is important to remember that for this implementation
 * the multiplied matrices have the dimensions MxN, NxK & MxK which
 * differs from others where M & N are switched.
 * Furthermore, 1D uses different matrices than 2D so their execution
 * time is not totally comparable.
 **/

#include <iostream>
#include <cstdlib>

#include <util.h>
#include "array.h"
#include "matrix_mult.h"


using namespace std;

#ifndef SIZE
#define SIZE 1024
#endif
#ifndef MIN
#define MIN -10000.0
#endif
#ifndef MAX
#define MAX 10000.0
#endif
#ifndef REPEATS
#define REPEATS 1
#endif

int main(int argc, char* argv[]){
  // time measuring
  unsigned long *exec_times_2D, *exec_times_opt_2D, *exec_times_1D, *exec_times_opt_1D;
  
  long* args = handle_Input(argc, argv);
  
  long M, N, K, seed, repeats;
  M = args[0] == 0 ? SIZE : args[0];
  N = args[1] == 0 ? SIZE : args[1];
  K = args[2] == 0 ? SIZE : args[2];
  seed = args[5] == 0 ? time(NULL) : args[5];
  repeats = args[8] == 0 ? REPEATS : args[8];

  exec_times_2D = new unsigned long[repeats];
  exec_times_1D = new unsigned long[repeats];
  exec_times_opt_2D = new unsigned long[repeats];
  exec_times_opt_1D = new unsigned long[repeats];

  srand(seed);
  delete[] args;
  
  VALUE **A_2d, **B_2d, **C1_2d, **C2_2d;
  VALUE *A_1d, *B_1d, *C1_1d, *C2_1d;
  
  cout << "\n=================================================\n";
  cout << "Matrix-Multiplication:\n";
  cout << "M = " << M << "| N = " << N << "| K = " << K << "\n";
  if(MIN<MAX){
    cout <<"Bounds (lower/upper) = " << MIN << "/" << MAX << "\n";
  }
  cout << "Seed = " << seed << "\n";
  if(repeats > 1){
    cout << "Sample-Size = " << repeats << "\n";
  }
  cout << 	"=================================================\n\n";


  for(long i=0; i<repeats; i++){
    // Allocation 2D
    A_2d = new VALUE*[M];
    B_2d = new VALUE*[N];
    
    C1_2d = new VALUE*[M];
    C2_2d = new VALUE*[M];
    
    allocate_2D(A_2d, M, N);
    allocate_2D(B_2d, N, K);
    allocate_2D(C1_2d, M, K);
    allocate_2D(C2_2d, M, K);

    // Initialization 2D
    init_2D(A_2d, M, N, MIN, MAX);
    init_2D(B_2d, N, K, MIN, MAX);
    
    fill_2D(C1_2d, M, K, 0.0);
    fill_2D(C2_2d, M, K, 0.0);
    
    // Execution 2D
    exec_times_2D[i] = multiply_matrix2D(A_2d, B_2d, C1_2d, M, N, K);
    
   
    exec_times_opt_2D[i] = multiply_matrix2D_optimized(A_2d, B_2d, C1_2d, M, N, K);
    
#ifdef DEBUG
    cout << "> Execution-Count: " << i+1 << "\n";
    cout << "> time taken: \n";
    cout << "Naive: " << exec_times_2D[i] << "\n";
    cout << "Optimized:\n " << exec_times_opt_2D[i] + "\n\n"; 

    if(!compare_2D(C1_2d, C2_2d, M, K)){
      //cout << "> ERROR: Different results for 2D matrix multiplications\n";
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
    copy_2D_to_1D(A_2d, A_1d, M, N);
    delete_2D(A_2d, M);
    copy_2D_to_1D(B_2d, B_1d, M, N);
    delete_2D(B_2d, N);
    
    // Execution 1D
    exec_times_1D[i] = multiply_matrix1D(A_1d, B_1d, C1_1d, M, N, K);
    
    
    exec_times_opt_1D[i] = multiply_matrix1D_optimized(A_1d, B_1d, C2_1d, M, N, K);
    
#ifdef DEBUG
    cout << "> time taken: \n";
    cout << "Naive: " << exec_times_1D[i] << "\n";
    cout << "Optimized: " << exec_times_opt_1D[i] + "\n\n"; 

    if(!compare_1D(C1_1d, C2_1d, M*K)){
      cout << "> ERROR: Different results for 1D matrix multiplications\n";
    }
#endif

    // Finalization 1D
    delete[] A_1d;
    delete[] B_1d;
    
    delete[] C1_1d;
    delete[] C2_1d;
    
  }
  
  
  // for getting the optimized average time an offset of repeats is needed
  cout << "avg. Time (Naive, 2D):    \t" << get_Average(exec_times_2D, repeats) << " ms\n";
  cout << "avg. Time (Optimized, 2D):\t" << get_Average(exec_times_opt_2D, repeats) << " ms\n";
  cout << "avg. Time (Naive, 1D):    \t" << get_Average(exec_times_1D, repeats) << " ms\n";
  cout << "avg. Time:(Optimized, 1D):\t" << get_Average(exec_times_opt_1D, repeats) << " ms\n\n";
  
  
  delete[] exec_times_2D;
  delete[] exec_times_1D;
  delete[] exec_times_opt_2D;
  delete[] exec_times_opt_1D;


  return EXIT_SUCCESS;
}