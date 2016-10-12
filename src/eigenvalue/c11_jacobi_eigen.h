#ifndef C11_JACOBI_EIGEN_H
#define C11_JACOBI_EIGEN_H

#include "array.h"

#include <cmath>
#include <iostream>
#include <iostream>
#include <future>
#include <thread>
#include <vector>

#ifndef EPS
#define EPS 1.0e-36
#endif
#ifndef THRESHOLD
#define THRESHOLD 50
#endif

struct Pair{long k; long l;};

template <typename T>
struct Container { T val; long k; long l;};

template <typename T>
struct Container<T> _init_Container(T val, long k, long l);

template <typename T>
struct Container<T> _init_Container(T val, long k, long l){
  struct Container<T> cont;
  cont.val = val;
  cont.k = k;
  cont.l = l;
  return cont;
}

struct Pair _init_Pair(long k, long l){
  struct Pair pair;
  pair.k = k;
  pair.l = l;
  
  return pair;
}

/*====================================================================*/
// Execute-Functions

void _execute(std::function<bool (long, long)> func, long start, int num_tasks, long num_elems){
  using namespace std;
  
  vector<future<bool>> futures;
  int t, extra; // extra will at most be as big as num_tasks-1
  long chunksize, start_task, end_task;
  
  // do sequentially if N is small
  if(num_tasks < THRESHOLD){
    func(start, start+num_elems);
    return;
  }
  
  // split work
  chunksize = num_elems / num_tasks;
	extra = num_elems % num_tasks; 
	start_task = start;
	end_task = start+chunksize;
  
  // run threads
  for(t=0; t<num_tasks; t++){
    // test whether extra work still needs to be done
    if(t < extra){
      end_task++;
    }
    
    futures.push_back(async(launch::async, func, start_task, end_task)); 
    
    start_task = end_task;
    end_task = start_task + chunksize;
  }
  
  for(future<bool> &f: futures){
    f.get();
  }
}

void _execute_Start_End(std::function<bool (long, long)> func, long start, long end){
  long N = end-start;
  
  if(N==0){
    // no need to execute the function
    return;
  }
  
  long num_threads = get_num_threads(N);

  _execute(func, start, num_threads, N);
}


/*====================================================================*/
// Jacobi Rotations

template <typename T>
void copy_Diagonals_to_1D(T *d, T **A, long N){
  auto code = [d, A](long start_task, long end_task)
  {
    long i;
    for(i=start_task; i<end_task; i++){
      d[i] = A[i][i];
    }
    
    return true;
  };

  _execute(code, 0, get_num_threads(N), N);
}

template <typename T>
T get_Max_Off_Diagonal(T **A, long *k, long *l, long N){
  using namespace std;
  
  auto code = [A, N](long start_task, long end_task) -> struct Container<T>
  {
    long i=0,j=1;
    
    
    struct Container<T> max = _init_Container(0.0, 0, 1);
    for(i=start_task; i<end_task; i++){
      for(j=i+1; j<N; j++){
        if(fabs(A[i][j]) >= max.val){
          max.val = fabs(A[i][j]);
          max.k = i;
          max.l = j;
        }
      }
    }
    
    return max;
  };

  //----------------------------
  // parallel part
  // can't be pulled out otherwise there will be instantiation issues
  vector<future<struct Container<T>>> futures;
  struct Container<T> max = _init_Container((T)0.0, 0, 1);
  
  
  int t, extra; // extra will at most be as big as num_tasks-1
  long chunksize, start, end, start_task, end_task, num_tasks, num_elems;
  start = 0;
  // N-1 because the last row has no off-diagonal elem to the right of 
  // the diagonal elem
  end = N-1;
  num_elems = end-start;
  num_tasks = get_num_threads(num_elems);
  
  // do sequentially if N is small
  if(num_tasks < THRESHOLD){
    max = code(start, end);
    *k = max.k;
    *l = max.l;
    return max.val;
  }
  
  // split work
  chunksize = num_elems / num_tasks;
	extra = num_elems % num_tasks; 
	start_task = start;
	end_task = start+chunksize;
  
  // run threads
  for(t=0; t<num_tasks; t++){
    // test whether extra work still needs to be done
    if(t < extra){
      end_task++;
    }
    
    futures.push_back(async(launch::async, code, start_task, end_task)); 
    
    
    start_task = end_task;
    end_task = start_task + chunksize;
  }
  
  
  
  struct Container<T> tmp;
  for(future<struct Container <T>> &f: futures){
    tmp = f.get();
    
    if(tmp.val > max.val){
      max = tmp;
    }
  }
  

  
  *k = max.k;
  *l = max.l;
  return max.val;
}

template <typename T>
void rotate(T **A, T *d, T **V, long k, long l, long N){
  T diff, phi, tau, tmp, t, c, s;
  
  diff = d[l] - d[k];
  if(fabs(A[k][l]) < fabs(diff)*EPS){
    t = A[k][l]/diff;
  }else{
    phi = diff/(2.0*A[k][l]);
    t = 1.0/(fabs(phi) + std::sqrt(phi*phi + 1.0));
    if(phi < 0.0){
        t = -t;
    }
  }
  c = 1.0/std::sqrt(t*t + 1.0); 
  s = t*c;
  tau = s/(1.0 + c);
  tmp = A[k][l];
  A[k][l] = 0.0;
  d[k] = d[k] - t*tmp;
  d[l] = d[l] + t*tmp;
  
  
  // case of i < k
  auto code_ik = [A, k, l, s, tau](long start_task, long end_task)
  {
    long i;
    T tmp;
    for(i=start_task; i<end_task; i++){
      tmp = A[i][k];
      A[i][k] = tmp - s*(A[i][l] + tau*tmp);
      A[i][l] = A[i][l] + s*(tmp - tau*A[i][l]);
    }
    return true;
  };
  
  // case of k < i < l
  auto code_kil = [A, k, l, s, tau](long start_task, long end_task)
  {
    long i;
    T tmp;
    for(i=start_task; i<end_task; i++){
      tmp = A[k][i];
      A[k][i] = tmp - s*(A[i][l] + tau*tmp);
      A[i][l] = A[i][l] + s*(tmp - tau*A[i][l]);
    }
    return true;
  };
  
  // case of i > l
  auto code_il = [A, k, l, s, tau, N](long start_task, long end_task)
  {
    long i;
    T tmp;
    for(i=start_task; i<end_task; i++){
      tmp = A[k][i];
      A[k][i] = tmp - s*(A[l][i] + tau*tmp);
      A[l][i] = A[l][i] + s*(tmp - tau*A[l][i]);
    }
    return true;
  };
  
  // update transformation matrix
  auto code_update = [V, k, l, s, tau, N](long start_task, long end_task)
  {
    long i;
    T tmp;
    for(i=start_task; i<end_task; i++){
      tmp = V[i][k];
      V[i][k] = tmp - s*(V[i][l] + tau*tmp);
      V[i][l] = V[i][l] + s*(tmp - tau*V[i][l]);
    }
    return true;
  };

  _execute_Start_End(code_ik, 0, k);
  _execute_Start_End(code_kil, k+1, l);
  _execute_Start_End(code_il, l+1, N);
  _execute_Start_End(code_update, 0, N);
}


template <typename T>
long c11_jacobi_eigen(T **A, long N, T *d, T **V, long max_iterations, T epsilon){
  long i, k, l;
  T max;
  
  init_Identity(V, N);
  
  copy_Diagonals_to_1D(d, A, N);
  
  for(i=0; i<max_iterations; i++) {
    max = get_Max_Off_Diagonal(A, &k, &l, N);
    
    if(max<epsilon){
      // break when close enough
      return i;
    }
    
    rotate(A, d, V, k, l, N);
  }
  
  return -1;
}

#endif
