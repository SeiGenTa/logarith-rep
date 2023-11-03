#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void BucketSort(unsigned long long int collection[], int n) {
    const int numBuckets = 10;

    vector<vector<unsigned long long int>> buckets(numBuckets);

    unsigned long long int maxValue = *max_element(collection, collection + n);

    for (int i = 0; i < n; ++i) {
        int bucketIndex = (collection[i] * numBuckets) / (maxValue + 1);
        buckets[bucketIndex].push_back(collection[i]);
    }

    for (int x = 0; x < numBuckets; ++x) {
        if (!buckets[x].empty()) {
            sort(buckets[x].begin(), buckets[x].end());
        }
    }

    int index = 0;
    for (int a = 0; a < numBuckets; ++a) {
        for (int b = 0; b < buckets[a].size(); ++b) {
            collection[index++] = buckets[a][b];
        }
    }
}

int main() {
    unsigned long long int numbers[] = {9, 1, 5, 90, 2, 7};
    int n = sizeof(numbers) / sizeof(numbers[0]);

    BucketSort(numbers, n);

    for (int i = 0; i < n; i++) {
        cout << numbers[i] << std::endl;
    }

    return 0;
}