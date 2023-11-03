#include<bits/stdc++.h>
using namespace std;
typedef long int lInt;
using namespace std::chrono;

#include "quicksort.h"

int generateArray(lInt amountData ,lInt maxNumber ,vector<lInt> &array, bool debugMode){
    if (debugMode) cout << "Generando el array" << endl;
    random_device rd;
    mt19937_64 gen(rd());

    if (debugMode) cout << "Construyendo array inicial" << endl;
    vector<lInt> arrayNumbers;
    for (lInt i = 0; i < maxNumber; i++) {
        arrayNumbers.push_back(i + 1); // Asignar valores secuenciales al array
    }

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
    const char* nameFileResult = "experimentacion.txt"; //Nombre de donde se guardaran los 
    bool debugMode = true;
    //----------------------CONFIGURACION----------------//

    ofstream archivo(nameFileResult, std::ios::out);
    if (archivo.is_open()) {
        archivo << "Resultados Quicksort \t Resultados RadixSort" << endl;
        archivo.close();
    }

    for (int i = 0; i < n; i++){
        vector<lInt> myArray;
        if (debugMode) cout << "Prueba n°: " << i + 1 << endl;
        generateArray(100000000,100000001,myArray,debugMode);


        //AQUI DEBEMOS COLOCAR LOS ALGORITMOS//
        auto inicio = high_resolution_clock::now();
        //ALGORITMO 1
        auto fin = high_resolution_clock::now();
        auto tiempo_transcurrido1 = duration_cast<chrono::seconds>(fin - inicio).count();

        inicio = high_resolution_clock::now();
        //ALGORITMO 2
        fin = high_resolution_clock::now();
        auto tiempo_transcurrido2 = duration_cast<chrono::seconds>(fin - inicio).count();

        ofstream archivo(nameFileResult, std::ios::app);
        if (!archivo.is_open()){
            cout << "a ocurrido un error al momento de abrir el archivo" << endl;
            return 1;
        }
        archivo << tiempo_transcurrido1 << "\t" << tiempo_transcurrido2 << endl;
        archivo.close();
    }
    return 0;
}