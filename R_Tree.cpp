#include <iostream>
#include <bits/stdc++.h>
#include <vector>

using namespace std;

//Estructura Rectángulo
struct Rectangulo{
    double x1 , y1 , x2 , y2;
};

//Estructura Nodo que posee rectangulo con sus hijos
struct Nodo{
    Rectangulo MBR;
    vector<Rectangulo> Rectangulos;
    vector<Nodo*> Hijos;
};

vector<Nodo>&nearest_recursivo(vector<Nodo>&nodos,int m){
    int maxi = ceil(nodos.size()/m);
    vector<Nodo>*Nodos = new vector<Nodo>;
    for(int i=0; i<maxi;i++){
        vector<Rectangulo>*rec = new vector<Rectangulo>;
        for(int j=i*m;j<min((int)nodos.size(),(i+1)*m);j++){
            rec->push_back(nodos[j].MBR);
        }
        double xmax = -1e9, xmin = 1e9, ymax = -1e9, ymin = 1e9;

        for(int k=0;k<rec->size();k++){
            xmax = max(xmax,(*rec)[k].x1);
            xmax = max(xmax,(*rec)[k].x2);
            xmin = min(xmin,(*rec)[k].x1);
            xmin = min(xmin,(*rec)[k].x2);
            ymax = max(ymax,(*rec)[k].y1);
            ymax = max(ymax,(*rec)[k].y2);
            ymin = min(ymin,(*rec)[k].y1);
            ymin = min(ymin,(*rec)[k].y2);
        }
        Rectangulo mbr = {xmin,ymin,xmax,ymax};
        vector<Nodo*>*hijos = new vector<Nodo*>;
        for(int l=i*m;l<min((int)nodos.size(),(i+1)*m);l++){
            hijos->push_back(&nodos[l]);
        }
        Nodo hoja = {mbr,*rec,*hijos};
        Nodos->push_back(hoja);
    }

    // Caso base
    if(maxi == 1){
        return *Nodos ;
    }
    else{
        return nearest_recursivo(*Nodos,m);
    }

}


// m cantidad maxima de hijos por nodo
int nearestx(vector<Rectangulo>&Rectangulos,int m){

    sort(Rectangulos.begin(), Rectangulos.end(), [](const Rectangulo& a, const Rectangulo& b) {
        return (a.x1 + a.x2) / 2 < (b.x1 + b.x2) / 2;
    });
   
    // Hasta aqui esta todo ordenado
    int maxi = ceil(Rectangulos.size()/m);
    vector<Nodo>*Nodos = new vector<Nodo>;
    for(int i=0; i<maxi;i++){
        vector<Rectangulo>*rec = new vector<Rectangulo>;
        for(int j=i*m;j<min((int)Rectangulos.size(),(i+1)*m);j++){
            rec->push_back(Rectangulos[j]);
        }
        double xmax = -1e9, xmin = 1e9, ymax = -1e9, ymin = 1e9; 
        
        for(int k=0;k<rec->size();k++){
            xmax = max(xmax,(*rec)[k].x1);
            xmax = max(xmax,(*rec)[k].x2);
            xmin = min(xmin,(*rec)[k].x1);
            xmin = min(xmin,(*rec)[k].x2);
            ymax = max(ymax,(*rec)[k].y1);
            ymax = max(ymax,(*rec)[k].y2);
            ymin = min(ymin,(*rec)[k].y1);
            ymin = min(ymin,(*rec)[k].y2);
        }
        Rectangulo mbr = {xmin,ymin,xmax,ymax};
        vector<Nodo*>*nulo = new vector<Nodo*>;
        Nodo hoja = {mbr,*rec,*nulo};
        Nodos->push_back(hoja);
    }
    nearest_recursivo(*Nodos,m).size();
    return 0;
} 

// Función para serializar un nodo
void serializarNodo(ofstream& archivo, Nodo* nodo) {
    if (!nodo) {
        archivo.write(reinterpret_cast<char*>(0), sizeof(int));  // Indicador de nodo nulo
    } else {
        archivo.write(reinterpret_cast<char*>(nodo), sizeof(Nodo));
        for (Rectangulo& rect : nodo->Rectangulos) {
            archivo.write(reinterpret_cast<char*>(&rect), sizeof(Rectangulo));
        }
        int numHijos = nodo->Hijos.size();
        archivo.write(reinterpret_cast<char*>(&numHijos), sizeof(int));
        for (Nodo* hijo : nodo->Hijos) {
            serializarNodo(archivo, hijo);
        }
    }
}

// Función para deserializar un nodo
Nodo* deserializarNodo(ifstream& archivo) {
    Nodo* nodo = new Nodo;
    archivo.read(reinterpret_cast<char*>(nodo), sizeof(Nodo));
    for (Rectangulo& rect : nodo->Rectangulos) {
        archivo.read(reinterpret_cast<char*>(&rect), sizeof(Rectangulo));
    }
    int numHijos;
    archivo.read(reinterpret_cast<char*>(&numHijos), sizeof(int));
    for (int i = 0; i < numHijos; i++) {
        Nodo* hijo = deserializarNodo(archivo);
        nodo->Hijos.push_back(hijo);
    }
    return nodo;
}

// Función para guardar el árbol R-Tree en un archivo binario
void guardarArbol(const char* nombreArchivo, Nodo* raiz) {
    ofstream archivo(nombreArchivo, ios::binary);
    serializarNodo(archivo, raiz);
    archivo.close();
}

// Función para cargar el árbol R-Tree desde un archivo binario
Nodo* cargarArbol(const char* nombreArchivo) {
    ifstream archivo(nombreArchivo, ios::binary);
    Nodo* raiz = deserializarNodo(archivo);
    archivo.close();
    return raiz;
}

int main() {
    // Crea un conjunto de rectángulos de prueba
    vector<Rectangulo> RectangulosPrueba;
    Rectangulo r1 = {1.0, 2.0, 3.0, 4.0};
    Rectangulo r2 = {2.0, 3.0, 4.0, 5.0};
    RectangulosPrueba.push_back(r1);
    RectangulosPrueba.push_back(r2);

    // Define el valor de m (número máximo de hijos por nodo)
    int m = 2;

    // Llama a la función nearestx para construir el árbol y realizar la búsqueda
    int resultado = nearestx(RectangulosPrueba, m);

    // Imprime el resultado
    cout << "Número de resultados encontrados: " << resultado << endl;

    return 0;
};




