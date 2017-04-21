#include <iomanip>
#include <fstream>
#include "helpers.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Please enter the appropriate number of arguments (1).";
        return 1;
    }

    string filename = argv[1];
    ifstream file;
    file.open(filename);
    long long int A[ARR_SIZE];
    long long int cur_num;

    for (int i = 0; i < ARR_SIZE; i++) {
        file >> cur_num;
        A[i] = cur_num;
    }

    file.close();

    cout << karmarkar_karp(A);

    return 0;
}