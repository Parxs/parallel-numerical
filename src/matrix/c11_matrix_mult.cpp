/** 
 * @brief Parallel implementation of matrix multiplication via OpenMP.
 * @date 02.02.2016
 */


#include <future>
#include <thread>
#include <vector>
#include <iostream>

#include "util.h"
#include "matrix_mult.h"

int num_workers;

using namespace std;


void _execute(function<bool (long, long, int)> func, int num_tasks, long num_elems){
  vector<future<bool>> futures;
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
  
  for(future<bool> &f: futures){
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
unsigned long multiply_matrix2D(VALUE** A, VALUE** B, VALUE** C, long M, long N, long K){
  num_workers = get_num_threads(M);
  auto code = [ &A, &B, &C, N, K](long start_task, long end_task, int thid) -> bool
  {
    VALUE a,b;
    long i,j,k;
    for(i=start_task; i<end_task; i++){
      for(k=0; k<K; k++){
        for(j=0; j<N; j++){
          a = A[i][j];
          b = B[j][k];
          C[i][k] += a*b;
        }
      }
    }
    return true;
  };
  
  unsigned long start = time_ms();
  
  _execute(code, num_workers, M);
  
  return time_ms() - start;
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
unsigned long multiply_matrix2D_optimized(VALUE** A, VALUE** B, VALUE** C, long M, long N, long K){
  num_workers = get_num_threads(M);
  
  auto code = [ &A, &B, &C, N, K](long start_task, long end_task, int thid) -> bool
  {
    VALUE a,b;
    long i,j,k;
    for(i=start_task; i<end_task; i++){
      for(j=0; j<N; j++){
        a = A[i][j];
        for(k=0; k<K; k++){
          b = B[j][k];
          C[i][k] += a*b;
        }
      }
    }
    return true;
  };
  
  unsigned long start = time_ms();
  
  _execute(code, num_workers, M);
  
  return time_ms() - start;
}


/**
 * @brief Naive matrix multiplication. Multiplies two 2D matrices saved in a 1D array.
 *
 * It is assumed that A,B & C are suitable for multiplying and
 * furthermore C must be initialized with 0.
 * There are also no checks for overflows, so it is easily possible
 * that C is not the correct result if the elements itself are too big 
 */
unsigned long multiply_matrix1D(VALUE* A, VALUE* B, VALUE* C, long M, long N, long K){
  num_workers = get_num_threads(M);
  
  auto code = [ &A, &B, &C, N, K](long start_task, long end_task, int thid) -> bool
  {
    VALUE a,b;
    long a_index, b_index, c_index;
    long i,j,k;
    for(i=start_task; i<end_task; i++){
      for(k=0; k<K; k++){
        for(j=0; j<N; j++){
          a_index = i*N+j;
          b_index = j*K+k;
          c_index = i*K+k;
          a = A[a_index];
          b = B[b_index];
          C[c_index] += a*b;
        }
      }
    }  
    return true;
  };
  
  unsigned long start = time_ms();
  
  _execute(code, num_workers, M);
  
  return time_ms() - start; 
}


/**
 * @brief Optimized matrix multiplication. Multiplies two 2D matrices saved in a 1D array.
 *
 * It is assumed that A,B & C are suitable for multiplying and
 * furthermore C must be initialized with 0.
 * There are also no checks for overflows, so it is easily possible
 * that C is not the correct result if the elements itself are too big 
 */
unsigned long multiply_matrix1D_optimized(VALUE* A, VALUE* B, VALUE* C, long M, long N, long K){
  num_workers = get_num_threads(M);
  
  VALUE** ptr = &C;
  auto code = [ &A, &B, ptr, N, K](long start_task, long end_task, int thid) -> bool
  {
    VALUE *C = *ptr;
    VALUE a,b;
    long a_index, b_index, c_index;
    long i,j,k;
    for(i=start_task; i<end_task; i++){
      for(j=0; j<N; j++){
        a_index = i*N+j;
        a = A[a_index];
        for(k=0; k<K; k++){
          b_index = j*K+k;
          c_index = i*K+k;
          b = B[b_index];
          C[c_index] += a*b;
        }
      }
    } 
    return true; 
  };
  
  unsigned long start = time_ms();
  
  _execute(code, num_workers, M);
  
  return time_ms() - start;
}
