void main(int argc, char**argv){
	int size = 1000000;
	
	int* arr = malloc(size*sizeof(int));
	
	for(int i=0; i<size; i++)
		arr[i] = i;
}
