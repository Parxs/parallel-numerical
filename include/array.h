#ifndef ARRAY_H
#define ARRAY_H

#include "array.hxx"
//----------------------------------------------------------------------
// Allocation
template <typename T>
void allocate_2D(T** A, long M, long N);
template <typename T>
void allocate_2D(T** A, long N);

// Finalizing
template <typename T>
void delete_2D(T** A, long N);

// Initializing
template <typename T>
void init_2D(T** A, long M, long N, T lower, T upper);
template <typename T>
void init_1D(T* A, long N, T lower, T upper);
template <typename T>
void init_Identity(T **A, long N);

// Filling
template <typename T>
void fill_2D(T** A, long M, long N, T elem);
template <typename T>
void fill_1D(T* A, long M, T elem);

// Copying
template <typename T>
void copy_1D(T* A, T* B, long N);
template <typename T>
void copy_2D(T** A, T** B, long M, long N);
template <typename T>
void copy_2D_to_1D(T** A, T* B, long M, long N);

// Comparison
template <typename T>
bool compare_2D(T **A, T **B, long M, long N);
template <typename T>
bool compare_1D(T *A, T *B, long N);
template <typename T>
bool compare_2D(T **A, T **B, T eps, long M, long N);
template <typename T>
bool compare_1D(T *A, T *B, T eps, long N);

// Sorting
template <typename T>
bool is_sorted(T* list, long N);

// Printing
template <typename T>
void print_List(T **list, long N, long M);
template <typename T>
void print_List(T **list, long N);
template <typename T>
void print_List(T *list, long N);

// Swapping
template <typename T>
void swap(T* A, long i, long j);


#endif // ARRAY_H
