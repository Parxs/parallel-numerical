#include <iostream>
#include <cstdlib>
#include <boost/optional.hpp>

#include "util.h"
#include "array.h"
#include "sorting.h"

#ifndef SIZE
#define SIZE 1024
#endif
#ifndef ITERATIONS
#define ITERATIONS 1
#endif

using namespace std;


int main(int argc, char* argv[]){
  
  Input_Container cont = get_Arguments(argc, argv);
  if(cont.help_needed){
    print_help(argv[0]);
    exit(0);
  }
  long N, iterations;
  if(cont.seed){
    util::mt.seed(*cont.seed);
  }
  iterations = (cont.iterations) ? (*cont.iterations) : ITERATIONS;
  
  N = (cont.N) ? (*cont.N) : (SIZE);
  
  unsigned long *exec_times = new unsigned long[iterations];
  
  cout << "\n=================================================" << "\n";
  cout << "Sorting:" << "\n";
  cout << "N = " << N << "\n";
  if(MIN<MAX){
    cout << "Bounds (lower/upper) = " << MIN << "/" << MAX << "\n";
  }
  if(cont.seed){
    cout << "Seed = " << *cont.seed << "\n";
  }
  if(iterations > 1){
    cout << "Sample-Size = " << iterations << "\n";
  }
  cout << 	"=================================================" << "\n\n";
  
  
  // Initialization
	int* list = new int[N];
  int* sorted_list = new int[N];
  
  
  for(long i=0; i< iterations; i++){
    init(list, N, MIN, MAX);

#ifdef DEBUG
    if(is_sorted(list, N)){
      cout << "> ATTENTION: list is already sorted." << "\n\n";
    }
#endif

    // Execution
    exec_times[i] = sort_List(list, sorted_list, N);
    
#ifdef DEBUG
    if(!is_sorted(sorted_list, N)){
      cout << "> ERROR: sorting the list failed" << "\n\n";
    }
#endif
  }

  cout << "avg. Time: \t" << get_Average(exec_times, iterations) << " ms\n\n";
  
  
  // Finalization
  delete[] list;
  delete[] sorted_list;
	
  delete[] exec_times;
  
	return EXIT_SUCCESS;
}
