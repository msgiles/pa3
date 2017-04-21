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

    MaxHeap(long long int * A, int n) {
        array.assign(A, A + n);
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

// Solution Solution::Solution(long long int*A, int n){
//     A = A;
//     P.resize(n);
//     randomize();
//     update();
// };

long long int karmarkar_karp(long long int * A, int n) {
    MaxHeap heap = MaxHeap(A, n);
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