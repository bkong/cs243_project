#include <stdio.h>
#include <stdlib.h>

#define SIZE 300

int **mkmatrix(int rows, int cols) {
    int i, j, count = 1;
    int **m = new int*[rows];
    for (i=0; i<rows; i++) {
		m[i] = new int[cols];
		for (j=0; j<cols; j++) {
		    m[i][j] = 0;
		}
    }
    return(m);
}

int *mkvector(int cols) {
    int i, j, count = 1;
    int* m = new int[cols];
    for (i=0; i<cols; i++) {
	    m[i] = 0;
    }
    return(m);
}

void freematrix(int rows, int **m) {
    while (--rows > -1) { delete[](m[rows]); }
   	delete[] (m);
}

int main(int argc, char *argv[]) {
    int n = ((argc == 2) ? atoi(argv[1]) : 1);

    int** A = mkmatrix(SIZE, SIZE);
    int** B = mkmatrix(SIZE, SIZE);
    int** C = mkmatrix(SIZE, SIZE);
    int*  X = mkvector(SIZE);
    int*  Y = mkvector(SIZE);

	int i, j, k, l;

    for (l=0; l<n; l++) {

		/* initialization */
		for (i=0; i<SIZE; ++i) {
			X[i] = Y[i] = 2;
			for (j=0; j<SIZE; ++j) {
				A[i][j] = B[i][j] = C[i][j] = 2;
			}
		}

		/* 5-pointed system */
		for (i=1; i<SIZE-1; ++i)
			for(j=1; j<SIZE-1; ++j)
				A[i][j] = (4*A[i][j] + A[i-1][j] + A[i+1][j]
							+ A[i][j-1] + A[i][j+1])/8;

		/* 9-pointed system */
		for (i=1; i<SIZE-1; ++i)
			for (j=1; j<SIZE-1; ++j)
				A[i][j] = (8*A[i][j] + A[i-1][j] + A[i+1][j]
							+ A[i][j-1] + A[i][j+1] + A[i-1][j+1]
							+ A[i+1][j+1] + A[i-1][j-1]
							+ A[i+1][j-1])/16;

		/* Matrix-Vector Multiply */
		for (i=0; i<SIZE; ++i)
			for (j=0; j<SIZE; ++j)
				Y[i] += A[i][j] * X[j];

		/* Matrix-Matrix Multiply */
		for (i=0; i<SIZE; ++i)
			for (j=0; j<SIZE; ++j)
				for (k=0; k<SIZE; ++k)
					C[i][j] += A[i][k] * B[k][j];

    }

	printf("%d %d\n", Y[2], C[2][3]);

    freematrix(SIZE, A);
    freematrix(SIZE, B);
    freematrix(SIZE, C);
    delete[] X;
    delete[] Y;

    return(0);
}

