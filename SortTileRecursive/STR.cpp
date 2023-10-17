#include <iostream>
#include<bits/stdc++.h> 
#include <vector>
using namespace std;
namespace fs = std::filesystem;

//Estructura Rectángulo
struct Rectangulo{
    double x1, y1, x2, y2;
};

bool crearSTR( vector<Rectangulo> rectangulos, int n , char nombre[20], int max) {

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
        archivo << "leaf \t 1 \n" ;
        for(int i = 0; i < rectangulos.size();i++){
            archivo << 
            (rectangulos[i].x1 + rectangulos[i].x2)/2 << "\t" <<
            (rectangulos[i].y1 + rectangulos[i].y2)/2 << "\t" <<
            rectangulos[i].x1 << "\t" << rectangulos[i].y1 << "\t" << rectangulos[i].x2 << "\t" << rectangulos[i].y2 << endl;
        } 
    
    }

    archivo.close();
    return true;
}

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
        Rectangulo{1, 2, 3, 4},
        Rectangulo{2, 3, 4, 5},
        Rectangulo{3, 4, 5, 6},
        Rectangulo{4, 5, 6, 7}
    };

    char txt[20] = "./data/raiz.txt";
    int tamanho_max = 2;

    crearSTR(rectangulos, 0, txt, tamanho_max);


}