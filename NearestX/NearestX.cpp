#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cmath>

using namespace std;

// Estructura Rectángulo
struct Rectangulo {
    double x1, y1, x2, y2;
};

// Estructura Nodo que posee rectángulo con sus hijos
struct Nodo {
    Rectangulo MBR;
    vector<Rectangulo> Rectangulos;
    vector<Nodo*> Hijos;
};

// Función para calcular el área de un rectángulo
double calcularArea(const Rectangulo& rect) {
    return (rect.x2 - rect.x1) * (rect.y2 - rect.y1);
}

// Función para calcular el área del MBR de un nodo
double calcularAreaMBR(const Nodo& nodo) {
    return calcularArea(nodo.MBR);
}

// Función para comparar dos rectángulos por su coordenada X
bool compararPorX(const Rectangulo& a, const Rectangulo& b) {
    return (a.x1 + a.x2) / 2 < (b.x1 + b.x2) / 2;
}

// Función para construir un árbol R recursivamente
Nodo* construirArbolR(vector<Rectangulo>& Rectangulos, int m) {
    if (Rectangulos.empty()) {
        return nullptr;
    }

    if (Rectangulos.size() <= m) {
        Nodo* nodoHoja = new Nodo;
        nodoHoja->Rectangulos = Rectangulos;
        nodoHoja->MBR = Rectangulos[0];

        for (const Rectangulo& rect : Rectangulos) {
            nodoHoja->MBR.x1 = min(nodoHoja->MBR.x1, rect.x1);
            nodoHoja->MBR.y1 = min(nodoHoja->MBR.y1, rect.y1);
            nodoHoja->MBR.x2 = max(nodoHoja->MBR.x2, rect.x2);
            nodoHoja->MBR.y2 = max(nodoHoja->MBR.y2, rect.y2);
        }

        return nodoHoja;
    }

    // Ordenar los rectángulos por coordenada X
    sort(Rectangulos.begin(), Rectangulos.end(), compararPorX);

    int division = ceil(sqrt(Rectangulos.size()));
    vector<vector<Rectangulo>> divisiones;

    for (int i = 0; i < division; i++) {
        int inicio = i * (Rectangulos.size() / division);
        int fin = (i + 1) * (Rectangulos.size() / division);
        if (i == division - 1) {
            fin = Rectangulos.size();
        }
        vector<Rectangulo> division(Rectangulos.begin() + inicio, Rectangulos.begin() + fin);
        divisiones.push_back(division);
    }

    vector<Nodo*> hijos;
    vector<Rectangulo> MBRHijos;

    for (const vector<Rectangulo>& division : divisiones) {
        Nodo* hijo = construirArbolR(division, m);
        if (hijo) {
            hijos.push_back(hijo);
            MBRHijos.push_back(hijo->MBR);
        }
    }

    Nodo* nodo = new Nodo;
    nodo->Rectangulos = Rectangulos;
    nodo->Hijos = hijos;

    if (!MBRHijos.empty()) {
        nodo->MBR = MBRHijos[0];
        for (const Rectangulo& MBRHijo : MBRHijos) {
            nodo->MBR.x1 = min(nodo->MBR.x1, MBRHijo.x1);
            nodo->MBR.y1 = min(nodo->MBR.y1, MBRHijo.y1);
            nodo->MBR.x2 = max(nodo->MBR.x2, MBRHijo.x2);
            nodo->MBR.y2 = max(nodo->MBR.y2, MBRHijo.y2);
        }
    }

    return nodo;
}

// Función para guardar un nodo en un archivo binario
void guardarNodoEnArchivo(const Nodo* nodo, ofstream& archivo) {
    if (nodo) {
        // Guardar el MBR del nodo
        archivo.write(reinterpret_cast<const char*>(&nodo->MBR), sizeof(Rectangulo));
    
        // Guardar el número de rectángulos en el nodo
        int numRectangulos = nodo->Rectangulos.size();
        archivo.write(reinterpret_cast<const char*>(&numRectangulos), sizeof(int));

        // Guardar los rectángulos del nodo
        for (const Rectangulo& rect : nodo->Rectangulos) {
            archivo.write(reinterpret_cast<const char*>(&rect), sizeof(Rectangulo));
        }

        // Guardar los hijos
        int numHijos = nodo->Hijos.size();
        archivo.write(reinterpret_cast<const char*>(&numHijos), sizeof(int));
        for (const Nodo* hijo : nodo->Hijos) {
            guardarNodoEnArchivo(hijo, archivo);
        }
    }
}

// Función para guardar el árbol R en un archivo binario
void guardarArbolR(Nodo* raiz, const string& nombreArchivo) {
    ofstream archivo(nombreArchivo, ios::binary);
    if (archivo.is_open()) {
        guardarNodoEnArchivo(raiz, archivo);
        archivo.close();
        cout << "Árbol R guardado en disco en formato binario." << endl;
    } else {
        cerr << "No se pudo abrir el archivo binario para guardar el árbol R." << endl;
    }
}

int main() {
    // Define tus datos de entrada (rectángulos y valor de "m")
    vector<Rectangulo> Rectangulos = {
        {0, 0, 2, 2},
        {1, 1, 3, 3},
        {3, 1, 4, 2},
        {2, 2, 4, 4},
        // Agrega más rectángulos según tus necesidades
    };

    int m = 2;  // Reemplaza con el valor de "m" que desees

    // Construir el árbol R
    Nodo* arbolR = construirArbolR(Rectangulos, m);

    // Guardar el árbol R en un archivo binario
    guardarArbolR(arbolR, "arbolR.bin");

    // Realiza las operaciones deseadas con el árbol R
    // ...

    return 0;
}

// Función para cargar un nodo desde un archivo binario
Nodo* cargarNodoDesdeArchivo(ifstream& archivo) {
    Nodo* nodo = new Nodo;

    // Cargar el MBR del nodo
    archivo.read(reinterpret_cast<char*>(&nodo->MBR), sizeof(Rectangulo));

    // Cargar el número de rectángulos en el nodo
    int numRectangulos;
    archivo.read(reinterpret_cast<char*>(&numRectangulos), sizeof(int));

    // Cargar los rectángulos del nodo
    for (int i = 0; i < numRectangulos; i++) {
        Rectangulo rect;
        archivo.read(reinterpret_cast<char*>(&rect), sizeof(Rectangulo));
        nodo->Rectangulos.push_back(rect);
    }

    // Cargar el número de hijos
    int numHijos;
    archivo.read(reinterpret_cast<char*>(&numHijos), sizeof(int));

    // Cargar los hijos
    for (int i = 0; i < numHijos; i++) {
        Nodo* hijo = cargarNodoDesdeArchivo(archivo);
        nodo->Hijos.push_back(hijo);
    }

    return nodo;
}

// Función para cargar el árbol R desde un archivo binario
Nodo* cargarArbolRDesdeArchivo(const string& nombreArchivo) {
    ifstream archivo(nombreArchivo, ios::binary);
    if (archivo.is_open()) {
        Nodo* raiz = cargarNodoDesdeArchivo(archivo);
        archivo.close();
        cout << "Árbol R cargado desde el archivo binario." << endl;
        return raiz;
    } else {
        cerr << "No se pudo abrir el archivo binario para cargar el árbol R." << endl;
        return nullptr;
    }
}