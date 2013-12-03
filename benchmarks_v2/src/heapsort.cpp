#include <iostream>
#include <stdlib.h>

using namespace std;

void heapsort(int n, int* ra) {
    int i, j;
    int ir = n;
    int l = (n >> 1) + 1;
    int rra;

    while (true) {
	if (l > 1) {
	    rra = ra[--l];
	} else {
	    rra = ra[ir];
	    ra[ir] = ra[1];
	    if (--ir == 1) {
		ra[1] = rra;
		return;
	    }
	}
	i = l;
	j = l << 1;
	while (j <= ir) {
	    if (j < ir && ra[j] < ra[j+1]) { ++j; }
	    if (rra < ra[j]) {
		ra[i] = ra[j];
		j += (i = j);
	    } else {
		j = ir + 1;
	    }
	}
	ra[i] = rra;
    }
}

const long long A = 16807;
const long long M = 2147483647;
long long X;

void isrand(int r) {
    X = r;
}

int irand() {
	X = (A * X) % M;
	return (int) X;
}


int main(int argc, char *argv[]) {
    int N = ((argc == 2) ? atoi(argv[1]) : 1);
    int* ary;
    int i;

	ary = new int[N+1];
    isrand(123457);
	for (i=1; i<=N; i++) {
		ary[i] = irand();
	}

    heapsort(N, ary);

	cout << "Need to add test sort!" << endl;
    cout << ary[N] << endl;

    delete[] ary;
    return(0);
}


