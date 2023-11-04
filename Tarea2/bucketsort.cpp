#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
typedef unsigned long long int lInt;

void bucketSort(vector<lInt>& arr) {
    if (arr.empty()) return;

    lInt maxValue = *max_element(arr.begin(), arr.end());

    // Crear "buckets" (contenedores)
    int n = arr.size();
    const int numBuckets = 10;  // número de buckets
    vector<vector<lInt>> buckets(numBuckets);

    // Distribuir los elementos en los buckets
    for (int i = 0; i < n; i++) {
        int bucketIndex = (arr[i] * numBuckets) / (maxValue + 1);
        buckets[bucketIndex].push_back(arr[i]);
    }

    // Ordenar los elementos dentro de cada bucket (si los hubiera)
    for (int i = 0; i < numBuckets; i++) {
        if (!buckets[i].empty()) {
            sort(buckets[i].begin(), buckets[i].end());
        }
    }

    // Combinar los elementos de los buckets en el arreglo original
    int index = 0;
    for (int i = 0; i < numBuckets; i++) {
        for (int j = 0; j < buckets[i].size(); j++) {
            arr[index++] = buckets[i][j];
        }
    }
}