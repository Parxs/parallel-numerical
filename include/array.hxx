#ifndef ARRAY_HXX
#define ARRAY_HXX

#include "util.h"

#include <iostream>
#include <random>

// Allocating
/**
 * @brief Allocates the second dimension of a 2D array.
 * @param A array for allocation 
 * @param M first dimension
 * @param N second dimension
 **/
template <typename T>
void allocate(T** A, long M, long N){
  for(long i=0; i<M; i++){
    A[i] = new T[N];
  }
}

/**
 * @brief Allocates the second dimension of a 2D array when both of its dimensions are equal.
 * @param A array to be allocated
 * @param N first and second dimension
 **/
template <typename T>
void allocate(T** A, long N){
  allocate(A, N, N);
}


// Finalizing
/**
 * @brief Frees a 2D array.
 * @param A array to be deallocated
 * @param N second dimension
 **/
template <typename T>
void delete_2D(T** A, long N){
  for(long i=0; i<N; i++){
    delete[] A[i];
  }
  delete[] A;
}


// Initializing
/**
 * @brief Initializes an 2D array with random numbers.
 * @param A array to be initialized
 * @param M first dimension
 * @param N second dimension
 * @param lower lower boundary for the random number
 * @param upper upper boundary for the random number
 **/
template <typename T>
void init(T** A, long M, long N, T lower, T upper){
  for(long i=0; i<M; i++){
    for(long j=0; j<N; j++){
      A[i][j] = get_rand(lower, upper);
    }
  }
}

/**
 * @brief Initializes an 1D array with random numbers.
 * @param A array to be initialized
 * @param N dimension
 * @param lower lower boundary for the random number
 * @param upper upper boundary for the random number
 **/
template <typename T>
void init(T* A, long N, T lower, T upper){
  for(long i=0; i<N; i++){
    A[i]= get_rand(lower, upper);
  }
}

/**
 * @brief Initializes a 2D array with the identity-matrix of NxN.
 * @param A array to be initialized
 * @param N dimension
 **/
template <typename T>
void init_Identity(T **A, long N){
  for(long i=0; i<N; i++){
    for(long j=0; j<N; j++){
      // fill with 0s
      A[i][j] = (T)0;
    }
    // overwrite diagonal with 1s
    A[i][i] = (T)1;
  }
}

/**
 * @brief Initializes the given 2D array with random numbers such that it will be symmetric.
 * @param A array to be initialized
 * @param N dimension
 * @param lower lower boundary for the random number
 * @param upper upper boundary for the random number
 **/
template <typename T>
void init_Symmetric(T **A, long N, T lower, T upper){
  for(long i=0; i<N; i++){
    A[i][i] = get_rand(lower, upper);
    for(long j=i+1; j<N; j++){
      T rand = get_rand(lower, upper);
      A[i][j] = rand;
      A[j][i] = rand;
    }
  }
}


// Utility
// Filling
/**
 * @brief Fills an 2D array with the given element.
 * @param A array to be filled
 * @param M first dimension
 * @param N second dimension
 * @param elem  the element the array should be filled with
 **/
template <typename T>
void fill(T** A, long M, long N, T elem){
  for(long i=0; i<M; i++){
    for(long j=0; j<N; j++){
      A[i][j] = elem;  
    }
  }
}

/**
 * @brief Fills an 1D array with the given element.
 * @param A array to be filled
 * @param N dimension
 * @param elem  the element the array should be filled with
 **/
template <typename T>
void fill(T* A, long N, T elem){
  for(long i=0; i<N; i++){
    A[i] = elem;  
  }
}

/**
 * @brief Reconstructs a symmetric matrix out of its diagonal and lower-triangle matrix A.
 * 
 * All of the values beneath the diagonal will be copied to above the
 * diagonal such that the resulting array is symmetric. The resulting 
 * matrix will be saved in A.
 * @param A array to be reconstructed
 * @param N dimension
 **/
template <typename T>
void reconstruct_Symmetric(T **A, long N){
  long i,j;
  for(i=0; i<N; i++){
    for(j=i+1; j<N; j++){
      A[i][j] = A[j][i];
    }
  }
}

// Copying
/**
 * @brief Copy a 1D array to another 1D one.
 * 
 * It is assumed that the arrays are the same size.
 * @param A source array
 * @param B destination array
 * @param N dimension
 **/
template <typename T>
void copy(T* A, T* B, long N){
  for(long i=0; i<N; i++){
    B[i] = A[i];
  }
}

/**
 * @brief Copy a 2D array to another 2D one.
 * 
 * It is assumed that the arrays are the same size.
 * @param A source array
 * @param B destination array
 * @param M first dimension
 * @param N second dimension
 **/
template <typename T>
void copy(T** A, T** B, long M, long N){
  for(long i=0; i<M; i++){
    for(long j=0; j<N; j++){
      B[i][j] = A[i][j];
    }
  }
}

/**
 * @brief Copies elements from a 2D array to an 1D one.
 *
 * It is assumed that the 1D array is of size M*N.
 * @param A source array
 * @param B destination array
 * @param M first dimension
 * @param N second dimension
 **/
template <typename T>
void copy_to_1D(T** A, T* B, long M, long N){
  for(long i=0; i<M; i++){
    for(long j=0; j<N; j++){
      B[i*N+j] = A[i][j]; 
    }
  }
}


// Comparison
/**
 * @brief Tests whether two 2D matrices are equal.
 * @param A first matrix
 * @param B second matrix
 * @param M first dimension
 * @param N second dimension
 * @return result of comparison
 **/
template <typename T>
bool compare(T **A, T **B, long M, long N){
  for(long i=0; i<M; i++){
    for(long j=0; j<N; j++){
	   if(A[i][j] != B[i][j]){
	      return false;
	   }
	}   
  }
  return true;
}

/**
 * @brief Tests whether two 1D matrices are equal.
 * @param A first matrix
 * @param B second matrix
 * @param N dimension
 * @return result of comparison
 **/
template <typename T>
bool compare(T *A, T *B, long N){
  for(long i=0; i<N; i++){
     if(A[i] != B[i]){
        return false;
     }
  } 
  return true;  	
}

/**
 * @brief Tests whether two 2D matrices are equal up to an epsilon.
 * 
 * Comparing two floating point numbers is problematic because not all
 * numbers can be stored precisely as floating point. Therefore a 
 * certain impreciseness should be allowed and can be expressed via eps.
 * @param A first matrix
 * @param B second matrix
 * @param eps the maximal allowed difference between two elements
 * @param M first dimension
 * @param N second dimension
 * @return result of comparison
 **/
template <typename T>
bool compare(T **A, T **B, T eps, long M, long N){
  for(long i=0; i<M; i++){
    for(long j=0; j<N; j++){
      if(fabs(A[i][j]-B[i][j]) > eps){
        return false;
      }
    }   
  }
  return true;
}

/**
 * @brief Tests whether two 2D matrices are equal up to an epsilon.
 * 
 * Comparing two floating point numbers is problematic because not all
 * numbers can be stored precisely as floating point. Therefore a 
 * certain impreciseness should be allowed and can be expressed via eps.
 * @param A first matrix
 * @param B second matrix
 * @param eps the maximal allowed difference between two elements
 * @param M dimension
 * @return result of comparison
 **/
template <typename T>
bool compare(T *A, T *B, T eps, long N){
  for(long i=0; i<N; i++){
     if(fabs(A[i]-B[i]) > eps){
        return false;
     }
  } 
  return true;  
}



/**
 * @brief Tests whether a given array is sorted.
 * @param A array to be tested
 * @param N dimension
 **/
template <typename T>
bool is_sorted(T* list, long N){
	T last = list[0];
	for(long i=1; i<N; i++){
		if(last>list[i]){
			return false;
		}
    last = list[i];
	}
	return true;
}

// Printing
/**
 * @brief Prints the given one dimensional array.
 * @param list array to be printed
 * @param N dimension
 **/
template <typename T>
void print(T* list, long N){
  std::cout << std::showpos << std::scientific;
  for(long i=0; i<N; i++){
    std::cout << list[i] << "\t";
    if((i+1)%10 == 0){ // i+1 because list starts with 0 not 1
      std::cout << "\n";
    }
  }
  std::cout << "\n" << std::endl;
  std::cout << std::noshowpos;
}

/**
 * @brief Prints the given two dimensional array.
 * @param list array to be printed
 * @param M first dimension
 * @param N second dimension
 **/
template <typename T>
void print(T** list, long M, long N){
  std::cout << std::showpos << std::scientific;
  for(long i=0; i<M; i++){
    for(long j=0; j<N; j++){
      std::cout << list[i][j];
      std::cout << "\t";
    }
    std::cout << "\n";
  }
  std::cout << "\n" << std::endl;
  std::cout << std::noshowpos;
}

/**
 * @brief Prints the given two dimensional array where the dimensions are the same.
 * @param list array to be printed
 * @param N dimension
 **/
template <typename T>
void print(T** list, long N){
  print_List(list, N, N);
}


/**
 * @brief Swaps the two elements with the given index in the given list.
 * 
 * This function has the modifier inline because the function is so 
 * small that it is preferred that the compiler inlines it to avoid 
 * overhead.
 * @param A array where the swapping should happen
 * @param i index of the first element
 * @param j index of the second element
 **/
template <typename T>
inline void swap(T* A, long i, long j){
  T tmp = A[i];
  A[i] = A[j];
  A[j] = tmp;
}
#endif
