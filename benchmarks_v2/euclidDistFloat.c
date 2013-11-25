//test performance reading a large collection of vectors with float values and calculating the squared euclidiean length.
#include <stdio.h>
#include <math.h>


int main(){
	FILE *myFile;
	double myvariable;
	int i;
	int j;
	
	#define num_vecs 1000000
	#define vec_dimension 3
	
	//since telling c to "read to end of file" or "until end of line" is a real pain, just manually coding the size of the file and number of dimensions
	
	
	myFile = fopen("data/vectorsFloat.txt", "r");
	for(i=0; i<num_vecs; i++)
	{
		double vals[vec_dimension];
		double* arrPt = vals;
		for(j=0; j< vec_dimension; j++)
		{
			fscanf(myFile, "%lf", arrPt);
			arrPt++;
		}
		float answer = vals[0]*vals[0] + vals[1]*vals[1]+vals[2]*vals[2];
		//printf("%lf\n", answer);
	}
}
