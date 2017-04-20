#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
#include "helpers.h"

using namespace std;

// Usage: ./loop flag nloops
void deprep(long long int*A, int*P, long long int*O, int n);

void rand_ints(long long int*A, int len, long long int min, long long int max);

void rand_soln(int*S, int len, int min, int max);

void rand_neighbor(int *S, int *nS, int min, int max);

int main(int argc, char* argv[]) {
	srand(time(NULL));
	int flag = atoi(argv[1]);  // Debugging flag
	int trials = atoi(argv[2]);  // Number of diff random int arrays
	int iterations = atoi(argv[3]);  // For heuristics allowing for multiple runs, number of iters for each trial

	// TODO: timing

	// Allocate prepartitioning arrays, int arrays, and temperature array
	int *S = (int*) malloc(ARR_SIZE * sizeof(int)),
		*nS = (int*) malloc(ARR_SIZE * sizeof(int)),
		*nnS = (int*) malloc(ARR_SIZE * sizeof(int));
	long long int *A = (long long int*) malloc(ARR_SIZE * sizeof(long long int)),
		*SO = (long long int*) malloc(ARR_SIZE * sizeof(long long int)),
		*nSO = (long long int*) malloc(ARR_SIZE * sizeof(long long int)),
		*nnSO = (long long int*) malloc(ARR_SIZE * sizeof(long long int));
	float *T_iter = (float *) malloc(iterations * sizeof(float));

	long long int S_resid = MAX,
				  nS_resid = MAX,
	 			  nnS_resid = MAX;

	// Preprocess T_iter array
	for (int i = 0; i<iterations; i++) {
		T_iter[i] = 10000000000 * pow(.8, i / 300);
	}

	for (int t = 0; t < trials; t++){
		rand_ints(A, ARR_SIZE, 1, MAX);
		S_resid = MAX;
		nS_resid = MAX;
		nnS_resid = MAX;

		// Random Solution
		rand_soln(S, ARR_SIZE, 0, ARR_SIZE);
		for (int t = 0; t < iterations; t++) {
			rand_soln(nS, ARR_SIZE, 0, ARR_SIZE);

			deprep(A,nS,nSO,ARR_SIZE);

			nS_resid = karmarkar_karp(nSO, ARR_SIZE);

			if (nS_resid < S_resid) {
				memcpy(S, nS, ARR_SIZE * sizeof(int));
				S_resid = nS_resid;
			}
		}
		//TODO: format return

		// Hill Climbing
		rand_soln(S, ARR_SIZE, 0, ARR_SIZE);
		for (int i = 0; i < iterations; i++) {

			rand_neighbor(nS, S, 1, ARR_SIZE);
			deprep(A,nS,nSO,ARR_SIZE);

			nS_resid = karmarkar_karp(nSO, ARR_SIZE);

			if (nS_resid < S_resid) {
				memcpy(S, nS, ARR_SIZE * sizeof(int));
				S_resid = nS_resid;
			}
		}
		//TODO: format return

		// Simulated Annealing
		rand_soln(S, ARR_SIZE, 0, ARR_SIZE);
		memcpy(nnS, S, sizeof(int) * ARR_SIZE);
		deprep(A, S, SO, ARR_SIZE);
		S_resid = karmarkar_karp(SO,ARR_SIZE);
		nnS_resid = S_resid;
		for (int i = 0; i < iterations; i++){
			rand_neighbor(nS, S, 1, ARR_SIZE);

			deprep(A, nS, nSO, ARR_SIZE);
			deprep(A, nnS, nnSO, ARR_SIZE);

			nS_resid = karmarkar_karp(nSO,ARR_SIZE);
			nnS_resid = karmarkar_karp(nnSO,ARR_SIZE);

			if(nS_resid < S_resid){
				memcpy(S, nS, sizeof(int) * ARR_SIZE);
				S_resid = nS_resid;
			}
			else {
				float p = exp((-nS_resid - S_resid) / T_iter[i]);
				float d = rand() / RAND_MAX;
				if (d <= p){
					memcpy(S, nnS, sizeof(int) * ARR_SIZE);
					S_resid = nnS_resid;
				}

			}

			if (S_resid < nnS_resid){
				memcpy(nnS, S, sizeof(int) * ARR_SIZE);
			}
		}
		//TODO: format return
	}
free(A);free(S);free(nS);free(nnS);free(SO);free(nSO);free(nnSO);free(T_iter);
}

void deprep(long long int*A, int*P, long long int*O, int len) {
	for (int i = 0; i < len; i++) {
		O[i] = 0;
	}
	
	for (int i = 0; i < len; i++) {
		int idx = P[i];
		O[idx] += A[i];
	}
}

void rand_ints(long long int*A, int len, long long int min, long long int max) {
	for (int i = 0; i < len; i++) {
		int r = min + rand() / (RAND_MAX / (max - min + 1) + 1);
		A[i] = r;
	}
}

void rand_soln(int*S, int len, int min, int max) {
	for (int i = 0; i < len; i++) {
		int r = min + rand() / (RAND_MAX / (max - min + 1) + 1);
		S[i] = r;
	}
}

void rand_neighbor(int *S, int *nS, int min, int max){
	int i = min + rand() / (RAND_MAX / (max - min + 1) + 1);
	int j = min + rand() / (RAND_MAX / (max - min + 1) + 1);
	do {
		int j = min + rand() / (RAND_MAX / (max - min + 1) + 1);
	}
	while (S[i] == j);
	nS[i] = j;
}
