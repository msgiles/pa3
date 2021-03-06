#include <math.h>
#include <time.h> 
#include <string>
#include <random>
#include <chrono>
#include "helpers.h"

using namespace std;
long long seed = chrono::system_clock::now().time_since_epoch().count();
mt19937_64 gen((unsigned)seed);

// Usage: ./loop flag trials iterations
void rand_ints(std::vector<long long int> *A, long long int min, long long int max);

int main(int argc, char* argv[]) {
    uniform_real_distribution<double> rnd_prb(0.0, 1.0);

	int flag = atoi(argv[1]);  // Debugging flag
	int trials = atoi(argv[2]);  // Number of diff random int arrays
	int iterations = atoi(argv[3]);  // For heuristics allowing for multiple runs, number of iters for each trial

	std::vector<long long int> A;
	A.resize(ARR_SIZE);

	long long int S_resid = MAX,
				  nS_resid = MAX,
	 			  nnS_resid = MAX;

	Return kk, S_random, S_hill, S_annealing, P_kk, P_random, P_hill, P_annealing;


	S_Solution s0(&A, &gen);
	S_Solution s1(&A, &gen);
	S_Solution s2(&A, &gen);

	P_Solution p0(&A, &gen);
	P_Solution p1(&A, &gen);
	P_Solution p2(&A, &gen);


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
		s0.randomize();
		p0.randomize();
		for (int i = 0; i < iterations; i++) {

			s1.randomize();
			p1.randomize();

			if (s1.resid < s0.resid){
				s0.reassign(s1);
//				cout << "standard residue: " << s0.resid << endl;
			}

			if (p1.resid < p0.resid){
				p0.reassign(p1);
//				cout << "prepartitioned residue: " << p0.resid << endl;
			}
		}
		S_random.add(((float)clock() - t) / CLOCKS_PER_SEC,s0.resid);
		P_random.add(((float)clock() - t) / CLOCKS_PER_SEC,p0.resid);

		// Hill Climbing
		cout << "Hill" << endl;
		t = clock();

		s0.randomize();
		p0.randomize();
		for (int i = 0; i < iterations; i++) {

			s1.neighbor(s0);
			p1.neighbor(p0);

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

		p0.randomize();
		s0.randomize();
		p2.reassign(p0);
		s2.reassign(s0);
		for (int i = 0; i < iterations; i++){

			p1.neighbor(p0);
			s1.neighbor(s0);

			if (s1.resid < s0.resid){
				s0.reassign(s1);
			}
			else {
				double p = exp((-s1.resid - s0.resid) / T_iter[i]);
				double d = rnd_prb(gen);
				if (d <= p){
					s0.reassign(s1);
				}
			}

			if (p1.resid < p0.resid){
				p0.reassign(p1);
			}
			else {
				double p = exp((-s1.resid - s0.resid) / T_iter[i]);
				double d = rnd_prb(gen);
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

    cout << "Karmarkar-Karp:" << endl;
    cout << "Residuals:" << endl;
    // cout << "kkrs <- ";
    kk.print_resids();
    cout << endl;
    cout << "Times:" << endl;
    // cout << "kkts <- ";
    kk.print_times();

    cout << "Random Solutions - Standard:" << endl;
    cout << "Residuals:" << endl;
    // cout << "Srandomrs <- ";
    S_random.print_resids();
    cout << "Times:" << endl;
    // cout << "Srandomts <- ";
    S_random.print_times();
    cout << "Random Solutions - Prepartitioned:" << endl;
    cout << "Residuals:" << endl;
    // cout << "Prandomrs <- ";
    P_random.print_resids();
    cout << "Times:" << endl;
    // cout << "Prandomrs <- ";
    P_random.print_times();

    cout << "Hill-Climbing - Standard:" << endl;
    cout << "Residuals:" << endl;
    // cout << "Shillrs <- ";
    S_hill.print_resids();
    cout << "Times:" << endl;
    // cout << "Shillts <- ";
    S_hill.print_times();
    cout << "Hill-Climbing - Prepartitioned:" << endl;
    cout << "Residuals:" << endl;
    // cout << "Phillrs <- ";
    P_hill.print_resids();
    cout << "Times:" << endl;
    // cout << "Phillts <- ";
    P_hill.print_times();

    cout << "Simulated Annealing - Standard:" << endl;
    cout << "Residuals:" << endl;
    // cout << "Sannealingrs <- ";
    S_annealing.print_resids();
    cout << "Times:" << endl;
    // cout << "Sannealingts <- ";
    S_annealing.print_times();
    cout << "Simulated Annealing - Prepartitioned:" << endl;
    cout << "Residuals:" << endl;
    // cout << "Pannealingrs <- ";
    P_annealing.print_resids();
    cout << "Times:" << endl;
    // cout << "Pannealingts <- ";
    P_annealing.print_times();

}

void rand_ints(std::vector<long long int> *A, long long int min, long long int max) {
    uniform_int_distribution<long long int> random(min, max);
	for (int i = 0; i < (*A).size(); i++) {
		long long int r = min + (random(gen) % (max - min + 1));
		(*A)[i] = r;
	}
}
