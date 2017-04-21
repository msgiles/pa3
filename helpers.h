//
// Created by Ethan on 4/19/17.
//

#ifndef CS124PA3_HELPERS_H
#define CS124PA3_HELPERS_H

#define MAX 1000000000000
#define ARR_SIZE 100

#include <iostream>
#include <iomanip>
#include <vector>
#include <random>

long long int karmarkar_karp(std::vector<long long int> *A);

class Return
{
public:
	std::vector<float> times;
	std::vector<long long int> resids;
	void add(float t, long long int r);
	void print_times();
	void print_resids();
};

class Solution
{
public:
	std::vector<long long int> *A;
    std::default_random_engine gen;
	std::vector<int> S;
	long long int resid;
	Solution(std::vector<long long int> *arr, std::default_random_engine *rnd_gen);
	void update();
	void randomize();
	void reassign(Solution src);
};

class P_Solution: public Solution {
public:
	P_Solution(std::vector<long long int> *arr, std::default_random_engine *rnd_gen);
	std::vector<long long int> nA;
	void update();
	void reassign(P_Solution src);
	void randomize();
	void neighbor(P_Solution src);
};

class S_Solution: public Solution {
public:
	S_Solution(std::vector<long long int> *arr, std::default_random_engine *rnd_gen);
	void update();
	void reassign(S_Solution src);
	void randomize();
	void neighbor(S_Solution src);
};

#endif //CS124PA3_KK_H
