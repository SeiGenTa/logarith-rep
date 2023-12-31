#include<bits/stdc++.h>
#include <thread>
#include <mutex>
using namespace std;
typedef unsigned long long int lInt;
using namespace std::chrono;

#include "quicksort.h"
#include "bucketsort.h"

int generateArray(lInt amountData ,vector<lInt> &arrayNumbers ,vector<lInt> &array, bool debugMode){
    if (debugMode) cout << "Generando el array" << endl;
    random_device rd;
    mt19937_64 gen(rd());

    if (debugMode) cout << "Creando array final" << endl;
    for (lInt i = 0; i < amountData; i++){
        uniform_int_distribution<lInt> distribution(0, arrayNumbers.size() - 1);
        lInt indiceAleatorio = distribution(gen);

        array.push_back(arrayNumbers[indiceAleatorio]);
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
}

std::mutex mtx;
std::mutex mtxResults;
int dispMem = 0;
int hilosCreados = 0;

void RadixTread(vector<lInt> &myArray ,vector<pair<int,int>> &results, int valueK){
    cout << "Ejecutando RadixSort" << endl;
    vector<lInt> arrayForRadixSort(myArray);
    auto inicio = high_resolution_clock::now();
    radixSort(arrayForRadixSort,valueK);
    auto fin = high_resolution_clock::now();
    int delay = duration_cast<chrono::seconds>(fin - inicio).count();
    cout << "Se termino RadixSort" << endl;
    pair<int,int> result(delay, valueK);
    results.push_back(result);
    

    hilosCreados--;
}


void crearHilos(vector<lInt> &myArray, int valueK, vector<pair<int,int>> &results) {
    vector<thread> hilos;

    for (int i = 1; i <= valueK; ++i) {
        lock_guard<mutex> lock(mtx);
        if (hilosCreados < 4) {
            cout << "corriendo radix con k = " << i << endl,
            hilosCreados++; // Incrementa el contador de hilos
            thread hilo(RadixTread, ref(myArray), ref(results), i);
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

void analizisData(lInt kOpti ,int j,int sizeArrays,vector<lInt> &arrayNumbers,bool debugMode,int valueK,char* &nameFileResult){
    for (int i = 0; i < numIteracionesPorHilo + 1; i++){
        vector<lInt> myArray
        generateArray(sizeArrays,arrayNumbers,myArray,debugMode);
        if (debugMode) cout << "Prueba n°: " << i + 67  << endl
        << "tamaño de lista: " << myArray.size() << endl
        << "valor de K: "<< valueK << endl;
        //AQUI DEBEMOS COLOCAR LOS ALGORITMOS/
        int tiempo_transcurrido1;
        thread miHilo(QuicksortTread, ref(myArray), ref(tiempo_transcurrido1))
        vector<pair<int,int>> results;
        thread hilo(RadixTread, ref(myArray), ref(results), kOpti)
        miHilo.join();
        hilo.join()
        lock_guard<mutex> lock(mtxResults);
        ofstream archivo(nameFileResult, std::ios::app);
        if (!archivo.is_open()){
            cout << "a ocurrido un error al momento de abrir el archivo" << endl;
            return 1;
        }
        archivo << tiempo_transcurrido1 << ";" << results[0].first << ";" << j << ";" << i + 67 << ";" << results[0].second << endl;
        archivo.close();
    }
}


int main() {
    int n = 100;
    const char* nameFileResult = "resultados.txt";
    bool debugMode = true;
    int max2elevated = 64;
    int sizeArrays = 100000000;

    ofstream archivo(nameFileResult, std::ios::out);
    if (archivo.is_open()) {
        archivo << "Resultado Quicksort;Resultado RadixSort;Valor n en 2^n;n° prueba;valor k radixsort" << endl;
        archivo.close();
    }

    for (int j = 1; j < max2elevated + 1; j++) {
        lInt maxNum = pow(2, j);
        vector<lInt> arrayNumbers;
        int valueK = log2(maxNum);
        if (debugMode) cout << "Construyendo array inicial" << endl;
        for (lInt i = 0; i < maxNum; i++)
            arrayNumbers.push_back(i + 1);

        int kOpti;
        int time;
        if (j != 1 && j != 10) {
            vector<lInt> myArray;
            generateArray(sizeArrays, arrayNumbers, myArray, debugMode);
            vector<pair<int, int>> results;
            crearHilos(myArray, valueK, results);

            for (int i = 0; i < results.size(); i++) {
                if (i == 0) {
                    kOpti = results[i].second;
                    time = results[i].first;
                }
                else if (results[i].second < time) {
                    kOpti = results[i].second;
                    time = results[i].first;
                }
            }
        } else {
            kOpti = 1;
            if (j == 10){
                kOpti = 10;
            }
        }

        cout << "K optimo es: " << kOpti << endl;

        // Dividir el bucle principal en dos hilos
        int numIteracionesPorHilo = n / 3;
        int tiempo_transcurrido1;
        vector<pair<int, int>> results1;
        vector<lInt> myArray1;
        thread hilo2([kOpti, numIteracionesPorHilo,j,sizeArrays,&arrayNumbers,debugMode,valueK,&nameFileResult](){
            for (int i = 0; i < numIteracionesPorHilo + 1; i++){
                vector<lInt> myArray;

                generateArray(sizeArrays,arrayNumbers,myArray,debugMode);
                if (debugMode) cout << "Prueba n°: " << i + 67  << endl
                << "tamaño de lista: " << myArray.size() << endl
                << "valor de K: "<< valueK << endl;
                //AQUI DEBEMOS COLOCAR LOS ALGORITMOS//

                int tiempo_transcurrido1;
                thread miHilo(QuicksortTread, ref(myArray), ref(tiempo_transcurrido1));

                vector<pair<int,int>> results;
                thread hilo(RadixTread, ref(myArray), ref(results), kOpti);

                miHilo.join();
                hilo.join();

                lock_guard<mutex> lock(mtxResults);
                ofstream archivo(nameFileResult, std::ios::app);
                if (!archivo.is_open()){
                    cout << "a ocurrido un error al momento de abrir el archivo" << endl;
                    return 1;
                }
                archivo << tiempo_transcurrido1 << ";" << results[0].first << ";" << j << ";" << i + 67 << ";" << results[0].second << endl;
                archivo.close();
            }
            return 0;
        });

        thread hilo3([kOpti, numIteracionesPorHilo,j,sizeArrays,&arrayNumbers,debugMode,valueK,&nameFileResult](){
            for (int i = 0; i < numIteracionesPorHilo; i++){
                vector<lInt> myArray;

                generateArray(sizeArrays,arrayNumbers,myArray,debugMode);
                if (debugMode) cout << "Prueba n°: " << i + 34 << endl
                << "tamaño de lista: " << myArray.size() << endl
                << "valor de K: "<< valueK << endl;
                //AQUI DEBEMOS COLOCAR LOS ALGORITMOS//

                int tiempo_transcurrido1;
                thread miHilo(QuicksortTread, ref(myArray), ref(tiempo_transcurrido1));

                vector<pair<int,int>> results;
                thread hilo(RadixTread, ref(myArray), ref(results), kOpti);

                miHilo.join();
                hilo.join();

                lock_guard<mutex> lock(mtxResults);
                ofstream archivo(nameFileResult, std::ios::app);
                if (!archivo.is_open()){
                    cout << "a ocurrido un error al momento de abrir el archivo" << endl;
                    return 1;
                }
                archivo << tiempo_transcurrido1 << ";" << results[0].first << ";" << j << ";" << i + 34 << ";" << results[0].second << endl;
                archivo.close();
            }
            return 0;
        });

        thread hilo1([&myArray1, kOpti, numIteracionesPorHilo,j,sizeArrays,&arrayNumbers,debugMode,valueK,&nameFileResult](){
            for (int i = 0; i < numIteracionesPorHilo; i++){
                vector<lInt> myArray;

                generateArray(sizeArrays,arrayNumbers,myArray,debugMode);
                if (debugMode) cout << "Prueba n°: " << i + 1 << endl
                << "tamaño de lista: " << myArray.size() << endl
                << "valor de K: "<< valueK << endl;
                //AQUI DEBEMOS COLOCAR LOS ALGORITMOS//

                int tiempo_transcurrido1;
                thread miHilo(QuicksortTread, ref(myArray), ref(tiempo_transcurrido1));

                vector<pair<int,int>> results;
                thread hilo(RadixTread, ref(myArray), ref(results), kOpti);

                miHilo.join();
                hilo.join();

                lock_guard<mutex> lock(mtxResults);
                ofstream archivo(nameFileResult, std::ios::app);
                if (!archivo.is_open()){
                    cout << "a ocurrido un error al momento de abrir el archivo" << endl;
                    return 1;
                }
                archivo << tiempo_transcurrido1 << ";" << results[0].first << ";" << j << ";" << i + 1 << ";" << results[0].second << endl;
                archivo.close();
            }
            return 0;
        });

        hilo1.join();
        hilo2.join();
        hilo3.join();
    }

    return 0;
}

