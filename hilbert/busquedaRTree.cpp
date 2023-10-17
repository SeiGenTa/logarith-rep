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

    bool isEqual(int x1_e, int y1_e, int x2_e, int y2_e){
        if (x1_e != x1)return false;
        if (y1_e != y1)return false;
        if (x2_e != x2)return false;
        if (y2_e != y2)return false;
        return true;
    }
};
struct infoDir{
    int valueMin;
    int valueMax;
    char nameUbq[23];
};
struct Node{
    int64_t valueHilbert;
    int x1,	y1,	x2,	y2;
};

bool searchRectanguleInter(Rectangle* rectangle, int64_t &valueHilbert, char dir[], Rectangle* response){
    ifstream  archivo(dir);
    if (!archivo.is_open()){
        cout << "ERROR: can't open the direct: " << dir << endl;
        return false;
    }
    string linea;
    getline(archivo, linea);

    string _; // <- basurero

    istringstream ss(linea);
    getline(ss, _, '\t');
    string isLeaf;
    getline(ss, isLeaf, '\t');
    if (stoi(isLeaf) == 1){
        for (int i = 0; i < 3; i++)
            getline(archivo, linea);

        vector<Node> info;

        while(getline(archivo, linea)){
            istringstream ss(linea);
            string valueHilbert, x1, y1, x2, y2;
            Node thisInfo;
            if(getline(ss, valueHilbert, '\t') && getline(ss, x1, '\t') &&
            getline(ss, y1, '\t') && getline(ss, x2, '\t') && getline(ss, y2, '\t')){
                thisInfo.valueHilbert = stoll(valueHilbert);
                thisInfo.x1 = stoi(x1);
                thisInfo.y1 = stoi(y1);
                thisInfo.x2 = stoi(x2);
                thisInfo.y2 = stoi(y2);
                info.push_back(thisInfo);
            }
        }
        archivo.close();

        for (int i = 0; i < info.size(); i++){
            if (valueHilbert == info[i].valueHilbert){
                if(rectangle->isEqual(info[i].x1,info[i].y1,info[i].x2,info[i].y2)){
                    response->x1 = info[i].x1;
                    response->y1 = info[i].y1;
                    response->x2 = info[i].x2;
                    response->y2 = info[i].y2;
                    return true;
                }
            }
            else if (valueHilbert < info[i].valueHilbert){
                return false;
            }

        }
        
        
        return false;
    }

    vector<infoDir> infosDir;

    for (int i = 0; i < 3; i++)
        getline(archivo, linea);

    while(getline(archivo, linea)){
        istringstream ss(linea);
        string limInf, limSup, dir;
        if(getline(ss, limInf, '\t') &&
            getline(ss, limSup, '\t') &&
            getline(ss, dir, '\t')){
                char nameDir[23];
                strcpy(nameDir, dir.c_str());
                infoDir dataInLine;
                dataInLine.valueMin = stoi(limInf);
                dataInLine.valueMax = stoi(limSup);
                strcpy(dataInLine.nameUbq,nameDir);
                infosDir.push_back(dataInLine);
            }
    }
    archivo.close();

    for (int i = 0; i < infosDir.size(); i++){
        if ((valueHilbert <= infosDir[i].valueMax) &&
            (valueHilbert >= infosDir[i].valueMin))
            if (searchRectanguleInter(rectangle,valueHilbert,infosDir[i].nameUbq,response))
                return true;
        else{
            return false;
        }
    }

    
    return false;
};

bool searchRectangule(Rectangle* rectangle, Rectangle* response){
    int64_t valueHilbert = rectangle->getValueCurveHilbert();
    char name[23] = "./Data/R_Tree_Raiz.txt";
    return searchRectanguleInter(rectangle, valueHilbert, name, response);
};

int main(){
    //CONFIGURATION//
    Rectangle response;
    Rectangle rectangle = {65, 50, 93, 90};
    //////
    if (searchRectangule(&rectangle,&response)){
        cout << "x1: " << response.x1 << "\tx2: " << response.x2 << "\ty1: " << response.y1 << "\ty2: " << response.y2 << endl;
    }
    
};