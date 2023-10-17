#include<bits/stdc++.h> 
namespace fs = std::filesystem;
using namespace std;

void rot(int64_t  n, int64_t  *x, int64_t  *y, int64_t  rx, int64_t  ry) {
    int64_t  t;
    if (ry == 0) {
        if (rx == 1) {
            *x = n-1 - *x;
            *y = n-1 - *y;
        }
        t  = *x;
        *x = *y;
        *y = t;
    }
};

void d2xy(int64_t  d, int64_t  *x, int64_t  *y) {
    int64_t  rx, ry, s, t=d;
    *x = *y = 0;
    for (s = 1; s < 524288; s *= 2) {
        rx = 1 & (t/2);
        ry = 1 & (t ^ rx);
        rot(s, x, y, rx, ry);
        *x += s * rx;
        *y += s * ry;
        t /= 4;
    }
}

int64_t  xy2d (int64_t  x, int64_t  y) {
    int64_t  rx, ry, s, d=0;
    for (s = 524288/2; s > 0; s /= 2) {
        rx = (x & s) > 0;
        ry = (y & s) > 0;
        d += s * s * ((3 * rx) ^ ry);
        rot(s, &x, &y, rx, ry);
    };
    return d;
};

struct Rectangle{
    int x1, y1, x2, y2;

    void setRectangle(int new_x1,int new_y1,int new_x2,int new_y2){
        x1 = new_x1; y1 = new_y1; x2 = new_x2; y2 = new_y2;
    }
    //getCenter: void -> pair<int,int>
    //Obtiene un "valor" central de nuestro rectangulo, el cual sera util para calcular nuestro valor
    //en la curva de hilbert
    pair<int,int> getCenter(){
        pair<int,int> center;
        center.first = (x1 + x2 )/ 2 ;
        center.second = (y1 + y2 )/ 2 ;
        return center;
    };

    int64_t getValueCurveHilbert(){
        pair<int,int> center = getCenter();
        return xy2d(center.first, center.second);
    }
};
// Función de partición para QuickSort
int particion(vector<Rectangle>& arr, int low, int high) {
    int pivot = arr[high].getValueCurveHilbert();
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j].getValueCurveHilbert() < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return (i + 1);
}
// Función QuickSort personalizada
void quickSort(vector<Rectangle>& arr, int low, int high) {
    if (low < high) {
        int pi = particion(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

const char caracteresPermitidos[] = "abcdefghijklmnopqrstuvwxyz";
const int numCaracteres = sizeof(caracteresPermitidos) - 1;

void generarNombreAleatorio(char nombre[], int longitud) {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribucion(0, numCaracteres - 1);

    for (int i = 0; i < longitud - 1; i++) {
        int indice = distribucion(generator);
        nombre[i] = caracteresPermitidos[indice];
    }

    // Agregar un carácter nulo al final para formar una cadena C válida.
    nombre[longitud - 1] = '\0';
}

struct infoDir{
    int valueMin;
    int valueMax;
    char nameUbq[23];
};

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



void RTreeInmersion(vector<Rectangle> info, char name[],int maxSizeOfData){
    int infoSize = info.size();
    ofstream archivo(name);

    if (!archivo.is_open()) {
            cout << "ERROR: can't open the direction: " << name << endl;
            return ;
        }
    if (infoSize < maxSizeOfData){
        archivo << "1" << endl;
        for (int i = 0; i < infoSize; i++){
            archivo << info[i].x1 << "\t"; 
            archivo << info[i].y1 << "\t";
            archivo << info[i].x2 << "\t";
            archivo << info[i].y2 << "\t" << endl;
        }
        archivo.close();
        return;
    }

    archivo << "0" << endl;

    vector<vector<Rectangle>> parts = dividirEnNPartes(info,maxSizeOfData);
    for (int i = 0; i < parts.size(); i++){
        generarNombreAleatorio(name,10);
        char ubq[23];
        strcpy(ubq, "./Data/");
        strcat(ubq, name);
        strcat(ubq, ".txt");
        int  x1 ,y1 ,x2, y2;
        for (int j = 0; j < parts[i].size(); j++){
            if (j == 0){
                x1 = parts[i][j].x1;
                y1 = parts[i][j].y1;
                x2 = parts[i][j].x2;
                y2 = parts[i][j].y2;
            }
            else{
                if (x1 > parts[i][j].x1){
                    x1 = parts[i][j].x1;
                }if (y1 > parts[i][j].y1){
                    y1 = parts[i][j].y1;
                }if (x2 < parts[i][j].x2){
                    x2 = parts[i][j].x2;
                }if (y2 < parts[i][j].y2){
                    y2 = parts[i][j].y2;
                }
            }
        }
        archivo << x1 << "\t" << y1 << "\t" << x2 << "\t" << y2 << "\t" << ubq << endl;
        RTreeInmersion(parts[i],ubq,maxSizeOfData);
    }
    archivo.close();
    return;
}


int main(){
    //CONFIGURACION///////////////////////////////////////
    bool debug = false;
    int maxSizeOfRectangle = 26;
    vector<Rectangle> rectangles = {
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
    bool erase = true;
    //////////////////////////////////////////////////////
    if (erase){
        for (const auto& entry : fs::directory_iterator("./Data")) {
            if (entry.is_regular_file() && entry.path().extension() == ".txt") {
                fs::remove(entry.path());
                if(debug)
                std::cout << "Se ha eliminado: " << entry.path() << std::endl;
        }
    }
    }
    quickSort(rectangles,0,rectangles.size()-1);

    char name[23] = "./Data/R_Tree_Raiz.txt";
    RTreeInmersion(rectangles, name, maxSizeOfRectangle);

    
    return 0;
}