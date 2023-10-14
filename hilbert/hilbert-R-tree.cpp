#include<bits/stdc++.h> 
using namespace std;
void rot(int n, int *x, int *y, int rx, int ry) {
    int t;
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

void d2xy(int d, int *x, int *y) {
    int rx, ry, s, t=d;
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

int xy2d (int x, int y) {
    int rx, ry, s, d=0;
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
    int valueCurveHilbert;
    void* info;

    public:
    //Iniciador de el LeafNode, por decirlo asi, aqui almacenaremos nuestro datos, para eso el puntero a void, pero quien sabe
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

    int getValueCurveHilbert(){
        return valueCurveHilbert;
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

public:
    Nodo(int sizeInfo){
        maxInfo = sizeInfo;
        mbr = {0,0,0,0};
    };

    //createHilbertRTree:: vector<Rectangle> ->
    //Genera el RTree de Hilbert
    void createHilbertRTree(vector<Rectangle> info){
        vector<LeafNode*> LeafNodes;
        for(int i = 0; i < info.size(); i++){
            LeafNodes.push_back(new LeafNode(info[i].x1,info[i].y1,info[i].x2,info[i].y2));
        }
        quickSort(LeafNodes,0,LeafNodes.size() - 1);
        /*
        for(int i = 0; i < LeafNodes.size(); i ++){
            LeafNodes[i]->printInfoLeaf();
        }
        */
    }
    
    //printInfo: void -> void
    //Muestra informacion importante de nuestro "Rectangulo Nodo"
    void printInfo(){
        cout<< "x1: " << mbr.x1 << ", x2: " << mbr.x2 << ", y1: " << mbr.y1 << ", y2: " << mbr.y2 << "\n";
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
            }
            mbr.setRectangle(x1Max,y1Max,x2Max,y2Max);
            return;
        }

        if (childs.empty() == 0){
            mbr.setRectangle(0,0,0,0);
            return;
        }
        
        for(int i = 0; i < childs.size(); i++){
            Rectangle* infoChild = childs[i] -> getRectangle();
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
        }
        mbr.setRectangle(x1Max,y1Max,x2Max,y2Max);
    }
};


int main(){
    ////SETTING////
    int maxSizeOfRectangle = 8;
    vector<Rectangle> rectangles = {
        {2,2,4,4},
        {0,0,2,2},
    };
    ///////////////
    Nodo myNodo(maxSizeOfRectangle);
    myNodo.createHilbertRTree(rectangles);
    myNodo.printInfo();
    return 1;
}