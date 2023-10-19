#include<bits/stdc++.h> 
using namespace std;


struct Rectangle{
    int x1, y1, x2, y2;
};

int main(){
    int x;
    cin >> x;

    ofstream archivo("rectangulo.txt", std::ios::app);

    if (!archivo.is_open()){
        cout << "error";
    }


    archivo << "{\n";
    for (int i = 0; i < x; ++i) {
        int x1 = std::rand() % (500000 - 20);  // Garantiza una diferencia máxima de 20
        int y1 = std::rand() % (500000 - 20);
        int x2 = x1 + (std::rand() % 21);      // Diferencia máxima de 20
        int y2 = y1 + (std::rand() % 21);
        archivo << "\t" << "{" << x1 << ","
        << y1 << ","
        << x2 << ","
        << y2 << "},\n";
    }
    archivo << "} \n";

    archivo.close();
    
    return 0;

}