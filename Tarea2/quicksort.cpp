#include<bits/stdc++.h>
using namespace std;
typedef unsigned long long int lInt;

void swap(lInt& a, lInt& b) {
    lInt temp = a;
    a = b;
    b = temp;
}

// Función para particionar el vector
void partition(std::vector<lInt>& arr, int low, int high, int& left, int& right) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(low, high);
    int pivotIndex = distribution(gen);
    lInt pivot = arr[pivotIndex];

    left = low;
    right = high;

    int i = low;
    while (i <= right) {
        if (arr[i] < pivot) {
            swap(arr[i], arr[left]);
            i++;
            left++;
        } else if (arr[i] > pivot) {
            swap(arr[i], arr[right]);
            right--;
        } else {
            i++;
        }
    }
}

// Función de Quicksort
void quicksort(std::vector<lInt>& arr, int low, int high) {
    if (low < high) {
        int left, right;
        partition(arr, low, high, left, right);

        quicksort(arr, low, left - 1);
        quicksort(arr, right + 1, high);
    }
}

int Initquiksort(vector<lInt> &array){
    lInt n = array.size();
    quicksort(array, 0, n - 1);
    return 0;
}