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
	void add(double t, long long int r);
	Return();
	~Return();
	std::vector<double> times;
	std::vector<long long int> resids;
};

#endif //CS124PA3_KK_H
