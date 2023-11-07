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

void QuicksortTread(vector<lInt> &myArray ,int &delay){
    cout << "Ejecutando quicksort" << endl;
    vector<lInt> arrayForQuiksort(myArray);
    auto inicio = high_resolution_clock::now();
    Initquiksort(arrayForQuiksort);
    auto fin = high_resolution_clock::now();
    delay = duration_cast<chrono::seconds>(fin - inicio).count();
    cout << "Se termino Quicksort" << endl;
    arrayForQuiksort.clear();
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
    const char* nameFileResult = "resultados.txt";
    const char* nameFileKOpti = "KOpti.txt";
    const bool debugMode = true;
    const int max2elevated = 64;
    const int sizeArrays = 100000000;
    //--------------CONFIGURACION-------------///

    ofstream archivo(nameFileResult, std::ios::out);
    if (archivo.is_open()) {
        archivo << "Resultado Quicksort;Resultado RadixSort;Valor n en 2^n;n° prueba;valor k radixsort" << endl;
        archivo.close();
    }

    ofstream archivo(nameFileKOpti, std::ios::out);
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
            cout << "valor K: " << results[i].second << "tardo: " << results[i].first << endl;
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

        for (int i = 1; i <= n; ++i) {
            unique_lock<mutex> lock(mtx);
            cond.wait(lock, [&]() { return hilosCreados < hilosPermitidos; });

            hilosCreados++;
            int t = i;

            lock.unlock();
            thread hilo([&sizeArrays, &maxNum, &debugMode, &valueK, &nameFileResult, t, &kOpti, &j]() {
                vector<lInt> myArray;
                generateArray(sizeArrays,maxNum,myArray,debugMode);
                if (debugMode) cout << "Prueba n°: " << t << endl
                << "tamaño de lista: " << myArray.size() << endl
                << "valor de K: "<< kOpti << endl;
                int tiempo_transcurrido1;
                thread miHilo(QuicksortTread, ref(myArray), ref(tiempo_transcurrido1));
                vector<pair<int,int>> results;
                thread hilo(RadixTread, ref(myArray), ref(results), kOpti, false);
                miHilo.join();
                hilo.join();

                //lock_guard<mutex> lockResults(mtxResults);
                ofstream archivo(nameFileResult, ios::app);
                if (!archivo.is_open()) {
                    cout << "Ha ocurrido un error al momento de abrir el archivo" << endl;
                }
                else {
                    archivo << tiempo_transcurrido1 << ";" << results[0].first << ";" << j << ";" << t << ";" << results[0].second << endl;
                    archivo.close();
                }
                hilosCreados--;
                cout << "hilos creados: " << hilosCreados << endl;
                cond.notify_one();
                return 0;
            });
            hilos.push_back(move(hilo));
        }
        for (thread& hilo : hilos) {
            hilo.join();
        }
    }

    return 0;
}

