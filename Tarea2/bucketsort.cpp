#include <iostream>
#include <vector>
#include <algorithm>

void bucketSort(std::vector<unsigned long long int>& arr, int k) {
    if (arr.empty()) return;

    unsigned long long int maxValue = *std::max_element(arr.begin(), arr.end());

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

void radixSort(std::vector<unsigned long long int>& arr, int maxBits) {
    for (int k = 1; k <= maxBits; k++) {
        bucketSort(arr, k);
        std::cout << "Array after sorting with " << k << " bits: ";
        for (unsigned long long int num : arr) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }
}

int main() {
    std::vector<unsigned long long int> arr = {170, 45, 75, 90, 802, 24, 2, 66, 1, 8};
    int maxBits = 8; // Definir el número máximo de bits a considerar

    radixSort(arr, maxBits);

    std::cout << "Final sorted array: ";
    for (unsigned long long int num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}