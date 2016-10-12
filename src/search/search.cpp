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

using namespace std;


int main(int argc, char* argv[]){
	
  
	long* args = handle_Input(argc, argv);
	long N = (args[1] == 0) ? SIZE : args[1];
  long seed = (args[5] == 0) ? time(NULL) : args[5];
  srand(seed);
  int find = (int)((args[6] == 0) ? SEARCH_FOR : args[6]);
  
 
	delete[](args);
  
  cout << endl << "=================================================" << endl;
  cout << "Sorting:" << endl;
  cout << "N = " << N << endl;
  if(MIN<MAX){
    cout << "Bounds (lower/upper) = " << MIN << "/" << MAX << endl;
  }
  cout << "Searching for: " << find << endl;
  cout << "Seed = " << seed << endl;
  cout << 	"=================================================" << endl << endl;
  
  
  // Initialization
	int* list = new int[N];
	init_1D(list, N, MIN, MAX);
  
  // to ensure the element is in the list
  list[0] = find;
	
  std::sort(list, list+N);
  
  
  // Execution
  
	unsigned long exec_time;
	exec_time = search(list, N, find);
	cout << "Search time: \t" << exec_time << "ms" << endl << endl;
  
  // print_list_1D(list, N);
  
  
  // Finalization
  delete[](list);
	
	cout << endl << endl;
	return EXIT_SUCCESS;
}
