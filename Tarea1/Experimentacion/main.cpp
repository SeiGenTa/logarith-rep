#include<bits/stdc++.h> 
namespace fs = std::filesystem;
using namespace std;


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

};

#include "generadorRectangulos.h"
#include "STR.h"
#include "NXC.h"
#include "GenerateRTree.h"
#include "BusquedaR-Tree.h"



int main(){
    for(int i = 10; i <= 15; i++){
        vector<Rectangle> rectangulosQ;
        vector<Rectangle> rectangulosR;
        cout << "generando los rectangulos \n";
        generateRectangulos(&rectangulosQ,&rectangulosR,i);
        cout << "generando el arbol STR \n";
        generateSTR(&rectangulosR, true, 80);
        cout << "generando el arbol NearestX \n";
        generateNearestX(&rectangulosR, true, 80);
        cout << "generando el arbol de Hilbert \n";
        generateHilbert(&rectangulosR, true, 80);
        cout << "inicia busqueda \n";
        func(&rectangulosR,i);
    }

    return 1;
}