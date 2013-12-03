//test the performance of setting values in an array by accessing elements randomly

#include <time.h>
#include <stdlib.h>


int main(){
	//200 megs
	#define arrSize 200000000
	srand(time(NULL));
	
	int* arr = malloc(sizeof(int)*arrSize);
	
	int i;
	for(i=0; i<arrSize; i++){
		int r = rand()%arrSize;
		arr[r] = i;
		
		}
		
}
