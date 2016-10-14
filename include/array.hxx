#ifndef ARRAY_HXX
#define ARRAY_HXX

#include "util.h"

#include <iostream>

// Allocating
/**
 * @brief Allocates the second dimension of a 2D array.
 * 
 * Uses calloc so elements will be initialized.
 **/
template <typename T>
void allocate_2D(T** A, long M, long N){
  long i;
  for(i=0; i<M; i++){
    A[i] = new T[N];
  }
}

template <typename T>
void allocate_2D(T** A, long N){
  allocate_2D(A, N, N);
}


// Finalizing
/**
 * @brief Frees the second dimension of a given array & the array itself.
 **/
template <typename T>
void delete_2D(T** A, long N){
  long i;
  for(i=0; i<N; i++){
    delete[] A[i];
  }
  delete[] A;
}


// Initializing
/**
 * @brief Initializes an 2D array with postive random numbers.
 **/
template <typename T>
void init_2D(T** A, long M, long N, T lower, T upper){
  long i, j;
  for(i=0; i<M; i++){
    for(j=0; j<N; j++){
      A[i][j] = get_rand(lower, upper);
    }
  }
}

/**
 * @brief Initializes an 1D array with postive random numbers.
 **/
template <typename T>
void init_1D(T* A, long N, T lower, T upper){
  long i;
  for(i=0; i<N; i++){
    A[i]= get_rand(lower, upper);
  }
}

template <typename T>
void init_Identity(T **A, long N){
  long i,j;
  for(i=0; i<N; i++){
    for(j=0; j<N; j++){
      // fill with 0s
      A[i][j] = (T)0;
    }
    // overwrite diagonal with 1s
    A[i][i] = (T)1;
  }
  
}

// Utility
// Filling
/**
 * @brief Fills an 2D array with the given element.
 **/
template <typename T>
void fill_2D(T** A, long M, long N, T elem){
  long i,j;
  for(i=0; i<M; i++){
    for(j=0; j<N; j++){
      A[i][j] = elem;  
	}
  }
}

/**
 * @brief Fills an 1D array with the given element.
 **/
template <typename T>
void fill_1D(T* A, long N, T elem){
  long i;
  for(i=0; i<N; i++){
    A[i] = elem;  
  }
}

// Copying
template <typename T>
void copy_1D(T* A, T* B, long N){
  long i;
  for(i=0; i<N; i++){
    B[i] = A[i];
  }
}

template <typename T>
void copy_2D(T** A, T** B, long M, long N){
  long i, j;
  for(i=0; i<M; i++){
    for(j=0; j<N; j++){
      B[i][j] = A[i][j];
    }
  }
}

/**
 * @brief Copies elements from a 2D array to an 1D array.
 *
 * It is assumed that both arrays can hold the same amount of
 * elements.
 **/
template <typename T>
void copy_2D_to_1D(T** A, T* B, long M, long N){
  long i, j;
  for(i=0; i<M; i++){
    for(j=0; j<N; j++){
      B[i*N+j] = A[i][j]; 
    }
  }
}


// Comparison
/**
 * @brief Tests whether two 2D matrices are equal and prints an error if they are not.
 **/
template <typename T>
bool compare_2D(T **A, T **B, long M, long N){
  long i, j;
  for(i=0; i<M; i++){
    for(j=0; j<N; j++){
	   if(A[i][j] != B[i][j]){
	      return false;
	   }
	}   
  }
  return true;
}

/**
 * @brief Tests whether two 1D matrices are equal and prints an error if they are not.
 **/
template <typename T>
bool compare_1D(T *A, T *B, long N){
  long i;
  for(i=0; i<N; i++){
     if(A[i] != B[i]){
        return false;
     }
  } 
  return true;  	
}

template <typename T>
bool compare_2D(T **A, T **B, T eps, long M, long N){
  long i, j;
  for(i=0; i<M; i++){
    for(j=0; j<N; j++){
      if(fabs(A[i][j]-B[i][j]) > eps){
#ifdef DEBUG
        log_Difference(std::cout,A[i][j], B[i][j]);
         
#endif
        
        return false;
      }
    }   
  }
  return true;
}

template <typename T>
bool compare_1D(T *A, T *B, T eps, long N){
  long i;
  for(i=0; i<N; i++){
     if(fabs(A[i]-B[i]) > eps){
        return false;
     }
  } 
  return true;  
}



// Sorting
template <typename T>
bool is_sorted(T* list, long N){
	T last = list[0];
  long i;
	for(i=1; i<N; i++){
		if(last>list[i]){
			return false;
		}
    last = list[i];
	}
	return true;
}

// Printing
template <typename T>
void print_list_1D(T* list, long N){
  std::cout << std::showpos << std::scientific;
  long i;
  for(i=0; i<N; i++){
    std::cout << list[i] << "\t";
    if((i+1)%10 == 0){ // because list starts with 0 not 1
      std::cout << "\n";
    }
  }
  std::cout << "\n" << std::endl;
  std::cout << std::noshowpos;
}
//TODO: temp fix - replace name of calls with lowercase list in the future
template <typename T>
void print_List_1D(T* list, long N){
  print_list_1D(list, N);
}

template <typename T>
void print_List_2D(T** list, long M, long N){
  std::cout << std::showpos << std::scientific;
  long i,j;
  for(i=0; i<M; i++){
    for(j=0; j<N; j++){
      std::cout << list[i][j];
      std::cout << "\t";
    }
    std::cout << "\n";
  }
  std::cout << "\n" << std::endl;
  std::cout << std::noshowpos;
}

template <typename T>
void print_List_2D(T** list, long N){
  print_List_2D(list, N, N);
}


// Swapping
template <typename T>
void swap(T* A, long i, long j){
  T tmp = A[i];
  A[i] = A[j];
  A[j] = tmp;
}
#endif // ARRAY_HXX
