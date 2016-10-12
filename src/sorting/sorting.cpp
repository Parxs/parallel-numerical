#include <iostream>
#include <cstdlib>

#include "util.h"
#include "array.h"
#include "sorting.h"

#ifndef SIZE
#define SIZE 1024
#endif


using namespace std;


int main(int argc, char* argv[]){
	
  
	long* args = handle_Input(argc, argv);
	long N = (args[1] == 0) ? SIZE : args[1];
  long seed = (args[5] == 0) ? time(NULL) : args[5];
  
  srand(seed);
	delete[](args);
  
  cout << endl << "=================================================" << endl;
  cout << "Sorting:" << endl;
  cout << "N = " << N << endl;
  if(MIN<MAX){
    cout << "Bounds (lower/upper) = " << MIN << "/" << MAX << endl;
  }
  cout << "Seed = " << seed << endl;
  cout << 	"=================================================" << endl << endl;
  
  
  // Initialization
	int* list = new int[N];
  int* sorted_list = new int[N];
	init_1D(list, N, MIN, MAX);

#ifdef DEBUG
  //print_list_1D(list, N);
  if(is_sorted(list, N)){
    cout << "> ATTENTION: list is already sorted." << endl << endl;
  }
#endif
	

	
  // Execution
  
	unsigned long exec_time;
	exec_time = sort_List(list, sorted_list, N);
	cout << "Sorting time: \t" << exec_time << "ms" << endl << endl;
  
#ifdef DEBUG
  if(!is_sorted(sorted_list, N)){
    cout << "> ERROR: sorting the list failed" << endl;
    
    print_list_1D(sorted_list, N);
    delete[](list);
    delete[](sorted_list);
    return EXIT_FAILURE;
  }else{
    cout << "> List has been successfully sorted." << endl;
    //print_list_1D(sorted_list, N);
  }
#endif
  
  // Finalization
  delete[](list);
  delete[](sorted_list);
	
	cout << endl << endl;
	return EXIT_SUCCESS;
}
