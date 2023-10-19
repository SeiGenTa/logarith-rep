#include <iostream>
#include<bits/stdc++.h> 
#include <vector>
using namespace std;
namespace fs = std::filesystem;


struct Rectangle{
    int x1, y1, x2, y2;

    void setRectangle(int new_x1,int new_y1,int new_x2,int new_y2){
        x1 = new_x1; y1 = new_y1; x2 = new_x2; y2 = new_y2;
    }
	pair<double,double> getCenter(){
        pair<double,double> center;
        center.first = (x1 + x2 )/ 2.0 ;
        center.second = (y1 + y2 )/ 2.0 ;
        return center;
    };
    //getCenter2: void -> pair<int,int>
    //Obtiene un "valor" central de nuestro rectangulo, el cual sera util para calcular nuestro valor
    //en la curva de hilbert
    pair<int,int> getCenter2(){
        pair<int,int> center;
        center.first = (x1 + x2 )/ 2 ;
        center.second = (y1 + y2 )/ 2 ;
        return center;
    };

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


bool crearSTR(vector<Rectangle> rectangulos, int n , char nombre[20], int max){

    if(n % 2 == 0)
    sort(rectangulos.begin(), rectangulos.end(), [](const Rectangle& izq, const Rectangle& der) {
        return (izq.x1 + izq.x2) / 2 < (der.x1 + der.x2) / 2;
    });

    else
    sort(rectangulos.begin(), rectangulos.end(), [](const Rectangle& izq, const Rectangle& der) {
        return (izq.y1 + izq.y2) / 2 < (der.y1 + der.y2) / 2;
    });
    
    ofstream archivo(nombre);
    if(!archivo.is_open()){
        cout << "Error al abrir el archivo" << endl;
        return false;
    }

    //for(Rectangle i : rectangulos){
    //    cout << i.x1 << "\t" << i.y1 << "\t" << i.x2 << "\t" << i.y2 << endl;
    //}
    
    if(rectangulos.size() <= max){
        archivo << "1\n" ;
        for(int i = 0; i < rectangulos.size();i++){
            archivo << rectangulos[i].x1 << "\t" << rectangulos[i].y1 << "\t" << rectangulos[i].x2 << "\t" << rectangulos[i].y2 << endl;
        } 
        archivo.close();
        return true;
    }
    
    archivo << "0\n" ;
    vector<vector<Rectangle>> partes = dividirEnNPartes(rectangulos, max);
 
    for(int i = 0; i < partes.size(); i++){

        char nombreHijo[7];
        char direccion[20];

        generarNombreAleatorio(nombreHijo, 7);

        strcpy(direccion, "./dSTR/");
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

int generateSTR(vector<Rectangle> *rectangles, bool erase,int maxSize){
    bool debug = true;

    for (const auto& entry : fs::directory_iterator("./dSTR")) {
            if (entry.is_regular_file() && entry.path().extension() == ".txt") {
                fs::remove(entry.path());
        }
    }
    char txt[20] = "./dSTR/raiz.txt";
    vector<Rectangle> rectanglesCopy(*rectangles);

    crearSTR(rectanglesCopy, 0, txt, maxSize);

    return 0;
}