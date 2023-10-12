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

void d2xy(int n, int d, int *x, int *y) {
    int rx, ry, s, t=d;
    *x = *y = 0;
    for (s = 1; s < n; s *= 2) {
        rx = 1 & (t/2);
        ry = 1 & (t ^ rx);
        rot(s, x, y, rx, ry);
        *x += s * rx;
        *y += s * ry;
        t /= 4;
    }
}

int xy2d (int n, int x, int y) {
    int rx, ry, s, d=0;
    for (s = n/2; s > 0; s /= 2) {
        rx = (x & s) > 0;
        ry = (y & s) > 0;
        d += s * s * ((3 * rx) ^ ry);
        rot(s, &x, &y, rx, ry);
    };
    return d;
};

struct Rectangle{
    double x1, y1, x2, y2;
};

class Nodo{

private:
    bool isLeaf;
    Rectangle mbr;
    vector<Nodo*> childs;
    int maxInfo;

public:
    Nodo(int sizeInfo,double maxRx,double maxRy, double minRx, double minRy){
        maxInfo = sizeInfo;
        mbr = {minRx,minRy,maxRx,maxRy};
        isLeaf = true;
    };
    
    void generateInfo(){
        cout<< "tamaño: " << maxInfo << "\n";
    }

    void insertNodo(Nodo &new_nodo){
        childs.push_back(&new_nodo);
    }
    void infoChilds(){
        for(int i = 0; i < childs.size(); i++){
            (*childs[i]).generateInfo();
        }
    }
    bool getIsLeaf(){
        return isLeaf;
    }
    Rectangle getCaractersOfRectangle(){

    }
};


int main(){
    ////SETTING////
    int maxSizeOfRectangle = 8;
    ///////////////
    Nodo myNodo(maxSizeOfRectangle,0,0,0,0);
    return 1;
}