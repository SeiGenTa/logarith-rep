#include<bits/stdc++.h> 
#include <chrono>
namespace fs = std::filesystem;
using namespace std;
using namespace std::chrono;

struct Rectangle{
    int x1, y1, x2, y2;

    bool isEqual(int x1_e, int y1_e, int x2_e, int y2_e){
        if (x1_e != x1)return false;
        if (y1_e != y1)return false;
        if (x2_e != x2)return false;
        if (y2_e != y2)return false;
        return true;
    }
};

struct Nodo{
    int x1,y1,x2,y2;
    char dir[23];
};

bool searchRTree(char dir[], Rectangle* rectangleSearched, int* amountR){
    ifstream  archivo(dir);
    if (!archivo.is_open()){
        cout << "ERROR: can't open the direct: " << dir << endl;
        return false;
    }
    (*amountR) ++;

    string linea;
    getline(archivo, linea);

    istringstream ss(linea);

    string isLeaf;

    getline(ss, isLeaf, '\t');

    if (stoi(isLeaf) == 0){
        
        vector<Nodo> nodos;

        while(getline(archivo, linea)){
            istringstream ss(linea);
            string x1, y1, x2, y2, dir; 
   
            if(getline(ss, x1, '\t') && getline(ss, y1, '\t') &&
            getline(ss, x2, '\t') && getline(ss, y2, '\t') && getline(ss, dir, '\t')){
                
                Nodo thisNodo;
                strcpy(thisNodo.dir, dir.c_str());
                thisNodo.x1 = stoi(x1);
                thisNodo.y1 = stoi(y1);
                thisNodo.x2 = stoi(x2);
                thisNodo.y2 = stoi(y2);
                nodos.push_back(thisNodo);
            }
        }
        
        archivo.close();
        for(int i = 0; i < nodos.size(); i++){
            /*cout << nodos[i].x1 << "\t" << nodos[i].y1 << "\t" << nodos[i].x2 << "\t" << nodos[i].y2 << "\t"
            << nodos[i].dir << "\n";*/
            if (nodos[i].x1 <= rectangleSearched->x1 && nodos[i].y1 <= rectangleSearched->y1
            && nodos[i].x2 >= rectangleSearched->x2 && nodos[i].y2 >= rectangleSearched->y2){
                if (searchRTree(nodos[i].dir,rectangleSearched,amountR)){
                    return true;
                }
            }
        }
        return false;
    }

    

    vector<Rectangle> rectangles;

    while(getline(archivo, linea)){
            istringstream ss(linea);
            string x1, y1, x2, y2; 
   
            if(getline(ss, x1, '\t') && getline(ss, y1, '\t') &&
            getline(ss, x2, '\t') && getline(ss, y2, '\t') ){
                
                Rectangle thisRectangle;

                thisRectangle.x1 = stoi(x1);
                thisRectangle.y1 = stoi(y1);
                thisRectangle.x2 = stoi(x2);
                thisRectangle.y2 = stoi(y2);

                rectangles.push_back(thisRectangle);
            }
        }
    archivo.close();

    for(int i = 0; i < rectangles.size(); i++){
        if(rectangleSearched->isEqual(rectangles[i].x1,rectangles[i].y1,rectangles[i].x2,rectangles[i].y2)){
            return true;
        }
    }
    return false;
}

int main(){
    auto inicio = high_resolution_clock::now();
    Rectangle searched = {6, 7, 12, 14};
    char direccion[] = "./Data/R_Tree_Raiz.txt";
    int cantidadLecturas = 0;
    bool isFinded = searchRTree(direccion,&searched,&cantidadLecturas);
    auto fin = high_resolution_clock::now();

    auto tiempo_transcurrido = duration_cast<milliseconds>(fin - inicio).count();

    if (isFinded){
        cout << "el rectangulo se encontro! \nse hicieron: " << cantidadLecturas << " lecturas" << endl
        << "tardo: " << tiempo_transcurrido <<endl;
    }else{
        cout << "no se encontro, cantidad de archivos leidos: " << cantidadLecturas << " leido" << endl
        << "tardo: " << tiempo_transcurrido << endl;
    }
    return 1;
}