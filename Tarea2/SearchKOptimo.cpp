#include<bits/stdc++.h>
#include <thread>
#include <mutex>
using namespace std;
typedef unsigned long long int lInt;
using namespace std::chrono;

#include "quicksort.h"
#include "bucketsort.h"

int generateArray(lInt amountData ,lInt maxNum ,vector<lInt> &array, bool debugMode){
    if (debugMode) cout << "Generando el array" << endl;
    random_device rd;
    mt19937_64 gen(rd());

    if (debugMode) cout << "Creando array final" << endl;
    for (lInt i = 0; i < amountData; i++){
        uniform_int_distribution<lInt> distribution(0, maxNum);
        lInt indiceAleatorio = distribution(gen);

        array.push_back(indiceAleatorio);
    }

    return 0; 
}

std::mutex mtx;
std::mutex mtxResults;
condition_variable cond;
int dispMem = 0;
int hilosCreados = 0;

void RadixTread(vector<lInt> &myArray ,vector<pair<int,int>> &results, int valueK, bool reduce){
    cout << "Ejecutando RadixSort" << endl;
    vector<lInt> arrayForRadixSort(myArray);
    auto inicio = high_resolution_clock::now();
    radixSort(arrayForRadixSort,valueK);
    auto fin = high_resolution_clock::now();
    int delay = duration_cast<chrono::seconds>(fin - inicio).count();
    cout << "Se termino RadixSort" << endl;
    pair<int,int> result(delay, valueK);
    results.push_back(result);
    arrayForRadixSort.clear();
    

    if (reduce) hilosCreados--;
}


void crearHilos(vector<lInt> &myArray, int valueK, vector<pair<int,int>> &results) {
    vector<thread> hilos;

    for (int i = 1; i <= valueK; ++i) {
        lock_guard<mutex> lock(mtx);
        if (hilosCreados < 2) {
            cout << "corriendo radix con k = " << i << endl,
            hilosCreados++; // Incrementa el contador de hilos
            thread hilo(RadixTread, ref(myArray), ref(results), i,true);
            hilos.push_back(move(hilo));
        } else {
            this_thread::sleep_for(chrono::seconds(1)); // Espera si el límite se alcanzó
            i--;
        }
    }

    for (thread& hilo : hilos) {
        hilo.join();
    }
}


int main() {
    //--------------CONFIGURACION-------------///
    const int hilosPermitidos = 2; //Cantidad de threads que permite al momento 
    const int n = 30;
    const char* nameFileResult = "KOpti.txt";
    const bool debugMode = true;
    const int max2elevated = 64;
    const int sizeArrays = 100000000;
    //--------------CONFIGURACION-------------///

    ofstream archivo(nameFileResult, std::ios::out);
    if (archivo.is_open()) {
        archivo << "Valor K;valor del n del universo;tiempo tardado" << endl;
        archivo.close();
    }

    for (int j = 1; j < max2elevated + 1; j++) {
        lInt maxNum = pow(2, j);
        int valueK = 26;
        int kOpti;
        int time;


        cout << "bucando K optimo" << endl;
        vector<lInt> myArray;
        generateArray(sizeArrays, maxNum, myArray, debugMode);
        vector<pair<int, int>> results;
        crearHilos(myArray, valueK, results);
        for (int i = 0; i < results.size(); i++) {
            ofstream archivo(nameFileResult, std::ios::app);
            if (!archivo.is_open()) {
                cout << "Hemos tenido un problema al arbrir el archivo" << endl;
                
            }
            archivo  << results[i].second << ";" << j << ";" << results[i].first << endl;

            archivo.close();
            if (i == 0) {
                kOpti = results[i].second;
                time = results[i].first;
            }
            else if (results[i].first < time) {
                kOpti = results[i].second;
                time = results[i].first;
            }
        
        }

        cout << "K optimo es: " << kOpti << endl;

        vector<thread> hilos;
    }

    return 0;
}

