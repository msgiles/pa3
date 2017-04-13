#include <stdio>
#include "helpers.h"

int karmarkar_karp(int*A, int n) {
	// TODO: fill in heap stuff
	/*
	H = makeheap(A);
	do {
		e1 = H.deletemax();
		if (H.isemtpy()) {
			e2 = H.deletemax();
			H.insert(e1 - e2);
		}
		else {
			return e1;
		}
	}
	while (!(H.isempty()))
	*/

	// This works, right?
	return 1;
}

void deprep(int*A, int*P, int*O, int n) {
	for (int i = 0; i < n; i++) {
		int idx = P[i];
		O[idx] += A[i];
	}
}

void rand_array(int*A, int n, int min, int max) {
	for (int i = 0; i < n; i++) {
		O[i] = 0;
	}
	
	srand(time(NULL));
	for (int i = 0; i<n; i++) {
		int r = min + rand() / (RAND_MAX / (max - min + 1) + 1)
		A[i] = r;
	}
}