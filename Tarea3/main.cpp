#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <limits>
#include "Point.h"
#include "Divide.h"
#include "Random.h"
using namespace std;

/// TESTER
// Función para encontrar el par más cercano entre un conjunto de puntos
std::pair<Point, Point> encontrarParMasCercano(const std::vector<Point> &puntos)
{
    if (puntos.size() < 2)
    {
        // No hay suficientes puntos para encontrar un par cercano
        return {{0.0, 0.0}, {0.0, 0.0}};
    }

    double distanciaMinima = std::numeric_limits<double>::max();
    std::pair<Point, Point> parMasCercano = {{0.0, 0.0}, {0.0, 0.0}};

    // Iterar sobre todos los pares de puntos y encontrar la distancia mínima
    for (size_t i = 0; i < puntos.size(); ++i)
    {
        for (size_t j = i + 1; j < puntos.size(); ++j)
        {
            double distancia = euclideanDistance(puntos[i], puntos[j]);
            if (distancia < distanciaMinima)
            {
                distanciaMinima = distancia;
                parMasCercano = {puntos[i], puntos[j]};
            }
        }
    }

    return parMasCercano;
}

int main()
{
    srand(time(NULL));
    vector<Point> points;
    for (int i = 0; i < 200; ++i)
    {
        Point punto;
        punto.x = static_cast<float>(rand()) / RAND_MAX; // Valor aleatorio en [0, 1)
        punto.y = static_cast<float>(rand()) / RAND_MAX; // Valor aleatorio en [0, 1)
        points.push_back(punto);
    }
    pair<Point, Point> realClosest = encontrarParMasCercano(points);
    cout << "real distance: " << euclideanDistance(realClosest.first, realClosest.second)
         << ", the points is  (" << realClosest.second.x << " , " << realClosest.second.x << ") (" << realClosest.first.x
         << " , " << realClosest.first.x << ") \n";

    pair<Point, Point> parDeterminite = closestPair(points);

    cout << "distance calculated in determinite: " << euclideanDistance(parDeterminite.first, parDeterminite.second)
         << ", the points is  (" << parDeterminite.second.x << " , " << parDeterminite.second.x << ") (" << parDeterminite.first.x
         << " , " << parDeterminite.first.x << ") \n";

    pair<Point, Point> parRandom = closestPairRandom(points);

    cout << "distance calculated in random: " << euclideanDistance(parRandom.first, parRandom.second) << ", the points is  ("
         << parRandom.second.x << " , " << parRandom.second.x << ") (" << parRandom.first.x << " , " << parRandom.first.x << ") \n";
}