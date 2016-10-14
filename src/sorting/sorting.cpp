#include <iostream>
#include <cstdlib>

#include "util.h"
#include "array.h"
#include "sorting.h"

#ifndef SIZE
#define SIZE 1024
#endif
#ifndef REPEATS
#define REPEATS 1
#endif

using namespace std;


int main(int argc, char* argv[]){
	
  
	long* args = handle_Input(argc, argv);
	long N = (args[1] == 0) ? SIZE : args[1];
  long seed = (args[5] == 0) ? time(NULL) : args[5];
  srand(seed);
  
  long repeats = args[8] == 0 ? REPEATS : args[8];
  
  unsigned long *exec_times;
  exec_times = new unsigned long[repeats];
	delete[](args);
  
  cout << "\n=================================================" << "\n";
  cout << "Sorting:" << "\n";
  cout << "N = " << N << "\n";
  if(MIN<MAX){
    cout << "Bounds (lower/upper) = " << MIN << "/" << MAX << "\n";
  }
  cout << "Seed = " << seed << "\n";
  if(repeats > 1){
    cout << "Sample-Size = " << repeats << "\n";
  }
  cout << 	"=================================================" << "\n\n";
  
  
  // Initialization
	int* list = new int[N];
  int* sorted_list = new int[N];
  
  
  for(long i=0; i< repeats; i++){
    init_1D(list, N, MIN, MAX);

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

  cout << "avg. Time: \t" << get_Average(exec_times, repeats) << " ms\n\n";
  
  
  // Finalization
  delete[] list;
  delete[] sorted_list;
	
  delete[] exec_times;
  
	return EXIT_SUCCESS;
}
