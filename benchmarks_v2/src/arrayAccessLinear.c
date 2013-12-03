//test the performance of setting values in an array by accessing elements linearly

#include <time.h>
#include <stdlib.h>


int main(){
	//200 megs
	#define arrSize 200000000
	int* arr = malloc(sizeof(int)*arrSize);
	
	int i;
	for(i=0; i<arrSize; i++)
		arr[i] = i;
		
}
