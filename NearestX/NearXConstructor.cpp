#include<bits/stdc++.h> 

using namespace std;
namespace fs = std::filesystem;

//Struct del triangulo
//Posee una funcion que retorna el centro en doubles
struct Rectangle{
    int x1, y1, x2, y2;

    void setRectangle(int new_x1,int new_y1,int new_x2,int new_y2){
        x1 = new_x1; y1 = new_y1; x2 = new_x2; y2 = new_y2;
    }
    //getCenter: void -> pair<int,int>
    //Obtiene un "valor" central de nuestro rectangulo, el cual sera util para calcular nuestro valor
    //en la curva de hilbert
    pair<double,double> getCenter(){
        pair<double,double> center;
        center.first = (x1 + x2 )/ 2.0 ;
        center.second = (y1 + y2 )/ 2.0 ;
        return center;
    };
};

// Función de partición para QuickSort
int particion(vector<Rectangle>& arr, int low, int high) {
    int pivot = arr[high].getCenter().first;
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j].getCenter().first < pivot) {
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

//Genera nombres aleatorios para que usemos
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

struct dirs{
    int x_min;
    int x_max;
    char direction[]
}

bool NearsX

bool buildNearsX(vector<Rectangle>& rectangles, char name[], int sizeMax){
    int sizeRectangles = rectangles.size();
    if(sizeRectangles <= sizeMax){
        ofstream archivo(name);
        
        if (!archivo.is_open()) {
            cout << "ERROR: can't open the direction: " << name << endl;
            return false;}

        archivo << 1 << endl;

        for (int i = 0; i < sizeRectangles; i++){
            archivo << rectangles[i].getCenter().first << "\t" << rectangles[i].x1 << "\t" << rectangles[i].y1 << "\t"
                << rectangles[i].x2 << "\t" << rectangles[i].y2 << "\t" << endl;
        }
        archivo.close();
        return true;
    }

    ofstream archivo(name);
    if (!archivo.is_open()) {
        cout << "ERROR: can't open the direction: " << name << endl;
    return false;}
    archivo << 0 << endl;

    vector<vector<Rectangle>> divisions = dividirEnNPartes()

}

int main(){

    bool debug = true;

    for (const auto& entry : fs::directory_iterator("./Data")) {
            if (entry.is_regular_file() && entry.path().extension() == ".txt") {
                fs::remove(entry.path());
                if(debug)
                std::cout << "Se ha eliminado: " << entry.path() << std::endl;
        }
    }

    vector<Rectangle> rectangles = {
        {4, 4, 6, 6},
        {2, 2, 4, 4},
        {1 ,1 ,5 ,5},
        {0, 0, 2, 2},
        {57, 38, 76, 66},
        {80, 10, 92, 41},
        {37, 6, 48, 40},
        {64, 46, 94, 87},
    };
    int m = 4;
    int n = rectangles.size();

    //Dejamos ordenado la lista
    quickSort(rectangles, 0, n - 1);

    char dir[] = "./Data/raiz.txt";

    buildNearsX(rectangles, dir, m);

    return 0;
}