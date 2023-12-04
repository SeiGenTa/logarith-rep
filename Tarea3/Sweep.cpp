#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <limits>

#include "Point.h"

// Función para calcular la distancia euclidiana entre dos puntos
double calcularDistancia(const Point &p1, const Point &p2)
{
    return std::sqrt(std::pow(p2.x - p1.x, 2) + std::pow(p2.y - p1.y, 2));
}

// Función de comparación para ordenar puntos por coordenada x
bool compararPorX(const Point &p1, const Point &p2)
{
    return p1.x < p2.x;
}

// Función para encontrar el par más cercano utilizando el algoritmo de barrido
std::pair<Point, Point> encontrarParMasCercanoSweep(const std::vector<Point> &puntos)
{
    if (puntos.size() < 2)
    {
        // No hay suficientes puntos para encontrar un par cercano
        return {{0.0, 0.0}, {0.0, 0.0}};
    }

    // Ordenar los puntos por coordenada x
    std::vector<Point> puntosOrdenados = puntos;
    std::sort(puntosOrdenados.begin(), puntosOrdenados.end(), compararPorX);

    // Inicializar la distancia mínima
    double distanciaMinima = std::numeric_limits<double>::max();
    std::pair<Point, Point> parMasCercano = {{0.0, 0.0}, {0.0, 0.0}};

    // Índices de los puntos más cercanos en la lista ordenada
    size_t indicePunto1 = 0;
    size_t indicePunto2 = 0;

    // Algoritmo de barrido
    for (size_t i = 0; i < puntosOrdenados.size(); ++i)
    {
        for (size_t j = i + 1; j < puntosOrdenados.size() && puntosOrdenados[j].x - puntosOrdenados[i].x < distanciaMinima; ++j)
        {
            double distancia = calcularDistancia(puntosOrdenados[i], puntosOrdenados[j]);
            if (distancia < distanciaMinima)
            {
                distanciaMinima = distancia;
                parMasCercano = {puntosOrdenados[i], puntosOrdenados[j]};
                indicePunto1 = i;
                indicePunto2 = j;
            }
        }
    }

    // Verificar si hay puntos más cercanos en la franja central
    for (size_t i = indicePunto1; i < indicePunto2; ++i)
    {
        for (size_t j = i + 1; j < indicePunto2; ++j)
        {
            double distancia = calcularDistancia(puntosOrdenados[i], puntosOrdenados[j]);
            if (distancia < distanciaMinima)
            {
                distanciaMinima = distancia;
                parMasCercano = {puntosOrdenados[i], puntosOrdenados[j]};
            }
        }
    }

    return parMasCercano;
}
