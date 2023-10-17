#include <iostream>
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

Nodo SortTileRecursive(vector<Nodo> Nodos, int M){

    //Caso base
    if (Nodos.size() <= M) {
        Nodo Padre;

        //Calculamos el MBR del padre
        double xmin = 1e9, xmax = -1e9, ymin = 1e9, ymax = -1e9;
        for (auto nodo : Nodos) {
            xmin = min(xmin, nodo.MBR.x1);
            xmin = min(xmin, nodo.MBR.x2);
            xmax = max(xmax, nodo.MBR.x1);
            xmax = max(xmax, nodo.MBR.x2);
            ymin = min(ymin, nodo.MBR.y1);
            ymin = min(ymin, nodo.MBR.y2);
            ymax = max(ymax, nodo.MBR.y1);
            ymax = max(ymax, nodo.MBR.y2);
        }
        Rectangulo MBR = { xmin, ymin, xmax, ymax };
        Padre.MBR = MBR;

        //Agregamos los hijos al padre
        for (auto nodo : Nodos) {
            Padre.rectangulos.push_back(nodo.MBR);
            Padre.hijos.push_back(&nodo);
        }

        cout << "Hijos: " << Padre.hijos[1]->MBR.x1 << endl;

        Nodos.push_back(Padre);
        return Padre;
    }

    //Ordenamos los nodos segun su coordenada X
    sort(Nodos.begin(), Nodos.end(), [](const Nodo& izq, const Nodo& der) {
        return (izq.MBR.x1 + izq.MBR.x2) / 2 < (der.MBR.x1 + der.MBR.x2) / 2;
    });

    //Declarando la cantidad de grupos que se van a formar
    int n = Nodos.size();
    int S = sqrt(n/M);

    //Declarando los grupos de nodos segun su coordenada X
    vector<vector<Nodo>> GruposX;
    for (int i = 0; i < n; i += (M * S)) {
        vector<Nodo> GrupoX(Nodos.begin() + i, Nodos.begin() + min(i + (M * S), (int)n));
        GruposX.push_back(GrupoX);
    }

    //Creamos un arreglo de nodos que entregaremos a la recursion
    vector<Nodo>*Nodos2 = new vector<Nodo>;

    //Ahora debemos ordenar dentro de cada grupo segun su coordenada Y
    for(auto GrupoX : GruposX){
        sort(GrupoX.begin(), GrupoX.end(), [](const Nodo& izq, const Nodo& der) {
            return (izq.MBR.y1 + izq.MBR.y2) / 2 < (der.MBR.y1 + der.MBR.y2) / 2;
        });

        //Declarando los grupos de nodos segun su coordenada Y
        vector<vector<Nodo>> GruposY;
        for (int i = 0; i < GrupoX.size(); i += M) {
            vector<Nodo> GrupoY(Nodos.begin() + i, Nodos.begin() + min(i + M, (int)GrupoX.size()));
            GruposY.push_back(GrupoY);
        }

        //Rescatamos los nodos de cada GrupoY
        for(auto GrupoY : GruposY){
            vector<Nodo*>*nodo = new vector<Nodo*>;
            vector<Rectangulo>*rec = new vector<Rectangulo>;
            //Definimos el MBR de cada nodo
            double xmin = 1e9, xmax = -1e9, ymin = 1e9, ymax = -1e9;
            for (auto nodo2 : GrupoY) {
                nodo->push_back(&nodo2);
                rec->push_back(nodo2.MBR);
                xmin = min(xmin, nodo2.MBR.x1);
                xmin = min(xmin, nodo2.MBR.x2);
                xmax = max(xmax, nodo2.MBR.x1);
                xmax = max(xmax, nodo2.MBR.x2);
                ymin = min(ymin, nodo2.MBR.y1);
                ymin = min(ymin, nodo2.MBR.y2);
                ymax = max(ymax, nodo2.MBR.y1);
                ymax = max(ymax, nodo2.MBR.y2);
            }
            Rectangulo MBR = { xmin, ymin, xmax, ymax };
            Nodo hoja = {MBR,*rec,*nodo};
            Nodos2->push_back(hoja);
        }

    }

    SortTileRecursive(*Nodos2, M);

};

//Esta funcion construye el R-Tree
Nodo* ConstruirRtreeSTR(int M, vector<Rectangulo> rectangulos){
    
    //Ordenando los primeros puntos segun su coordenada X
    sort(rectangulos.begin(), rectangulos.end(), [](const Rectangulo& izq, const Rectangulo& der) {
        return (izq.x1 + izq.x2) / 2 < (der.x1 + der.x2) / 2;
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

    //Creamos un arreglo de nodos que entregaremos a la recursion
    vector<Nodo>*Nodos = new vector<Nodo>;

    //Ahora debemos ordenar dentro de cada grupo segun su coordenada Y
    for (auto GrupoX : GruposX) {  
        sort(GrupoX.begin(), GrupoX.end(), [](const Rectangulo& izq, const Rectangulo& der) {
            return (izq.y1 + izq.y2) / 2 < (der.y1 + der.y2) / 2;
        });

        //Declarando los grupos de puntos segun su coordenada Y
        vector<vector<Rectangulo>> GruposY;
        for (int i = 0; i < GrupoX.size(); i += M) {
            vector<Rectangulo> GrupoY(rectangulos.begin() + i, rectangulos.begin() + min(i + M, (int)GrupoX.size()));
            GruposY.push_back(GrupoY);
        }

        //Rescatamos los rectangulos de cada GrupoY
        for(auto GrupoY : GruposY){
            vector<Rectangulo>*rec = new vector<Rectangulo>;
            //Definimos el MBR de cada nodo
            double xmin = 1e9, xmax = -1e9, ymin = 1e9, ymax = -1e9;
            for (auto rectangulo : GrupoY) {
                rec->push_back(rectangulo);
                xmin = min(xmin, rectangulo.x1);
                xmin = min(xmin, rectangulo.x2);
                xmax = max(xmax, rectangulo.x1);
                xmax = max(xmax, rectangulo.x2);
                ymin = min(ymin, rectangulo.y1);
                ymin = min(ymin, rectangulo.y2);
                ymax = max(ymax, rectangulo.y1);
                ymax = max(ymax, rectangulo.y2);
            }
            Rectangulo MBR = { xmin, ymin, xmax, ymax };
            vector<Nodo*>*nulo = new vector<Nodo*>;
            Nodo hoja = {MBR,*rec,*nulo};
            Nodos->push_back(hoja);
        }
    }
    
    Nodo Padre = SortTileRecursive(*Nodos, M);
    Nodo *pointer = &Padre;
    return pointer;

};

// Función para eliminar nodos
void deleteNodes(Nodo* root) {
    if (!root) {
        return;
    }
    for (Nodo* child : root->hijos) {
        deleteNodes(child);
    }
    delete root;
}

void saveNode(ofstream& file, Nodo* node) {
    if (!node) {
        return;
    }

    // Serializa la estructura del nodo y escribe en el archivo
    file.write(reinterpret_cast<char*>(&node->MBR), sizeof(Rectangulo));
    int numChildren = node->hijos.size();
    file.write(reinterpret_cast<char*>(&numChildren), sizeof(int));
    
    for (Nodo* child : node->hijos) {
        saveNode(file, child);
    }
}

// Función para guardar el R-tree en un archivo binario
void saveRTree(Nodo* root, const string& filename) {
    ofstream file(filename, ios::out | ios::binary);
    if (!file.is_open()) {
        cerr << "Error al abrir el archivo para escritura." << endl;
        return;
    }

    // Aquí deberías serializar la estructura del árbol y escribirla en el archivo binario

    file.close();
}

Nodo* loadNode(ifstream& file) {
    Rectangulo MBR;
    file.read(reinterpret_cast<char*>(&MBR), sizeof(Rectangulo));
    int numChildren;
    file.read(reinterpret_cast<char*>(&numChildren), sizeof(int));
    
    Nodo* node = new Nodo();
    node->MBR = MBR;
    for (int i = 0; i < numChildren; ++i) {
        Nodo* child = loadNode(file);
        node->hijos.push_back(child);
    }
    return node;
}

// Función para cargar el R-tree desde un archivo binario
Nodo* loadRTree(const string& filename) {
    ifstream file(filename, ios::in | ios::binary);
    if (!file.is_open()) {
        cerr << "Error al abrir el archivo para lectura." << endl;
        return nullptr;
    }

    Nodo* root = nullptr;

    // Aquí deberías deserializar la estructura del árbol desde el archivo binario

    file.close();

    return root;
}

int main() {

    vector<Rectangulo> rectangulos = {
        Rectangulo{1, 2, 3, 4},
        Rectangulo{2, 3, 4, 5},
        Rectangulo{3, 4, 5, 6},
        Rectangulo{4, 5, 6, 7}
    };

    int M = 2; // Número máximo de hijos por nodo

    Nodo* root = ConstruirRtreeSTR(M, rectangulos);
    cout << "Árbol construido" << endl;
    saveRTree(root, "rtree.bin");
    cout << "Árbol guardado en rtree.bin" << endl;
    Nodo* loadedRoot = loadRTree("rtree.bin");
    cout << "Árbol cargado desde rtree.bin" << endl;

    // Realiza operaciones con el R-tree

    deleteNodes(root); // Limpia la memoria al final
    deleteNodes(loadedRoot);

    return 0;
};
