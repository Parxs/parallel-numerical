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

int main(int argc, char* argv[]){
  // time measuring
  unsigned long exec_time;
  
  long* args = handle_Input(argc, argv);
  
  long M, N, K, seed;
  M = args[0] == 0 ? SIZE : args[0];
  N = args[1] == 0 ? SIZE : args[1];
  K = args[2] == 0 ? SIZE : args[2];
  seed = args[5] == 0 ? time(NULL) : args[5];

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
  cout << 	"=================================================\n\n";

  // Allocation 2D
#ifdef DEBUG
  cout << "> Allocating 2D arrays\n";
#endif
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
  exec_time = multiply_matrix2D(A_2d, B_2d, C1_2d, M, N, K);
  cout << "Naive 2D: \t" << exec_time << " ms\n";
  
 
  exec_time = multiply_matrix2D_optimized(A_2d, B_2d, C1_2d, M, N, K);
  cout << "Optimized 2D: \t" << exec_time << " ms\n\n";
  
#ifdef TESTING
  if(!compare_2D(C1_2d, C2_2d, M, K, __LINE__)){
    cout << "> ERROR: Different results for 1D matrix multiplications";
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
  exec_time = multiply_matrix1D(A_1d, B_1d, C1_1d, M, N, K);
  cout << "Naive 1D: \t" << exec_time << " ms\n";
  
  
  exec_time = multiply_matrix1D_optimized(A_1d, B_1d, C2_1d, M, N, K);
  cout << "Optimized 1D: \t" << exec_time << " ms\n\n";
  
#ifdef TESTING
  if(!compare_1D(C1_1d, C2_1d, M*K)){
    cout << "> ERROR: Different results for 1D matrix multiplications";
  }
#endif

  // Finalization 1D
  delete[] A_1d;
  delete[] B_1d;
  
  delete[] C1_1d;
  delete[] C2_1d;
  


  return EXIT_SUCCESS;
}
