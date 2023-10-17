#include <iostream>
#include<bits/stdc++.h> 
#include <vector>
using namespace std;
namespace fs = std::filesystem;

//Estructura Rectángulo
struct Rectangulo{
    double x1, y1, x2, y2;
};

const char caracteresPermitidos[] = "abcdefghijklmnopqrstuvwxyz";
const int numCaracteres = sizeof(caracteresPermitidos) - 1;
void generarNombreAleatorio(char nombre[], int longitud) {
    random_device rd;
    mt19937 generator(rd());
    uniform_int_distribution<int> distribucion(0, numCaracteres - 1);

    for (int i = 0; i < longitud - 1; i++) {
        int indice = distribucion(generator);
        nombre[i] = caracteresPermitidos[indice];
    }

    // Agregar un carácter nulo al final para formar una cadena C válida.
    nombre[longitud - 1] = '\0';
}

template <typename T>
std::vector<std::vector<T>> dividirEnNPartes(const std::vector<T>& lista, size_t n) {
    std::vector<std::vector<T>> resultado(n);
    size_t elementosPorParte = lista.size() / n;
    size_t elementosExtras = lista.size() % n;

    size_t indice = 0;
    for (size_t i = 0; i < n; i++) {
        size_t longitudParte = elementosPorParte + (i < elementosExtras ? 1 : 0);
        resultado[i].insert(resultado[i].end(), lista.begin() + indice, lista.begin() + indice + longitudParte);
        indice += longitudParte;
    }

    return resultado;
}


bool crearSTR( vector<Rectangulo> rectangulos, int n , char nombre[20], int max){
    if(n % 2 == 0)
    sort(rectangulos.begin(), rectangulos.end(), [](const Rectangulo& izq, const Rectangulo& der) {
        return (izq.x1 + izq.x2) / 2 < (der.x1 + der.x2) / 2;
    });

    else
    sort(rectangulos.begin(), rectangulos.end(), [](const Rectangulo& izq, const Rectangulo& der) {
        return (izq.y1 + izq.y2) / 2 < (der.y1 + der.y2) / 2;
    });

    ofstream archivo(nombre);
    if(!archivo.is_open()){
        cout << "Error al abrir el archivo" << endl;
        return false;
    }

    if(rectangulos.size() <= max){
        archivo << "1\n" ;
        for(int i = 0; i < rectangulos.size();i++){
            archivo << rectangulos[i].x1 << "\t" << rectangulos[i].y1 << "\t" << rectangulos[i].x2 << "\t" << rectangulos[i].y2 << endl;
        } 
        archivo.close();
        return true;
    }
    archivo << "0\n" ;
    vector<vector<Rectangulo>> partes = dividirEnNPartes(rectangulos, max);

    for(int i = 0; i < partes.size(); i++){

        char nombreHijo[7];
        char direccion[20];

        generarNombreAleatorio(nombreHijo, 7);

        strcpy(direccion, "./data/");
        strcat(direccion, nombreHijo);
        strcat(direccion, ".txt");

        int  x1 ,y1 ,x2, y2;
        for (int j = 0; j < partes[i].size(); j++){
            if (j == 0){
                x1 = partes[i][j].x1;
                y1 = partes[i][j].y1;
                x2 = partes[i][j].x2;
                y2 = partes[i][j].y2;
            }
            else{
                if (x1 > partes[i][j].x1){
                    x1 = partes[i][j].x1;
                }if (y1 > partes[i][j].y1){
                    y1 = partes[i][j].y1;
                }if (x2 < partes[i][j].x2){
                    x2 = partes[i][j].x2;
                }if (y2 < partes[i][j].y2){
                    y2 = partes[i][j].y2;
                }
            }
        }

        archivo << x1 << "\t" << y1 << "\t" << x2 << "\t" << y2 << "\t" << direccion << endl;

        crearSTR(partes[i], n+1, direccion, max);
        
    }
    archivo.close();
    return true;
};

int main(){

    bool debug = true;

    for (const auto& entry : fs::directory_iterator("./data")) {
            if (entry.is_regular_file() && entry.path().extension() == ".txt") {
                fs::remove(entry.path());
                if(debug)
                std::cout << "Se ha eliminado: " << entry.path() << std::endl;
        }
    }

    vector<Rectangulo> rectangulos = {
        {4, 4, 6, 6},
        {2, 2, 4, 4},
        {1 ,1 ,5 ,5},
        {0, 0, 2, 2},
        {57, 38, 76, 66},
        {80, 10, 92, 41},
        {37, 6, 48, 40},
        {64, 46, 94, 87},
        {26, 33, 74, 89},
        {65, 50, 93, 79},
        {76, 32, 95, 70},
        {88, 3, 97, 50},
        {50, 46, 68, 76},
        {12, 28, 39, 53},
        {68, 52, 100, 94},
        {18, 0, 45, 31},
        {29, 41, 89, 97},
        {33, 50, 53, 74},
        {16, 9, 44, 39},
        {18, 53, 44, 80},
        {9, 15, 26, 43},
        {75, 44, 96, 84},
        {75, 67, 89, 85},
        {34, 14, 44, 28},
        {61, 49, 89, 96},
        {13, 66, 54, 95},
        {38, 15, 52, 42},
        {58, 2, 96, 36},
        {31, 66, 70, 96},
        {38, 10, 56, 27},
        {26, 8, 58, 40},
        {15, 26, 44, 54},
        {10, 0, 50, 44},
        {29, 14, 45, 31},
        {57, 15, 87, 33},
        {28, 28, 40, 46},
        {44, 33, 63, 59},
        {42, 0, 66, 36},
        {24, 13, 59, 47},
        {66, 53, 93, 85},
        {26, 19, 44, 37},
        {4, 3, 34, 23},
        {38, 0, 70, 31},
        {55, 4, 89, 38},
        {32, 13, 66, 42},
        {44, 12, 61, 30},
        {29, 50, 61, 80},
        {49, 28, 95, 65},
        {35, 16, 60, 43},
        {57, 47, 83, 77},
        {2, 0, 39, 37},
    };

    char txt[20] = "./data/raiz.txt";
    int tamanho_max = 8;

    crearSTR(rectangulos, 0, txt, tamanho_max);

    return 0;

}