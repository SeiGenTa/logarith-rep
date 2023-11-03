#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void bucketSort(vector<unsigned long long int>& arr) {
    if (arr.empty()) return;

    unsigned long long int maxValue = *max_element(arr.begin(), arr.end());

    // Crear "buckets" (contenedores)
    int n = arr.size();
    const int numBuckets = 10;  // número de buckets
    vector<vector<unsigned long long int>> buckets(numBuckets);

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