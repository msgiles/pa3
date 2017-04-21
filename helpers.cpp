//
// Created by Ethan on 4/20/17.
//
#include "helpers.h"

using namespace std;

// Class for binary max heap, originally a min heap adapted from CLRS in programming asst 1
class MaxHeap {
public:
    // array of numbers
    vector<long long int> array;
    int heap_size;

    MaxHeap(vector<long long int> A, int n) {
        array = A;
        heap_size = n;
    }

    // Automatic floor division in C++
    int parent(int i) { return (i + 1) / 2 - 1; }
    int left(int i) { return 2 * (i + 1) - 1; }
    int right(int i) { return 2 * (i + 1); }

    // Exchange the places of two numbers on the heap
    void swap(int a, int b) {
        long long int temp = array[b];
        array[b] = array[a];
        array[a] = temp;
    }

    // For debugging purposes
    void display_heap() {
        for (int i = 0; i < heap_size; i++) {
            cout << setw(1) << i << ": (" << array[i]<< ", " <<  ")    ";

            if (((i + 2) & (i + 1)) == 0) {
                cout << endl;

            }

        }
        cout << endl << endl;
    }


    bool check_empty() {
        if (heap_size == 0) {
            return true;
        } else {
            return false;
        }
    }


    // extract max
    long long int extract_max() {
        if (heap_size < 1) {
            return 0;
        }
        long long int max = array[0];
        array[0] = array[heap_size - 1];
        heap_size--;
        max_heapify(0);
        return max;
    };

    // Recursively fix the heap starting at index i after inserting to or
    // removing from it
    void max_heapify(int i) {
        int largest;
        int l = left(i);
        int r = right(i);

        if (l < heap_size && array[l] > array[i]) {
            largest = l;
        } else {
            largest = i;
        }
        if (r < heap_size && array[r] > array[largest]) {
            largest = r;
        }
        if (largest != i) {
            swap(i, largest);
            max_heapify(largest);
        }
    }


    // Place number onto heap
    void insert(long long int p) {
        heap_size++;
        int n = heap_size - 1;
        array[n] = p;
        while (n != 0 && array[n] > array[parent(n)]) {
            swap(n, parent(n));
            n = parent(n);
        }
    };

    void build_heap() {
        for (int i = heap_size / 2 - 1; i >= 0; i--) {
            max_heapify(i);
        }
    }
};

void Return::add(float t, long long int r){
    times.push_back(t);
    resids.push_back(r);
};

void Return::print_times(){
    int len = times.size();
    cout << "{";
    for (int i = 0; i < len; i++){
        cout << times[i];
        if (i == len - 1){
        cout << "}" << endl;
        }
        else {
            cout << ",";
        }
    }
};

void Return::print_resids(){
    int len = resids.size();
    cout << "{";
    for (int i = 0; i < len; i++){
        cout << resids[i];
        if (i == len - 1){
        cout << "}" << endl;
        }
        else {
            cout << ",";
        }
    }
};

Solution::Solution(std::vector<long long int> *arr){
    A = arr;
    S.resize(ARR_SIZE);
};

void Solution::update(){};
void Solution::randomize(){};
void Solution::reassign(Solution src){
    S = src.S;
    resid = src.resid;
};

P_Solution::P_Solution(std::vector<long long int> *arr) : Solution(arr) {
    nA.resize(ARR_SIZE);
    randomize();
    update();
};

void P_Solution::update(){
    std::fill(nA.begin(), nA.end(),0);
    for (int i = 0; i < nA.size(); i++){
        nA[S[i]] += (*A)[i];
    }
    resid = karmarkar_karp(&nA);
};

void P_Solution::randomize(){
    for (int i = 0; i < S.size(); i++){
        int r = (rand() % (int)(S.size()));
        S[i] = r;
    }
    update();
};

void P_Solution::reassign(P_Solution src){
    Solution::reassign(src);
    nA = src.nA;
};

void P_Solution::neighbor(P_Solution src){
    reassign(src);
    int i = (rand() % (int)(S.size()));
    int j;
    do {
        j = (rand() % (int)(S.size()));
    }
    while (S[i] == j);
    src.S[i] = j;
    update();
};

S_Solution::S_Solution(std::vector<long long int> *arr) : Solution(arr) {
    randomize();
    update();
};

void S_Solution::randomize(){
    for (int i = 0; i < S.size(); i++){
        int r = ((float) rand() / RAND_MAX);
        if (r < .5){
            S[i] = -1;
        }
        else {
            S[i] = 1;
        }
    }
    update();
};

void S_Solution::reassign(S_Solution src){
    Solution::reassign(src);
};

void S_Solution::update(){
    resid = 0;
    for (int i = 0; i < S.size(); i++){
        resid += (*A)[i] * S[i];
    }
    resid = abs(resid);
};

void S_Solution::neighbor(S_Solution src){
    reassign(src);
    int r = ((float) rand() / RAND_MAX);
    int i = (rand() % (int)(S.size()));
    if (r < .5){
        S[i] = S[i]*-1;
    }
    else {
        int j;
        do {
            j = (rand() % (int)(S.size()));
        }
        while (i == j);
        S[j] = S[j]*-1;
    }
    update();
};

long long int karmarkar_karp(vector<long long int> *A) {
    MaxHeap heap = MaxHeap(*A, A->size());
    heap.build_heap();
    long long int e1, e2;

    while (!heap.check_empty()) {
//        heap.display_heap();
        e1 = heap.extract_max();
        if (heap.check_empty()) {
            return e1;
        }
        else {
            e2 = heap.extract_max();
            heap.insert(e1-e2);
        }
    }

    return 1;
};