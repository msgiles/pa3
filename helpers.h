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

long long int karmarkar_karp(long long int * A, int n);

class Return
{
public:
	std::vector<float> times;
	std::vector<long long int> resids;
	void add(float t, long long int r);
	void print_times();
	void print_resids();
};

// class Solution
// {
// private:
// 	long long int * A;
// public:
// 	std::std::vector<int> P;
// 	long long int resid;
// 	Solution(long long int *A, int n);
// 	void randomize();
// 	void neighbor();
// 	void update();
// };

#endif //CS124PA3_KK_H
