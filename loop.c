#include <stdio.h>
#include "helpers.h"

// Usage: ./loop flag nloops
int main(int argc, char* argv[]) {
	int flag = atoi(argv[1]);
	int n = atoi(argv[2]);

	// TODO: timing

	// Allocate solution array and int array
	int *S = malloc(ARR_SIZE, sizeof(int));
		*nS = malloc(ARR_SIZE, sizeof(int));
		*A = malloc(ARR_SIZE, sizeof(int));
		*O = malloc(ARR_SIZE, sizeof(int));
	
	int S_resid = karmarkar_karp(S, ARR_SIZE);
	int nS_resid;

	// Random Solution
	for (int i = 0; i<n; i++) {
		rand_arr(nS, ARR_SIZE, 0, ARR_SIZE);
		rand_arr(A, ARR_SIZE, 1, MAX);

		deprep(A,nS,O);

		nS_resid = karmarkar_karp(O, ARR_SIZE);

		if (nS_resid < S_resid) {
			S = nS;
			S_resid = nS_resid;
		}
	}
	//TODO: format return

	// Hill Climbing

	// Simulated Annealing

}