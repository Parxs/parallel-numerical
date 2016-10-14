#include <iostream>
#include <cstdlib>
#include <algorithm>

#include "util.h"
#include "array.h"
#include "search.h"

#define SIZE 1024

#ifndef SEARCH_FOR
#define SEARCH_FOR rand()
#endif
#ifndef MIN
#define MIN 1
#endif
#ifndef MAX
#define MAX 100
#endif
#ifndef REPEATS
#define REPEATS 1
#endif

using namespace std;


int main(int argc, char* argv[]){
	unsigned long *exec_times;
  
	long* args = handle_Input(argc, argv);
	long N = (args[1] == 0) ? SIZE : args[1];
  long seed = (args[5] == 0) ? time(NULL) : args[5];
  srand(seed);
  int find = (int)((args[6] == 0) ? SEARCH_FOR : args[6]);
  
  long repeats = args[8] == 0 ? REPEATS : args[8];
	
  
  exec_times = new unsigned long[repeats];
 
	delete[](args);
  
  cout << "\n=================================================" << "\n";
  cout << "Sorting:" << "\n";
  cout << "N = " << N << "\n";
  if(MIN<MAX){
    cout << "Bounds (lower/upper) = " << MIN << "/" << MAX << "\n";
  }
  cout << "Searching for: " << find << "\n";
  cout << "Seed = " << seed << "\n";
  if(repeats > 1){
    cout << "Sample-Size = " << repeats << "\n";
  }
  cout << 	"=================================================" << "\n\n";
  
  
  // Initialization
	int* list = new int[N];
  
  for(long i=0; i<repeats; i++){
      init_1D(list, N, MIN, MAX);
      
      std::sort(list, list+N);
      
      // Execution
      long index;
      exec_times[i] = search(list, N, find, &index);
    
#ifdef DEBUG
      if(index < 0){
        for(long j=0; j<N; j++){
          if(list[j] == find){
            cout << "> ERROR: list holds element but it was not found!\n\n";
          }
        }
        
      }else if(list[index] != find){
        cout << "> ERROR: returned index points to an element which is not the searched element!\n\n";
      }
#endif
  }
  
  cout << "avg. Time: \t" << get_Average(exec_times, repeats) << " ms\n\n";
  
  // Finalization
  delete[] list;
  
  delete[] exec_times;
	
	return EXIT_SUCCESS;
}
