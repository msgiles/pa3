#include <math.h>
#include <time.h> 
#include <string>
#include <random>
#include <chrono>
#include "helpers.h"

using namespace std;

// Usage: ./loop flag trials iterations
void deprep(long long int*A, int*P, long long int*O, int n);

void rand_ints(std::vector<long long int> *A, long long int min, long long int max);

void rand_soln(int*S, int len, int min, int max);

void rand_neighbor(int *S, int *nS, int min, int max);

int main(int argc, char* argv[]) {

	srand(time(NULL));
	int flag = atoi(argv[1]);  // Debugging flag
	int trials = atoi(argv[2]);  // Number of diff random int arrays
	int iterations = atoi(argv[3]);  // For heuristics allowing for multiple runs, number of iters for each trial

	// Allocate prepartitioning arrays, int arrays, and temperature array
	// int *S = (int*) malloc(ARR_SIZE * sizeof(int)),
	// 	*nS = (int*) malloc(ARR_SIZE * sizeof(int)),
	// 	*nnS = (int*) malloc(ARR_SIZE * sizeof(int));
	// long long int *A = (long long int*) malloc(ARR_SIZE * sizeof(long long int)),
	// 	*SO = (long long int*) malloc(ARR_SIZE * sizeof(long long int)),
	// 	*nSO = (long long int*) malloc(ARR_SIZE * sizeof(long long int)),
	// 	*nnSO = (long long int*) malloc(ARR_SIZE * sizeof(long long int));
	// double *T_iter = (double *) malloc(iterations * sizeof(double));

	// long long int *A = (long long int*) malloc(ARR_SIZE * sizeof(long long int));

	std::vector<long long int> A;
	A.resize(ARR_SIZE);

	long long int S_resid = MAX,
				  nS_resid = MAX,
	 			  nnS_resid = MAX;

	Return kk, S_random, S_hill, S_annealing, P_kk, P_random, P_hill, P_annealing;


	S_Solution s0(&A);
	S_Solution s1(&A);
	S_Solution s2(&A);

	P_Solution p0(&A);
	P_Solution p1(&A);
	P_Solution p2(&A);


	clock_t t;

	// Preprocess T_iter array
	std::vector<double>T_iter;
	T_iter.resize(iterations);
	for (int i = 0; i<iterations; i++) {
		T_iter[i] = 10000000000 * pow((double) .8, (double) i / 300);
	}

	for (int trial = 0; trial < trials; trial++){
		cout << "Trial " << trial << endl;
		rand_ints(&A, 1, MAX);
		S_resid = MAX;
		nS_resid = MAX;
		nnS_resid = MAX;

		// Karmarkar-Karp
        cout << "Karmarkar-Karp" << endl;
		t = clock();
		S_resid = karmarkar_karp(&A);
		kk.add(((float)clock() - t) / CLOCKS_PER_SEC, S_resid);

		// Random Solution
        cout << "Random" << endl;
		t = clock();
		// rand_soln(S, ARR_SIZE, 0, ARR_SIZE-1);
		// deprep(A,S,SO,ARR_SIZE);
		// S_resid = karmarkar_karp(SO, ARR_SIZE);
		s0.randomize();
		p0.randomize();
		for (int i = 0; i < iterations; i++) {
			// rand_soln(nS, ARR_SIZE, 0, ARR_SIZE-1);

			// deprep(A,nS,nSO,ARR_SIZE);

			s1.randomize();
			p1.randomize();

			// nS_resid = karmarkar_karp(nSO, ARR_SIZE);

			// if (nS_resid < S_resid) {
			// 	memcpy(S, nS, ARR_SIZE * sizeof(int));
			// 	S_resid = nS_resid;
			// }

			if (s1.resid < s0.resid){
				s0.reassign(s1);
			}

			if (p1.resid < p0.resid){
				p0.reassign(p1);
			}
		}
		// random.add(((float)clock() - t) / CLOCKS_PER_SEC,S_resid);
		S_random.add(((float)clock() - t) / CLOCKS_PER_SEC,s0.resid);
		P_random.add(((float)clock() - t) / CLOCKS_PER_SEC,p0.resid);

		// Hill Climbing
		cout << "Hill" << endl;
		t = clock();
		// rand_soln(S, ARR_SIZE, 0, ARR_SIZE-1);
		// deprep(A,S,SO,ARR_SIZE);
		// S_resid = karmarkar_karp(SO, ARR_SIZE);
		s0.randomize();
		p0.randomize();
		for (int i = 0; i < iterations; i++) {
			// std::string prev = std::to_string(i-1);
			// cout << string(prev.length(), '\b');
			// cout << i << endl;
			// rand_neighbor(nS, S, 0, ARR_SIZE-1);
			// cout << "Generated Neighbor!" << endl;
			// deprep(A,nS,nSO,ARR_SIZE);
			// cout << "De-Prepartitioned" << endl;
			s1.neighbor(s0);
			p1.neighbor(p0);
			// nS_resid = karmarkar_karp(nSO, ARR_SIZE);
			// cout << "Residual: " << nS_resid << endl;

			// if (nS_resid < S_resid) {
			// 	memcpy(S, nS, ARR_SIZE * sizeof(int));
			// 	S_resid = nS_resid;
			// }
			if (s1.resid < s0.resid){
				s0.reassign(s1);
			}
			if (p1.resid < p0.resid){
				p0.reassign(p1);
			}
		}
		S_hill.add(((float)clock() - t) / CLOCKS_PER_SEC,s0.resid);
		P_hill.add(((float)clock() - t) / CLOCKS_PER_SEC,p0.resid);

		// Simulated Annealing
		cout << "Annealing" << endl;
		t = clock();
		// rand_soln(S, ARR_SIZE, 0, ARR_SIZE-1);
		// memcpy(nnS, S, sizeof(int) * ARR_SIZE);
		// deprep(A, S, SO, ARR_SIZE);
		// S_resid = karmarkar_karp(SO,ARR_SIZE);
		// nnS_resid = S_resid;
		p0.randomize();
		s0.randomize();
		p2.reassign(p0);
		s2.reassign(s0);
		for (int i = 0; i < iterations; i++){
			// rand_neighbor(nS, S, 0, ARR_SIZE-1);
			p1.neighbor(p0);
			s1.neighbor(s0);
			// deprep(A, nS, nSO, ARR_SIZE);
			// deprep(A, nnS, nnSO, ARR_SIZE);

			// nS_resid = karmarkar_karp(nSO,ARR_SIZE);
			// nnS_resid = karmarkar_karp(nnSO,ARR_SIZE);

			// if(nS_resid < S_resid){
			// 	memcpy(S, nS, sizeof(int) * ARR_SIZE);
			// 	S_resid = nS_resid;
			// }
			// else {
			// 	double p = exp((-nS_resid - S_resid) / T_iter[i]);
			// 	double d = (double) rand() / (double) RAND_MAX;
			// 	if (d <= p){
			// 		memcpy(S, nnS, sizeof(int) * ARR_SIZE);
			// 		S_resid = nnS_resid;
			// 	}

			// }

			// if (S_resid < nnS_resid){
			// 	memcpy(nnS, S, sizeof(int) * ARR_SIZE);
			// }

			if (s1.resid < s0.resid){
				s0.reassign(s1);
			}
			else {
				double p = exp((-s1.resid - s0.resid) / T_iter[i]);
				double d = (double) rand() / (double) RAND_MAX;
				if (d <= p){
					s0.reassign(s1);
				}
			}

			if (p1.resid < p0.resid){
				p0.reassign(p1);
			}
			else {
				double p = exp((-s1.resid - s0.resid) / T_iter[i]);
				double d = (double) rand() / (double) RAND_MAX;
				if (d <= p){
					p0.reassign(p1);
				}
			}

			if (s0.resid < s2.resid){
				s2.reassign(s0);
			}

			if (p0.resid < p2.resid){
				p2.reassign(p0);
			}
		}
		S_annealing.add(((float)clock() - t) / CLOCKS_PER_SEC,s2.resid);
		P_annealing.add(((float)clock() - t) / CLOCKS_PER_SEC,p2.resid);
	}
// free(A);free(S);free(nS);free(nnS);free(SO);free(nSO);free(nnSO);free(T_iter);

cout << "Karmarkar-Karp:" << endl;
cout << "Residuals:" << endl;
kk.print_resids();
cout << "Times:" << endl;
kk.print_times();

cout << "Random Solutions - Standard:" << endl;
cout << "Residuals:" << endl;
S_random.print_resids();
cout << "Times:" << endl;
S_random.print_times();
cout << "Random Solutions - Prepartitioned:" << endl;
cout << "Residuals:" << endl;
P_random.print_resids();
cout << "Times:" << endl;
P_random.print_times();

cout << "Hill-Climbing - Standard:" << endl;
cout << "Residuals:" << endl;
S_hill.print_resids();
cout << "Times:" << endl;
S_hill.print_times();
cout << "Hill-Climbing - Prepartitioned:" << endl;
cout << "Residuals:" << endl;
P_hill.print_resids();
cout << "Times:" << endl;
P_hill.print_times();

cout << "Simulated Annealing - Standard:" << endl;
cout << "Residuals:" << endl;
S_annealing.print_resids();
cout << "Times:" << endl;
S_annealing.print_times();
cout << "Simulated Annealing - Prepartitioned:" << endl;
cout << "Residuals:" << endl;
P_annealing.print_resids();
cout << "Times:" << endl;
P_annealing.print_times();

}

void deprep(long long int*A, int*P, long long int*O, int len) {
	for (int i = 0; i < len; i++) {
		O[i] = 0;
	}
	
	for (int i = 0; i < len; i++) {
		O[P[i]] += A[i];
	}
}

void rand_ints(std::vector<long long int> *A, long long int min, long long int max) {
	for (int i = 0; i < (*A).size(); i++) {
		long long int r = min + (rand() % (max - min + 1));
		(*A)[i] = r;
	}
}

void rand_soln(int*S, int len, int min, int max) {
	for (int i = 0; i < len; i++) {
		int r = min + (rand() % (max - min + 1));
		S[i] = r;
	}
}

void rand_neighbor(int *S, int *nS, int min, int max){
	memcpy(nS, S, sizeof(int) * ARR_SIZE);
	int i = min + (rand() % (max - min + 1));
	int j;
	// cout << "j: " << j << endl;
	// cout << "S[j]: " << S[j] << " vs " << S[i] << endl;
	do {
		j = min + (rand() % (max - min + 1));
		// cout << "j: " << j << endl;
		// cout << "S[j]: " << S[j] << " vs " << S[i] << endl;
	}
	while (S[i] == j);
	nS[i] = j;
}
