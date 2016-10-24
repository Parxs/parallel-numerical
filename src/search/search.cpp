#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <boost/optional.hpp>

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
#ifndef ITERATIONS
#define ITERATIONS 1
#endif

using namespace std;


int main(int argc, char* argv[]){
	unsigned long *exec_times;
  
  Input_Container cont = get_Arguments(argc, argv);
  if(cont.help_needed){
    print_help(argv[0]);
    exit(0);
  }
  long N, seed, iterations;
  seed = (cont.seed) ? (*cont.seed) : (time(NULL));
  srand(seed);
  iterations = (cont.iterations) ? (*cont.iterations) : ITERATIONS;
  
  N = (cont.N) ? (*cont.N) : (SIZE);
  
  int find = (cont.X) ? (*cont.X) : SEARCH_FOR;
  
  exec_times = new unsigned long[iterations];
  
  
  cout << "\n=================================================" << "\n";
  cout << "Sorting:" << "\n";
  cout << "N = " << N << "\n";
  if(MIN<MAX){
    cout << "Bounds (lower/upper) = " << MIN << "/" << MAX << "\n";
  }
  cout << "Searching for: " << find << "\n";
  cout << "Seed = " << seed << "\n";
  if(iterations > 1){
    cout << "Sample-Size = " << iterations << "\n";
  }
  cout << 	"=================================================" << "\n\n";
  
  
  // Initialization
	int* list = new int[N];
  
  for(long i=0; i<iterations; i++){
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
  
  cout << "avg. Time: \t" << get_Average(exec_times, iterations) << " ms\n\n";
  
  // Finalization
  delete[] list;
  
  delete[] exec_times;
	
	return EXIT_SUCCESS;
}
