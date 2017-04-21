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

class Solution
{
protected:
	long long int * A;
public:
	vector<int> S;
	long long int resid;
	Solution(long long int *A, int n);
	void randomize();
	void neighbor(Solution src);
	void update();
	void reassign(Solution src);
};

class P_Solution: public Solution {
	vector<long long int> nA;
};

class S_Solution: public Solution {
};

#endif //CS124PA3_KK_H
