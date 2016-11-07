/** 
 * @brief Parallel implementation of matrix multiplication via features
 * of C11.
 */
#ifndef C11_MATRIX_MULT_H
#define C11_MATRIX_MULT_H

#include <future>
#include <thread>
#include <vector>

#include "util.h"

namespace cEleven{
  
void _execute(std::function<void (long, long, int)> func, int num_tasks, long num_elems){
  using namespace std;
  vector<future<void>> futures;
  int t, extra; // extra will at most be as big as num_tasks-1
  long chunksize, start_task, end_task;
  
  // split work
  chunksize = num_elems / num_tasks;
	extra = num_elems % num_tasks; 
	start_task = 0;
	end_task = chunksize;
  
  // run threads
  for(t=0; t<num_tasks; t++){
    // test whether extra work still needs to be done
    if(t < extra){
      end_task++;
    }
    
    futures.push_back(async(launch::async, func, start_task, end_task, t)); 
    
    start_task = end_task;
    end_task = start_task + chunksize;
  }
  
  for(future<void> &f: futures){
    f.get();
  }
}

/**
 * @brief Naive matrix multiplication. Multiplies two 2D matrices.
 *
 * It is assumed that A,B & C are suitable for multiplying and
 * furthermore C must be initialized with 0.
 * There are also no checks for overflows, so it is easily possible
 * that C is not the correct result if the elements itself are too big 
 */
template <typename T>
void multiply_matrix(T **A, T **B, T **C, long M, long N, long K){
  auto code = [ &A, &B, &C, N, K](long start_task, long end_task, int thid)
  {
    T a,b;
    for(long i=start_task; i<end_task; i++){
      for(long k=0; k<K; k++){
        for(long j=0; j<N; j++){
          a = A[i][j];
          b = B[j][k];
          C[i][k] += a*b;
        }
      }
    }
  };
  
  _execute(code, get_num_threads(M), M);
}

/**
 * @brief Optimized matrix multiplication. Multiplies two 2D matrices.
 *
 * It is assumed that A,B & C are suitable for multiplying and
 * furthermore C must be initialized with 0.
 * This algorithm has been optimized by accessing the elements which are 
 * saved closed to already accessed ones first.
 * There are also no checks for overflows, so it is easily possible
 * that C is not the correct result if the elements itself are too big 
 */
template <typename T>
void multiply_matrix_optimized(T **A, T **B, T **C, long M, long N, long K){
  auto code = [ &A, &B, &C, N, K](long start_task, long end_task, int thid)
  {
    T a,b;
    for(long i=start_task; i<end_task; i++){
      for(long j=0; j<N; j++){
        a = A[i][j];
        for(long k=0; k<K; k++){
          b = B[j][k];
          C[i][k] += a*b;
        }
      }
    }
  };
  
  _execute(code, get_num_threads(M), M);
}

/**
 * @brief Naive matrix multiplication. Multiplies two 2D matrices saved in a 1D array.
 *
 * It is assumed that A,B & C are suitable for multiplying and
 * furthermore C must be initialized with 0.
 * There are also no checks for overflows, so it is easily possible
 * that C is not the correct result if the elements itself are too big 
 */
template <typename T>
void multiply_matrix(T *A, T *B, T *C, long M, long N, long K){
  auto code = [ &A, &B, &C, N, K](long start_task, long end_task, int thid)
  {
    T a,b;
    long a_index, b_index, c_index;
    
    for(long i=start_task; i<end_task; i++){
      for(long k=0; k<K; k++){
        for(long j=0; j<N; j++){
          a_index = i*N+j;
          b_index = j*K+k;
          c_index = i*K+k;
          a = A[a_index];
          b = B[b_index];
          C[c_index] += a*b;
        }
      }
    }  
  };
  
  _execute(code, get_num_threads(M), M);
} 

/**
 * @brief Optimized matrix multiplication. Multiplies two 2D matrices saved in a 1D array.
 *
 * It is assumed that A,B & C are suitable for multiplying and
 * furthermore C must be initialized with 0.
 * There are also no checks for overflows, so it is easily possible
 * that C is not the correct result if the elements itself are too big 
 */
template <typename T>
void multiply_matrix_optimized(T *A, T *B, T *C, long M, long N, long K){
  //VALUE** ptr = &C;
  auto code = [ &A, &B, &C,  N, K](long start_task, long end_task, int thid)
  {
    //VALUE *C = *ptr;
    T a,b;
    long a_index, b_index, c_index;
    
    for(long i=start_task; i<end_task; i++){
      for(long j=0; j<N; j++){
        a_index = i*N+j;
        a = A[a_index];
        for(long k=0; k<K; k++){
          b_index = j*K+k;
          c_index = i*K+k;
          b = B[b_index];
          C[c_index] += a*b;
        }
      }
    } 
  };

  _execute(code, get_num_threads(M), M);
}

}
#endif
