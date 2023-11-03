#include<bits/stdc++.h>
using namespace std;
typedef unsigned long long int lInt;

void swap(lInt& a, lInt& b) {
    lInt temp = a;
    a = b;
    b = temp;
}

int partition(std::vector<lInt>& arr, int low, int high) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(low, high);
    int randomIndex = distribution(gen);

    swap(arr[randomIndex], arr[low]);

    lInt pivot = arr[low];
    int i = low + 1;
    int j = high;

    while (true) {
        while (i <= high && arr[i] < pivot) {
            i++;
        }
        while (j >= low && arr[j] > pivot) {
            j--;
        }
        if (i < j) {
            swap(arr[i], arr[j]);
        } else {
            break;
        }
    }

    swap(arr[low], arr[j]);
    return j;
}

void quicksort(std::vector<lInt>& arr, int low, int high) {
    if (low < high) {
        int pivotIndex = partition(arr, low, high);
        quicksort(arr, low, pivotIndex - 1);
        quicksort(arr, pivotIndex + 1, high);
    }
}

int Initquiksort(vector<lInt> &array){
    int n = array.size();
    quicksort(array, 0, n - 1);
    return 0;
}