#include<bits/stdc++.h> 
using namespace std;

// ESTO ES EL CODIGO DE LA CURVA DE HILBERT QUE ESTA EN WIKIPEDIA
// DSE MODIFICO SOLO UN NUMERO PUESTO QUE PARA ESTE CASO SE CONSIDERO COMO CONSTANTE
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
    for (s = 1; s < 8388608; s *= 2) {
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
    for (s = 8388608/2; s > 0; s /= 2) {
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
};
class LeafNode{
    private:
    Rectangle mbr;
    int64_t  valueCurveHilbert;
    void* info;

    public:
    //Iniciador de el LeafNode, por decirlo asi, aqui almacenaremos nuestro datos, para eso el puntero a void, pero quien sabe <--xDDDDD??
    LeafNode(int x1, int y1,int x2,int y2){
        mbr = {x1,y1,x2,y2};
        pair<int,int> centerMBR = mbr.getCenter();
        valueCurveHilbert = xy2d(centerMBR.first, centerMBR.second);
    }

    void printInfoLeaf(){
        cout<< "x1: " << mbr.x1 << ", x2: " << mbr.x2 << ", y1: " << mbr.y1 << ", y2: " << mbr.y2 << "\nvalueCurve: " << valueCurveHilbert << "\n";
    }

    Rectangle* getRectangle(){
        return &mbr;
    }

    int64_t getValueCurveHilbert(){
        return valueCurveHilbert;
    }

    bool IsThisRectangle(Rectangle rectangleExt){
        if (
            (mbr.x1 == rectangleExt.x1) &&
            (mbr.y1 == rectangleExt.y1) &&
            (mbr.x2 == rectangleExt.x2) &&
            (mbr.y2 == rectangleExt.y2) 
        ){
            return true;
        }
        return false;
    }
};

// Función de partición para QuickSort
int particion(vector<LeafNode*>& arr, int low, int high) {
    int pivot = arr[high]->getValueCurveHilbert();
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j]->getValueCurveHilbert() < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return (i + 1);
}

// Función QuickSort personalizada
void quickSort(vector<LeafNode*>& arr, int low, int high) {
    if (low < high) {
        int pi = particion(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

class Nodo{

private:
    //Informacion de la forma del nodo
    Rectangle mbr;
    //Este vector solo se usara si somos un "nodo" final
    vector<LeafNode*> childs_l;
    //Este vector se ocupara si tendremos otros "nodos" dentro y no LeafNodo
    vector<Nodo*> childs;
    //Determina el maximo de "nodos" que almacenara este Nodo
    int maxInfo;
    int64_t hilbertValueMax;
    int64_t hilbertValueMin;

    //createHilbertRTreeWVL:: vector<LeafNode*> -> 
    //Genera el RTree de Hilbert de manera recursiva, es para asistir a los demas a la funcion
    //publica
    void createHilvertRTreeWVL(vector<LeafNode*> info){
        int lenV = info.size();
        if (lenV <= maxInfo) {
            childs_l = info;
            recalculateSize();
            return;
        }
        int sublistSize = lenV / maxInfo;
        // Crea un vector de vectores para almacenar las sublistas.
        vector<vector<LeafNode*>> sublists(maxInfo);
        // Divide la lista original en "maxInfo" partes.
        for (int i = 0; i < maxInfo; ++i) {
            int startIndex = i * sublistSize;
            int endIndex = (i == maxInfo - 1) ? lenV : (i + 1) * sublistSize;
            // Copia los elementos de la lista original a la sublista.
            sublists[i].assign(info.begin() + startIndex, info.begin() + endIndex);
        }
        for(int i = 0; i < maxInfo; i++){
            childs.push_back(new Nodo(maxInfo,0,0));
            childs[i] -> createHilvertRTreeWVL(sublists[i]);
        }
        recalculateSize();
    }

public:
    Nodo(int sizeInfo,int64_t hilberMax,int64_t hiblberMin){
        maxInfo = sizeInfo;
        mbr = {0,0,0,0};
        hilbertValueMax = hilberMax;
        hilbertValueMin = hiblberMin;
    };

    int64_t getHilbertValueMax(){
        return hilbertValueMax;
    };
    int64_t getHilbertValueMin(){
        return hilbertValueMin;
    };

    bool inRange(int64_t value){
        if ((hilbertValueMax >= value) && (hilbertValueMin <= value)) return true;
        return false;
    }

    //createHilbertRTree:: vector<Rectangle> -> 
    //Genera el RTree de Hilbert
    void createHilbertRTree(vector<Rectangle> info){
        vector<LeafNode*> leafNodes;
        int sizeData = info.size();
        for(int i = 0; i < sizeData; i++){
            leafNodes.push_back(new LeafNode(info[i].x1,info[i].y1,info[i].x2,info[i].y2));
        }
        quickSort(leafNodes,0,sizeData - 1);
        if (sizeData <= maxInfo){
            childs_l = leafNodes;
            recalculateSize();
            return;
        }
        int sublistSize = sizeData / maxInfo;
        // Crea un vector de vectores para almacenar las sublistas.
        vector<vector<LeafNode*>> sublists(maxInfo);
        // Divide la lista original en "maxInfo" partes.
        for (int i = 0; i < maxInfo; ++i) {
            int startIndex = i * sublistSize;
            int endIndex = (i == maxInfo - 1) ? sizeData : (i + 1) * sublistSize;
            // Copia los elementos de la lista original a la sublista.
            sublists[i].assign(leafNodes.begin() + startIndex, leafNodes.begin() + endIndex);
        }
        for(int i = 0; i < maxInfo; i++){
            childs.push_back(new Nodo(maxInfo,0,0));
            childs[i] -> createHilvertRTreeWVL(sublists[i]);
        }
        recalculateSize();
        
    }

    
    //printInfo: void -> void
    //Muestra informacion importante de nuestro "Rectangulo Nodo"
    void printInfo(){
        cout<< "x1: " << mbr.x1 << ", x2: " << mbr.x2 << ", y1: " << mbr.y1 << ", y2: " << mbr.y2 << "\n" 
        << "hilbertMax: " << hilbertValueMax << ", hilbertmin: " << hilbertValueMin << "\n";
        if (childs_l.size() != 0){
            for(int i = 0; i < childs_l.size(); i++){
                cout << childs_l[i]->getValueCurveHilbert() << "\n"; 
            }
        }
        if(childs.size() != 0){
            for (int i = 0; i < childs.size(); i++){
                cout << "\n informacion del hijo " << i << "\n";
                childs[i]->printInfo();
            }
        }
    }

    //getRectangle: void -> void
    //Retorna un puntero al rectangulo, lo que permite edicion o obtencion de datos
    Rectangle* getRectangle(){
        return &mbr;
    }
    //recalculateSize: void -> void
    //Dados los hijos actuales, recalcula el tamaño actual del rectangulo
    void recalculateSize(){
        int x1Max = 0;
        int y1Max = 0;
        int x2Max = 0;
        int y2Max = 0;
        if (childs_l.empty() == false){
            for(int i = 0; i < childs_l.size(); i++){
                Rectangle* infoChild = childs_l[i]->getRectangle();
                int64_t thisHilbert = childs_l[i]->getValueCurveHilbert();
                if(i == 0){
                    hilbertValueMax = thisHilbert;
                    hilbertValueMin = thisHilbert;
                    x1Max = infoChild->x1;
                    y1Max = infoChild->y1;
                    x2Max = infoChild->x2;
                    y2Max = infoChild->y2;
                }
                else{
                    if (x1Max > infoChild->x1){
                        x1Max = infoChild->x1;
                    }
                    if (y1Max > infoChild->y1){
                        y1Max = infoChild->y1;
                    }
                    if (x2Max < infoChild->x2){
                        x2Max = infoChild->x2;
                    }
                    if (y2Max < infoChild->y2){
                        y2Max = infoChild->y2;
                    }
                    if(hilbertValueMax < thisHilbert){
                        hilbertValueMax = thisHilbert;
                    }
                    if(hilbertValueMin > thisHilbert){
                        hilbertValueMin = thisHilbert;
                    }
                }
            }
            mbr.setRectangle(x1Max,y1Max,x2Max,y2Max);
            return;
        }

        if (childs.empty() == true){
            mbr.setRectangle(0,0,0,0);
            return;
        }
        
        for(int i = 0; i < childs.size(); i++){
            Rectangle* infoChild = childs[i] -> getRectangle();
            int64_t thisHilbertMax = childs[i]-> getHilbertValueMax();
            int64_t thisHilbertMin = childs[i]-> getHilbertValueMin();
            if (i == 0){
                hilbertValueMax = thisHilbertMax;
                hilbertValueMin = thisHilbertMin;
                x1Max = infoChild->x1;
                y1Max = infoChild->y1;
                x2Max = infoChild->x2;
                y2Max = infoChild->y2;
            }
            else{
                if (x1Max > infoChild->x1){
                    x1Max = infoChild->x1;
                }
                if (y1Max > infoChild->y1){
                    y1Max = infoChild->y1;
                }
                if (x2Max < infoChild->x2){
                    x2Max = infoChild->x2;
                }
                if (y2Max < infoChild->y2){
                    y2Max = infoChild->y2;
                }
                if(hilbertValueMax < thisHilbertMax){
                    hilbertValueMax = thisHilbertMax;
                }
                if(hilbertValueMin > thisHilbertMin){
                    hilbertValueMin = thisHilbertMin;
                }
            }
        }
        mbr.setRectangle(x1Max,y1Max,x2Max,y2Max);
    }

    //searchRectangle: Rectangle, leafNode* -> int
    //Rectangle es el rectangulo que estamos buscando y leafNode es un nodo que sera modificada
    //para guardar la informacion que sea encontrada, si no es encontrado la funcion retornada 0
    //si lo encuentra retornara 1
    int searchRectangle(Rectangle rectangle, LeafNode* result,int64_t valueHilbertRectangle){
        if (valueHilbertRectangle < 0){
            pair<int,int> center = rectangle.getCenter();
            valueHilbertRectangle = xy2d(center.first, center.second);
        }

        if(childs_l.empty() == false){
            for(int i = 0; i < childs_l.size(); i++){
                int64_t value = childs_l[i]->getValueCurveHilbert();
                if (value == valueHilbertRectangle){
                    if(childs_l[i]->IsThisRectangle(rectangle)){
                        result = childs_l[i];
                        return 1;
                    }
                }
            }
            return 0;
        }
        if(childs.empty() == true){
            
            return 0;
        }

        for (int i = 0; i < childs.size(); i++){
            if (childs[i]->inRange(valueHilbertRectangle)){
                if (childs[i]->searchRectangle(rectangle,result,valueHilbertRectangle) == 1){
                    return 1;
                }
            }
        }
        return 0;
    }
};


int main(){
    ////SETTING////
    bool debug = false;
    int maxSizeOfRectangle = 100;
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
    //Configuracion busqueda//
    LeafNode* result;
    Rectangle rectangleSearched = {18, 53, 44, 81};
    //Configuracion busqueda//
    ///////////////
    Nodo myNodo(maxSizeOfRectangle,0,0);
    myNodo.createHilbertRTree(rectangles);

    if (debug){
        pair<int,int> center = rectangleSearched.getCenter();
        int64_t valueHilbertRectangle = xy2d(center.first, center.second);
        cout << "buscamos: " << valueHilbertRectangle << "\n";
        myNodo.printInfo();
    }

    int resultNum = myNodo.searchRectangle(rectangleSearched,result,-1); //El -1 es configuracion inicial, no sacar
    if (resultNum == 1){
        cout << "se encontro\n";
        return 1;
    }
    cout << "no se encontro\n";
    return 1;
}