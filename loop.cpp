#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
#include <time.h> 
#include <string>
#include "helpers.h"

using namespace std;

// Usage: ./loop flag trials iterations
void deprep(long long int*A, int*P, long long int*O, int n);

void rand_ints(long long int*A, int len, long long int min, long long int max);

void rand_soln(int*S, int len, int min, int max);

void rand_neighbor(int *S, int *nS, int min, int max);

int main(int argc, char* argv[]) {
	srand(time(NULL));
	int flag = atoi(argv[1]);  // Debugging flag
	int trials = atoi(argv[2]);  // Number of diff random int arrays
	int iterations = atoi(argv[3]);  // For heuristics allowing for multiple runs, number of iters for each trial

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

	Return kk, random, hill, annealing;

	clock_t t;

	// Preprocess T_iter array
	for (int i = 0; i<iterations; i++) {
		T_iter[i] = 10000000000 * pow(.8, i / 300);
	}

	for (int trial = 0; trial < trials; trial++){
		// cout << "Trial " << trial << endl;
		rand_ints(A, ARR_SIZE, 1, MAX);
		S_resid = MAX;
		nS_resid = MAX;
		nnS_resid = MAX;

		// Karmarkar-Karp
		// cout << "Karmarkar-Karp" << endl;
		t = clock();
		S_resid = karmarkar_karp(A,ARR_SIZE);
		kk.add(clock() - t, S_resid);
		S_resid = MAX;

		// Random Solution
		// cout << "Random" << endl;
		t = clock();
		rand_soln(S, ARR_SIZE, 0, ARR_SIZE-1);
		deprep(A,S,SO,ARR_SIZE);
		S_resid = karmarkar_karp(SO, ARR_SIZE);
		for (int t = 0; t < iterations; t++) {
			rand_soln(nS, ARR_SIZE, 0, ARR_SIZE-1);

			deprep(A,nS,nSO,ARR_SIZE);

			nS_resid = karmarkar_karp(nSO, ARR_SIZE);

			if (nS_resid < S_resid) {
				memcpy(S, nS, ARR_SIZE * sizeof(int));
				S_resid = nS_resid;
			}
		}
		random.add(clock()-t,S_resid);

		// Hill Climbing
		// cout << "Hill" << endl;
		t = clock();
		rand_soln(S, ARR_SIZE, 0, ARR_SIZE-1);
		deprep(A,S,SO,ARR_SIZE);
		S_resid = karmarkar_karp(SO, ARR_SIZE);
		for (int i = 0; i < iterations; i++) {
			// std::string prev = std::to_string(i-1);
			// cout << string(prev.length(), '\b');
			// cout << i << endl;
			rand_neighbor(nS, S, 0, ARR_SIZE-1);
			// cout << "Generated Neighbor!" << endl;
			deprep(A,nS,nSO,ARR_SIZE);
			// cout << "De-Prepartitioned" << endl;

			nS_resid = karmarkar_karp(nSO, ARR_SIZE);
			// cout << "Residual: " << nS_resid << endl;

			if (nS_resid < S_resid) {
				memcpy(S, nS, ARR_SIZE * sizeof(int));
				S_resid = nS_resid;
			}
		}
		hill.add(clock()-t,S_resid);

		// Simulated Annealing
		// cout << "Annealing" << endl;
		t = clock();
		rand_soln(S, ARR_SIZE, 0, ARR_SIZE-1);
		memcpy(nnS, S, sizeof(int) * ARR_SIZE);
		deprep(A, S, SO, ARR_SIZE);
		S_resid = karmarkar_karp(SO,ARR_SIZE);
		nnS_resid = S_resid;
		for (int i = 0; i < iterations; i++){
			rand_neighbor(nS, S, 0, ARR_SIZE-1);

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
				float d = (float) rand() / (float) RAND_MAX;
				if (d <= p){
					memcpy(S, nnS, sizeof(int) * ARR_SIZE);
					S_resid = nnS_resid;
				}

			}

			if (S_resid < nnS_resid){
				memcpy(nnS, S, sizeof(int) * ARR_SIZE);
			}
		}
		annealing.add(clock()-t,nnS_resid);
	}
free(A);free(S);free(nS);free(nnS);free(SO);free(nSO);free(nnSO);free(T_iter);

// Return types
std::string kk_resids ("{"), kk_times ("{"), random_resids ("{"), random_times ("{"), 
hill_resids ("{"), hill_times ("{"), annealing_resids("{"), annealing_times("{");

for (int i = 0; i < trials-1; i++){
	kk_resids.append(std::to_string(kk.resids[i]) + ',');
	kk_times.append(std::to_string(kk.times[i]) + ',');
	random_resids.append(std::to_string(random.resids[i]) + ',');
	random_times.append(std::to_string(random.times[i]) + ',');
	hill_resids.append(std::to_string(hill.resids[i]) + ',');
	hill_times.append(std::to_string(hill.times[i]) + ',');
	annealing_resids.append(std::to_string(annealing.resids[i]) + ',');
	annealing_times.append(std::to_string(annealing.times[i]) + ',');
}

// kk_resids.append(std::to_string(kk.resids[trials-1]) + '}');
// kk_times.append(std::to_string(kk.times[trials - 1]) + '}');
// random_resids.append(std::to_string(random.resids[trials-1]) + '}');
// random_times.append(std::to_string(random.times[trials - 1]) + '}');
// hill_resids.append(std::to_string(hill.resids[trials-1]) + '}');
// hill_times.append(std::to_string(hill.times[trials - 1]) + '}');
// annealing_resids.append(std::to_string(annealing.resids[trials-1]) + '}');
// annealing_times.append(std::to_string(annealing.times[trials - 1]) + '}');


cout << "Karmarkar-Karp:" << endl;
cout << "Residuals:" << endl;
kk.print_resids();
cout << "Times:" << endl;
kk.print_times();
cout << "Random Solutions:" << endl;
cout << "Residuals:" << endl;
random.print_resids();
cout << "Times:" << endl;
random.print_times();
cout << "Hill-Climbing:" << endl;
cout << "Residuals:" << endl;
hill.print_resids();
cout << "Times:" << endl;
hill.print_times();
cout << "Simulated Annealing:" << endl;
cout << "Residuals:" << endl;
annealing.print_resids();
cout << "Times:" << endl;
annealing.print_times();



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
		int r = min + (rand() % (int)(max - min + 1));
		S[i] = r;
	}
}

void rand_neighbor(int *S, int *nS, int min, int max){
	memcpy(nS, S, sizeof(int) * ARR_SIZE);
	int i = min + (rand() % (int)(max - min + 1));
	int j = min + (rand() % (int)(max - min + 1));
	// cout << "j: " << j << endl;
	// cout << "S[j]: " << S[j] << " vs " << S[i] << endl;
	do {
		j = min + (rand() % (int)(max - min + 1));
		// cout << "j: " << j << endl;
		// cout << "S[j]: " << S[j] << " vs " << S[i] << endl;
	}
	while (S[i] == j);
	nS[i] = j;
}
