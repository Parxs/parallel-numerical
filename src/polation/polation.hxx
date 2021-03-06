#ifndef POLATION_HXX
#define POLATION_HXX

#include <iostream>

/**
 * @brief Initializes the values such that a quadratic function is approximated.
 * @param x_values where the values for x should be saved
 * @param y_values where the values for y should be saved
 * @param N size of arrays
 **/
template <typename T>
void init_Quadratic(T *x_values, T *y_values, long N){
  long i;
  for(i=0; i<N; i++){
    x_values[i] = get_rand(MIN, MAX);
    y_values[i] = x_values[i]*x_values[i];
  }
}

/**
 * @brief Runs x through a given polynomial
 * @param x the value which should be run through
 * @param polynomial list holding the exponents
 * @param degree the polynomial should have
 * @return result
 **/
template <typename T>
T apply_poly(T x, T *polynomial, long degree){
  long i;
  T y = polynomial[0];
  T x_n = x;
  // degree=3 refers to ax^3+bx^2+cx^1+dx^0 not ax^2+bx^1+cx^0!
  for(i=1; i<=degree; i++){
    y += x_n*polynomial[i];
    
    // calculate x^(i+1)
    x_n*=x;
  }
  return y;
}

/**
 * @brief Initializes the values such that a polynomial function is approximated.
 * @param x_values where the values for x should be saved
 * @param y_values where the values for y should be saved
 * @param N size of the arrays
 * @param polynomial list holding the exponents
 * @param degree the polynomial should have
 **/
template <typename T>
void init_Polynomial(T *x_values, T *y_values, long N, T *polynomial, long degree){
  long i;
  for(i=0; i<N; i++){
    T rand = get_rand(MIN, MAX);
    x_values[i] = rand;
    y_values[i] = apply_poly(rand, polynomial, degree);
  }
}



template <typename T>
void print_XY(T *x_values, T *y_values, long N){
  using namespace std;
  long i;
  for(i=0; i<N; i++){
    cout << "(" <<  x_values[i] << ")" << "->" << "(" << y_values[i] << ")" << "\t";
    if((i+1)%2 == 0){
      cout << "\n";
    }
  }
  
  cout << endl;
}


#endif //POLATION_HXX
