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

bool generate(vector<Rectangles> rectangulo){
        if (erase){
        for (const auto& entry : fs::directory_iterator("./Data")) {
            if (entry.is_regular_file() && entry.path().extension() == ".txt") {
                fs::remove(entry.path());
                if(debug)
                std::cout << "Se ha eliminado: " << entry.path() << std::endl;
        }
    }
}


int main(){
    //CONFIGURACION///////////////////////////////////////
    bool debug = false;
    int maxSizeOfRectangle = 4;
    vector<Rectangle> rectangles = {
        {0, 0, 2, 2},
        {5, 5, 10, 10},
        {3, 1, 7, 4},
        {10, 15, 20, 30},
        {5, 8, 15, 25},
        {30, 40, 45, 50},
        {25, 35, 60, 70},
        {12, 18, 30, 40},
        {2, 3, 8, 12},
        {18, 22, 40, 60},
        {7, 9, 20, 25},
        {8, 10, 14, 19},
        {32, 43, 52, 61},
        {14, 19, 29, 37},
        {23, 28, 44, 55},
        {12, 20, 35, 45},
        {27, 33, 50, 55},
        {45, 50, 60, 70},
        {17, 23, 33, 42},
        {35, 40, 55, 60},
        {4, 8, 12, 16},
        {50, 55, 70, 75},
        {15, 20, 38, 45},
        {29, 37, 48, 57},
        {19, 25, 41, 53},
        {11, 14, 26, 30},
        {30, 35, 52, 63},
        {9, 12, 22, 28},
        {26, 32, 43, 52},
        {7, 10, 18, 23},
        {34, 42, 57, 68},
        {16, 21, 37, 48},
        {21, 26, 39, 43},
        {28, 37, 51, 59},
        {8, 12, 24, 29},
        {1, 2, 4, 7},
        {13, 16, 31, 35},
        {25, 28, 47, 52},
        {6, 9, 20, 24},
        {37, 40, 58, 63},
        {14, 18, 28, 32},
        {3, 5, 11, 14},
        {19, 23, 34, 39},
        {32, 37, 49, 54},
        {9, 11, 21, 25},
        {22, 25, 41, 46},
        {5, 7, 16, 19},
        {11, 13, 24, 28},
        {26, 29, 45, 51},
        {4, 6, 15, 18},
        {12, 15, 29, 34},
        {27, 30, 53, 57},
        {7, 9, 19, 22},
        {16, 18, 34, 38},
        {31, 34, 55, 60},
        {8, 10, 21, 24},
        {17, 20, 38, 43},
        {35, 38, 56, 61},
        {6, 8, 18, 22},
        {15, 17, 33, 38},
        {30, 33, 52, 58},
        {5, 7, 16, 19},
        {13, 15, 29, 33},
        {28, 31, 49, 54},
        {3, 5, 12, 15},
        {11, 13, 26, 30},
        {25, 28, 44, 49},
        {2, 3, 7, 8},
        {12, 14, 27, 31},
        {24, 27, 43, 48},
        {1, 2, 4, 6},
        {10, 12, 23, 27},
        {22, 25, 39, 44},
        {9, 10, 19, 22},
        {18, 20, 36, 41},
        {34, 37, 53, 59},
        {7, 8, 17, 20},
        {16, 18, 32, 37},
        {31, 34, 49, 55},
        {6, 7, 15, 18},
        {14, 16, 30, 34},
        {29, 32, 46, 52},
        {5, 6, 12, 14},
        {13, 15, 28, 32},
        {27, 30, 45, 51},
        {4, 5, 10, 12},
        {11, 13, 25, 29},
        {26, 29, 43, 48},
        {3, 4, 7, 9},
        {9, 11, 21, 25},
        {24, 27, 40, 45},
        {2, 3, 5, 7},
        {8, 10, 18, 22},
        {22, 25, 36, 41},
        {1, 2, 3, 5},
        {7, 9, 16, 20},
        {20, 22, 34, 39},
        {6, 7, 13, 16},
        {14, 16, 28, 32},
        {28, 30, 45, 50},
        {5, 6, 11, 14},
        {12, 14, 26, 30},
        {25, 27, 40, 45},
        {4, 5, 9, 11},
        {10, 12, 21, 25},
        {23, 25, 37, 42},
        {8, 9, 17, 19},
        {17, 19, 33, 38},
        {32, 34, 51, 56},
        {7, 8, 15, 17},
        {15, 17, 29, 33},
        {30, 32, 47, 51},
        {6, 7, 12, 14},
        {13, 15, 27, 31},
        {27, 29, 42, 46},
        {5, 6, 10, 12},
        {11, 13, 24, 28},
        {24, 26, 39, 43},
        {4, 5, 8, 10},
        {9, 11, 20, 24},
        {22, 24, 36, 40},
        {3, 4, 6, 8},
        {7, 9, 16, 19},
        {19, 21, 32, 37},
        {2, 3, 4, 6},
        {6, 8, 14, 17},
        {16, 18, 30, 34},
        {1, 2, 2, 5},
        {5, 7, 12, 16},
        {13, 15, 26, 30},
        // Continúan los objetos Rectangle...
        // Agrega más objetos Rectangle aquí hasta superar los 200 objetos
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