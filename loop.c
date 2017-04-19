#include <stdio.h>
#include "helpers.h"

// Usage: ./loop flag nloops
void deprep(int*A, int*P, int*O, int n);

void rand_arr(int*A, int len, int min, int max);

void rand_neighbor(long long int *S, long long int *nS);

int main(int argc, char* argv[]) {
	srand(time(NULL));
	int flag = atoi(argv[1]);
	int trials = atoi(argv[2]);
	int iterations = atoi(argv[3]);

	// TODO: timing

	// Allocate prepartitioning arrays, int arrays, and temperature array
	long long int *S = malloc(ARR_SIZE, sizeof(int));
		*nS = malloc(ARR_SIZE, sizeof(int));
		*nnS = malloc(ARR_SIZE, sizeof(int));
		*A = malloc(ARR_SIZE, sizeof(long long int));
		*SO = malloc(ARR_SIZE, sizeof(long long int));
		*nSO = malloc(ARR_SIZE, sizeof(long long int));
		*nnSO = malloc(ARR_SIZE, sizeof(long long int));
		*T_iter = malloc(iterations, sizeof(float));

	long long int S_resid = MAX;
				  nS_resid = MAX;
	 			  nnS_resid = MAX;

	// Preprocess T_iter array
	for (int i = 0, i < iterations, i++){
		T_iter[i] = 10000000000 * pow(.8, i / 300);
	}

	for (int t = 0; t < trials; t++){
		rand_arr(A, ARR_SIZE, 1, MAX);
		S_resid = MAX;
		nS_resid = MAX;
		nnS_resid = MAX;

		// Random Solution
		rand_arr(S, ARR_SIZE, 0, ARR_SIZE);
		for (int t = 0; i < iterations; i++) {
			rand_arr(nS, ARR_SIZE, 0, ARR_SIZE);

			deprep(A,nS,nSO);

			nS_resid = karmarkar_karp(nSO, ARR_SIZE);

			if (nS_resid < S_resid) {
				memcpy(S, nS, ARR_SIZE * sizeof(int));
				S_resid = nS_resid;
			}
		}
		//TODO: format return

		// Hill Climbing
		rand_arr(S, ARR_SIZE, 0, ARR_SIZE);
		for (int i = 0; i < iterations; i++) {

			rand_neighbor(nS, S, 1, ARR_SIZE);
			deprep(A,nS,nSO);

			nS_resid = karmarkar_karp(nSO, ARR_SIZE);

			if (nS_resid < S_resid) {
				memcpy(S, nS, ARR_SIZE * sizeof(int));
				S_resid = nS_resid;
			}
		}
		//TODO: format return

		// Simulated Annealing
		rand_arr(S, ARR_SIZE, 0, ARR_SIZE);
		memcpy(nnS, S, sizeof(int) * ARR_SIZE);
		deprep(A, S, O);
		S_resid = karmarkar_karp(O);
		nnS_resid = S_resid;
		for (int i = 0; i < iterations; i++){
			rand_neighbor(nS, S, 1, ARR_SIZE);

			deprep(A, nS, nSO);
			deprep(A, nnS, nnSO);

			nS_resid = karmarkar_karp(nSO);
			nnS_resid = karmarkar_karp(nnSO);

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

}

void deprep(long long int*A, long long int*P, long long int*O, int n) {
	for (int i = 0; i < len; i++) {
		O[i] = 0;
	}
	
	for (int i = 0; i < n; i++) {
		int idx = P[i];
		O[idx] += A[i];
	}
}

void rand_arr(long long int*A, int len, long long int min, long long int max) {
	for (int i = 0; i<n; i++) {
		long long int r = min + rand() / (RAND_MAX / (max - min + 1) + 1)
		A[i] = r;
	}
}

void rand_neighbor(long long int *S, long long int *nS, int min, int max){
	int i = min + rand() / (RAND_MAX / (max - min + 1) + 1);
	do {
		int j = min + rand() / (RAND_MAX / (max - min + 1) + 1);
	}
	while (S[i] == j);
	nS[i] = j;
}
