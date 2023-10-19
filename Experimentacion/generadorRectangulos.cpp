#include<bits/stdc++.h> 
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

int generateRectangulos(vector<Rectangle>* rectangulosQ,vector<Rectangle>* rectangulosR, int q){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribucionCoord(0, 500000);

    int n = pow(2,q);

    // Generar conjunto R con tamaños de lados uniformemente distribuidos entre [0, 100] y valores aleatorios entre [0, 500000]
    for (int j = 0; j < n; ++j)
    {
        int x1 = distribucionCoord(gen);
        int y1 = distribucionCoord(gen);
        int x2 = x1 + std::uniform_int_distribution<>(0, 100)(gen);
        int y2 = y1 + std::uniform_int_distribution<>(0, 100)(gen);
        rectangulosR->push_back({x1,y1,x2,y2});
    }
    // Generar conjunto Q con tamaños de lados uniformemente distribuidos entre [0, 100000] y valores aleatorios entre [0, 500000]
    for (int j = 0; j < 100; ++j)
    {
        int x1 = distribucionCoord(gen);
        int y1 = distribucionCoord(gen);
        int x2 = x1 + std::uniform_int_distribution<>(0, 100000)(gen);
        int y2 = y1 + std::uniform_int_distribution<>(0, 100000)(gen);
        rectangulosQ->push_back({x1, y1, x2, y2});
    }

    return 0;
}