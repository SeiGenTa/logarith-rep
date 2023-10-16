#include<bits/stdc++.h> 
#include <vector>
using namespace std;

//Estructura Punto
struct Punto{
    double x , y;
};

//Estructura Rectángulo
struct Rectangulo{
    double x1, y1, x2, y2;
};

//Estructura Nodo que posee rectangulo con sus hijos
struct Nodo{
    Rectangulo MBR;
    vector<Rectangulo> Rectangulos;
    vector<Nodo*> Hijos;
};

//Esta funcion construye el R-Tree de manera recursiva
void ConstruirRtree(int M, vector<Punto> puntos){
    
    //Ordenando los primeros n puntos segun su coordenada X
    sort(puntos.begin(), puntos.end(), [](const Punto& izq, const Punto& der) {
        return izq.x < der.x;
    });

    //Declarando la cantidad de grupos que se van a formar
    int n = puntos.size();
    int S = sqrt(n/M);

    //Declarando los grupos de puntos segun su coordenada X
    vector<vector<Punto>> GruposX;
    for (int i = 0; i < n; i += (M * S)) {
        vector<Punto> GrupoX(puntos.begin() + i, puntos.begin() + min(i + (M * S), (int)n));
        GruposX.push_back(GrupoX);
    }

    //Creando rectangurlos para cada grupo de puntos segun su posicion en X
    vector<Rectangulo> rectangulos;
    for (auto GrupoX : GruposX) {
        double xmax = -1e9, xmin = 1e9, ymax = -1e9, ymin = 1e9;
        for (auto punto : GrupoX) {
            xmax = max(xmax, punto.x);
            xmin = min(xmin, punto.x);
            ymax = max(ymax, punto.y);
            ymin = min(ymin, punto.y);
        }
        Rectangulo rectangulo = {xmin, ymin, xmax, ymax};
        rectangulos.push_back(rectangulo);
    }

    //Creamos recursivamente los nodos del R-Tree
    Nodo* raiz = construirRecursiva(M, rectangulos);

};

//Queda crear el R-Tree recursivamente, ordenando por la coordenada Y