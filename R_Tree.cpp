#include <iostream>
#include <bits/stdc++.h>
#include <vector>

using namespace std;

//Estructura Rectángulo
struct Rectangulo{
    double x1 , y1 , x2 , y2;
};

//Estructura Nodo que posee rectangulo con sus hijos
struct Nodo{
    Rectangulo MBR;
    vector<Rectangulo> Rectangulos;
    vector<Nodo*> Hijos;
};

vector<Nodo>&nearest_recursivo(vector<Nodo>&nodos,int m){
    // Hasta aqui esta todo ordenado
    int maxi = ceil(nodos.size()/m);
    vector<Nodo>*Nodos = new vector<Nodo>;
    //vector<Nodo>Nodos(maxi);
    for(int i=0; i<maxi;i++){
        vector<Rectangulo>*rec = new vector<Rectangulo>;
        for(int j=i*m;j<min((int)nodos.size(),(i+1)*m);j++){
            rec->push_back(nodos[j].MBR);
        }
        double xmax, xmin, ymax, ymin;

        for(int k=0;k<rec->size();k++){
            xmax = max(xmax,(*rec)[k].x1);
            xmax = max(xmax,(*rec)[k].x2);
            ymax = max(ymax,(*rec)[k].y1);
            ymax = max(ymax,(*rec)[k].y2);
            xmin = min(xmin,(*rec)[k].x1);
            xmin = min(xmin,(*rec)[k].x2);
            ymin = min(ymin,(*rec)[k].y1);
            ymin = min(ymin,(*rec)[k].y2);
        }
        Rectangulo mbr = {xmin,ymin,xmax,ymax};
        vector<Nodo*>*hijos = new vector<Nodo*>;
        for(int l=i*m;l<min((int)nodos.size(),(i+1)*m);l++){
            hijos->push_back(&nodos[l]);
        }
        Nodo hoja = {mbr,*rec,*hijos};
        Nodos->push_back(hoja);
    }

    // Caso base
    if(maxi == 1){
        return *Nodos ;
    }
    else{
        return nearest_recursivo(*Nodos,m);
    }

}


// m cantidad maxima de hijos por nodo
int nearestx(vector<Rectangulo>&Rectangulos,int m){

    sort(Rectangulos.begin(), Rectangulos.end(), [](const Rectangulo& a, const Rectangulo& b) {
        return (a.x1 + a.x2) / 2 < (b.x1 + b.x2) / 2;
    });
   
    for(int i = 0 ; i<Rectangulos.size();i++){
        cout << Rectangulos[i].x1 << " " ; 
    }

    // Hasta aqui esta todo ordenado
    int maxi = ceil(Rectangulos.size()/m);
    vector<Nodo>*Nodos = new vector<Nodo>;
    for(int i=0; i<maxi;i++){
        vector<Rectangulo>*rec = new vector<Rectangulo>;
        for(int j=i*m;j<min((int)Rectangulos.size(),(i+1)*m);j++){
            rec->push_back(Rectangulos[j]);
        }
        double xmax, xmin, ymax, ymin; 
        
        for(int k=0;k<rec->size();k++){
            xmax = max(xmax,(*rec)[k].x1);
            xmax = max(xmax,(*rec)[k].x2);
            ymax = max(ymax,(*rec)[k].y1);
            ymax = max(ymax,(*rec)[k].y2);
            xmin = min(xmin,(*rec)[k].x1);
            xmin = min(xmin,(*rec)[k].x2);
            ymin = min(ymin,(*rec)[k].y1);
            ymin = min(ymin,(*rec)[k].y2);
        }
        Rectangulo mbr = {xmin,ymin,xmax,ymax};
        vector<Nodo*>*nulo = new vector<Nodo*>;
        Nodo hoja = {mbr,*rec,*nulo};
        Nodos->push_back(hoja);
    }
    nearest_recursivo(*Nodos,m);
    return 0;
} 
