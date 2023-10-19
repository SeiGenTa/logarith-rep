#include<bits/stdc++.h> 
#include <chrono>
namespace fs = std::filesystem;
using namespace std;
using namespace std::chrono;

struct Rectangle{
    int x1, y1, x2, y2;

    void setRectangle(int new_x1,int new_y1,int new_x2,int new_y2){
        x1 = new_x1; y1 = new_y1; x2 = new_x2; y2 = new_y2;
    }
	pair<double,double> getCenter(){
        pair<double,double> center;
        center.first = (x1 + x2 )/ 2.0 ;
        center.second = (y1 + y2 )/ 2.0 ;
        return center;
    };
    //getCenter2: void -> pair<int,int>
    //Obtiene un "valor" central de nuestro rectangulo, el cual sera util para calcular nuestro valor
    //en la curva de hilbert
    pair<int,int> getCenter2(){
        pair<int,int> center;
        center.first = (x1 + x2 )/ 2 ;
        center.second = (y1 + y2 )/ 2 ;
        return center;
    };

    bool isEqual(int nx1, int ny1, int nx2, int ny2){
        if ((nx1 == x1) && (nx2 == x2) && (ny1 == y1) && (ny2 == y2))
            return true;
        return false;
    }
};

struct Nodo{
    int x1,y1,x2,y2;
    char dir[23];
};

// Función para calcular los valores mínimos y máximos
void calcularLimites(const std::vector<Nodo>& nodos, int& x1_min, int& y1_min, int& x2_max, int& y2_max) {
    if (nodos.empty()) {
        x1_min = y1_min = x2_max = y2_max = 0;
        return;
    }

    x1_min = y1_min = std::numeric_limits<int>::max();
    x2_max = y2_max = std::numeric_limits<int>::min();

    for (const Nodo& nodo : nodos) {
        if (nodo.x1 < x1_min) {
            x1_min = nodo.x1;
        }
        if (nodo.y1 < y1_min) {
            y1_min = nodo.y1;
        }
        if (nodo.x2 > x2_max) {
            x2_max = nodo.x2;
        }
        if (nodo.y2 > y2_max) {
            y2_max = nodo.y2;
        }
    }
}

void calcularLimites2(const std::vector<Rectangle>& nodos, int& x1_min, int& y1_min, int& x2_max, int& y2_max) {
    if (nodos.empty()) {
        x1_min = y1_min = x2_max = y2_max = 0;
        return;
    }

    x1_min = y1_min = std::numeric_limits<int>::max();
    x2_max = y2_max = std::numeric_limits<int>::min();

    for (const Rectangle& nodo : nodos) {
        if (nodo.x1 < x1_min) {
            x1_min = nodo.x1;
        }
        if (nodo.y1 < y1_min) {
            y1_min = nodo.y1;
        }
        if (nodo.x2 > x2_max) {
            x2_max = nodo.x2;
        }
        if (nodo.y2 > y2_max) {
            y2_max = nodo.y2;
        }
    }
}

bool searchRTree(char dir[], Rectangle* rectangleSearched, int* amountR, vector<Rectangle>* visiteds){
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
        
        int x1,y1,x2,y2;

        calcularLimites(nodos,x1,y1,x2,y2);
        Rectangle thisRectangle = {x1,y1,x2,y2};
        visiteds->push_back(thisRectangle);

        for(int i = 0; i < nodos.size(); i++){
            if (nodos[i].x1 <= rectangleSearched->x1 && nodos[i].y1 <= rectangleSearched->y1
            && nodos[i].x2 >= rectangleSearched->x2 && nodos[i].y2 >= rectangleSearched->y2){
                if (searchRTree(nodos[i].dir,rectangleSearched,amountR,visiteds)){
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

    int x1,y1,x2,y2;

    calcularLimites2(rectangles,x1,y1,x2,y2);
    Rectangle thisRectangle = {x1,y1,x2,y2};
    visiteds->push_back(thisRectangle);

    for(int i = 0; i < rectangles.size(); i++){
        if(rectangleSearched->isEqual(rectangles[i].x1,rectangles[i].y1,rectangles[i].x2,rectangles[i].y2)){
            return true;
        }
    }
    return false;
}

int func(vector<Rectangle>* rectangulosQ, int n){

    const char* comando = "sudo sh -c 'echo 1 > /proc/sys/vm/drop_caches'";

    vector<Rectangle> rectangles_1(*rectangulosQ);     
    int64_t  time1 = 0;
    int64_t  time2 = 0;
    int64_t  time3 = 0;

    int64_t  amountFiles1 = 0;
    int64_t  amountFiles2 = 0;
    int64_t  amountFiles3 = 0;

    ofstream archivo("experimentacion.txt", std::ios::app);

    if (!archivo.is_open()){
        cout << "error";
        return 1;
    }

    archivo << "valor de n:\t" << n << endl;
    for (int i = 0; i < 100; i++){
        system(comando);
        int indiceAleatorio = std::rand() % rectangles_1.size();
        Rectangle searched = rectangles_1[indiceAleatorio];

        vector<Rectangle> visited;

        auto inicio = high_resolution_clock::now();
        char direccion[] = "./dHil/R_Tree_Raiz.txt";
        int cantidadLecturas = 0;
        bool isFinded = searchRTree(direccion,&searched,&cantidadLecturas,&visited);
        auto fin = high_resolution_clock::now();

        auto tiempo_transcurrido = duration_cast<nanoseconds>(fin - inicio).count();
        time1 += tiempo_transcurrido;
        amountFiles1 += cantidadLecturas;

        archivo << "1\t" << tiempo_transcurrido << "\t" << cantidadLecturas << "\t" << (isFinded? 1: 0) << "\t" <<"{";
        for(int i = 0; i < visited.size(); i++){
            archivo << "{" << visited[i].x1 << ", " << visited[i].y1 << ", " << visited[i].x2 << ", " << visited[i].y2 << "},";
        }
        archivo <<  "}\n";

        system(comando);
        visited.clear();



        inicio = high_resolution_clock::now();
        strcpy(direccion,"./dNXC/raiz_NRX.txt");
        cantidadLecturas = 0;
        isFinded = searchRTree(direccion,&searched,&cantidadLecturas,&visited);
        fin = high_resolution_clock::now();

        tiempo_transcurrido = duration_cast<nanoseconds>(fin - inicio).count();
        time2 += tiempo_transcurrido;
        amountFiles2+= cantidadLecturas;

        archivo << "2\t" << tiempo_transcurrido << "\t" << cantidadLecturas << "\t" << (isFinded? 1: 0) << "\t" <<"{";

        for(int i = 0; i < visited.size(); i++){
            archivo << "{" << visited[i].x1 << ", " << visited[i].y1 << ", " << visited[i].x2 << ", " << visited[i].y2 << "},";
        }
        archivo <<  "}\n";

        system(comando);
        visited.clear();


        inicio = high_resolution_clock::now();
        strcpy(direccion,"./dSTR/raiz.txt");
        cantidadLecturas = 0;
        isFinded = searchRTree(direccion,&searched,&cantidadLecturas,&visited);
        fin = high_resolution_clock::now();

        tiempo_transcurrido = duration_cast<nanoseconds>(fin - inicio).count();
        time3 += tiempo_transcurrido;
        amountFiles3 += cantidadLecturas;
        archivo << "3\t" << tiempo_transcurrido << "\t" << cantidadLecturas << "\t" << (isFinded? 1: 0) << "\t" <<"{";
        for(int i = 0; i < visited.size(); i++){
            archivo << "{" << visited[i].x1 << ", " << visited[i].y1 << ", " << visited[i].x2 << ", " << visited[i].y2 << "},";
        }
        archivo <<  "}\n";

        system(comando);
    }

    archivo << "tiempo promedio 1:\t" << time1 /100.0 << endl;
    archivo << "tiempo promedio 2:\t" << time2 /100.0 << endl;
    archivo << "tiempo promedio 3:\t" << time3 /100.0 << endl;

    archivo.close();
    return 1;

}