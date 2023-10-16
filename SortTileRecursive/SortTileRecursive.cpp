#include<bits/stdc++.h> 
#include <vector>
using namespace std;

//Estructura Rectángulo
struct Rectangulo{
    double x1, y1, x2, y2;
};

//Estructura Nodo que posee rectangulo con sus hijos
struct Nodo{
    Rectangulo MBR;
    vector<Rectangulo> rectangulos;
    vector<Nodo*> hijos;
};

//Esta funcion construye el R-Tree de manera recursiva
void ConstruirRtree(int M, vector<Rectangulo> rectangulos){
    
    //Ordenando los primeros n puntos segun su coordenada X
    sort(rectangulos.begin(), rectangulos.end(), [](const Rectangulo& izq, const Rectangulo& der) {
        return min(izq.x1,izq.x2) < min(der.x1, der.x2);
    });

    //Declarando la cantidad de grupos que se van a formar
    int n = rectangulos.size();
    int S = sqrt(n/M);

    //Declarando los grupos de puntos segun su coordenada X
    vector<vector<Rectangulo>> GruposX;
    for (int i = 0; i < n; i += (M * S)) {
        vector<Rectangulo> GrupoX(rectangulos.begin() + i, rectangulos.begin() + min(i + (M * S), (int)n));
        GruposX.push_back(GrupoX);
    }

    //Creando rectangurlos para cada grupo de puntos segun su posicion en X
    vector<Nodo> hyperrectangulos;
    for (auto GrupoX : GruposX) {
        double xmax = -1e9, xmin = 1e9, ymax = -1e9, ymin = 1e9;
        for (auto punto : GrupoX) {
            xmax = max(xmax, punto.x);
            xmin = min(xmin, punto.x);
            ymax = max(ymax, punto.y);
            ymin = min(ymin, punto.y);
        }
        HyperRectangulo hyperrectangulo = {xmin, ymin, xmax, ymax};
        hyperrectangulos.push_back(hyperrectangulo);
    }



    //Creamos recursivamente los nodos del R-Tree


};

//Queda crear el R-Tree recursivamente, ordenando por la coordenada Y