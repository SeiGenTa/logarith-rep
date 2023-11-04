#include <iostream>
#include <vector>
#include <algorithm>

void bucketSort(std::vector<unsigned long long int>& arr, int k) {
    if (arr.empty()) return;

    unsigned long long int maxValue = *max_element(arr.begin(), arr.end());

    int n = arr.size();
    int numBuckets = 1 << k;  // 2^k buckets
    std::vector<std::vector<unsigned long long int>> buckets(numBuckets);

    for (int i = 0; i < n; i++) {
        unsigned long long int key = (arr[i] >> k) & (numBuckets - 1);
        buckets[key].push_back(arr[i]);
    }

    for (int i = 0; i < numBuckets; i++) {
        if (!buckets[i].empty()) {
            std::sort(buckets[i].begin(), buckets[i].end());
        }
    }
    int index = 0;
    for (int i = 0; i < numBuckets; i++) {
        for (int j = 0; j < buckets[i].size(); j++) {
            arr[index++] = buckets[i][j];
        }
    }
}



int main() {
    vector<unsigned long long int> arr = {9876543210ULL, 1234567890ULL, 5555555555ULL, 9999999999ULL, 1111111111ULL, 7777777777ULL};
    
    cout << "Arreglo original: ";
    for (int i = 0; i < arr.size(); i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    // Aplicar Bucket Sort al arreglo
    bucketSort(arr);

    cout << "Arreglo ordenado: ";
    for (int i = 0; i < arr.size(); i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    return 0;
}