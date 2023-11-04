#include<bits/stdc++.h>
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


int main(){
    //----------------------CONFIGURACION----------------//
    int n = 100; // CANTIDAD DE PRUEBAS
    const char* nameFileResult = "resultados.txt"; //Nombre de donde se guardaran los 
    bool debugMode = true;
    int max2elevated = 64;
    int sizeArrays = 100000000;
    //----------------------CONFIGURACION----------------//

    ofstream archivo(nameFileResult, std::ios::out);
    if (archivo.is_open()) {
        archivo << "Resultados Quicksort;Resultados RadixSort;Valor n en 2^n;n° prueba;valor k radixsort" << endl;
        archivo.close();
    }

    for (int j = 1; j < max2elevated + 1; j++){
        lInt maxNum = pow(2,j);
        vector<lInt> arrayNumbers;
        if (debugMode) cout << "Construyendo array inicial" << endl;
        for (lInt i = 0; i < maxNum; i++) 
            arrayNumbers.push_back(i + 1); // Asignar valores secuenciales al array
        
        for (int i = 0; i < n; i++){
            vector<lInt> myArray;

            generateArray(sizeArrays,arrayNumbers,myArray,debugMode);
            if (debugMode) cout << "Prueba n°: " << i + 1 << endl
            << "tamaño de lista: " << myArray.size() << endl;

            //AQUI DEBEMOS COLOCAR LOS ALGORITMOS//
            if (debugMode) cout << "Ejecutando quicksort" << endl;

            auto inicio = high_resolution_clock::now();
            vector<lInt> arrayForQuiksort(myArray);
            Initquiksort(arrayForQuiksort);
            auto fin = high_resolution_clock::now();
            auto tiempo_transcurrido1 = duration_cast<chrono::seconds>(fin - inicio).count();
            

            if (debugMode) cout << "Ejecutando RadixSort" << endl;
            vector<lInt> arrayForRadixSort(myArray);
            inicio = high_resolution_clock::now();
            bucketSort(arrayForRadixSort);
            fin = high_resolution_clock::now();
            auto tiempo_transcurrido2 = duration_cast<chrono::seconds>(fin - inicio).count();

            ofstream archivo(nameFileResult, std::ios::app);
            if (!archivo.is_open()){
                cout << "a ocurrido un error al momento de abrir el archivo" << endl;
                return 1;
            }
            archivo << tiempo_transcurrido1 << ";" << tiempo_transcurrido2 << ";" << j << ";" << i + 1 << endl;
            archivo.close();
        }
    }

    return 0;
}

