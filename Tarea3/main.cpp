#include <bits/stdc++.h>
#include <thread>
#include <mutex>
#include "Point.h"
#include "Sweep.h"
#include "Random.h"
using namespace std;
using namespace std::chrono;

std::mutex mtk;
std::mutex mtxResults;

int dispMem = 0;
int hilosCreados = 0;

/// TESTER
// Función para encontrar el par más cercano entre un conjunto de puntos
std::pair<Point, Point> encontrarParMasCercano(const std::vector<Point> &puntos)
{
    if (puntos.size() < 2)
    {
        // No hay suficientes puntos para encontrar un par cercano
        return {{0.0, 0.0}, {0.0, 0.0}};
    }

    float distanciaMinima = std::numeric_limits<double>::max();
    std::pair<Point, Point> parMasCercano = {{0.0, 0.0}, {0.0, 0.0}};

    // Iterar sobre todos los pares de puntos y encontrar la distancia mínima
    for (size_t i = 0; i < puntos.size(); ++i)
    {
        for (size_t j = i + 1; j < puntos.size(); ++j)
        {
            float distancia = distanceBetweenTwoPoints(&puntos[i], &puntos[j]);
            if (distancia < distanciaMinima)
            {
                distanciaMinima = distancia;
                parMasCercano = {puntos[i], puntos[j]};
            }
        }
    }

    return parMasCercano;
}

// Función que realiza la ejecución en paralelo
int threadComplete(vector<Point> points, int n, const std::string &nameFileResult)
{
    vector<Point>
        thisPoints(points);
    //cout << "iniciando sweep" << endl;
    //auto inicio = std::chrono::high_resolution_clock::now();
    //pair<Point, Point> parDeterminite = encontrarParMasCercanoSweep(thisPoints);
    //auto fin = std::chrono::high_resolution_clock::now();
//
    //std::chrono::duration<double> duracion1 = fin - inicio;

    cout << "iniciando random" << endl;
    auto inicio = std::chrono::high_resolution_clock::now();
    pair<Point, Point> parRandom = closestPairRandom(thisPoints);
    auto fin = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duracion2 = fin - inicio;

    //cout << "distance calculated in determinite: " << distanceBetweenTwoPoints(&parDeterminite.first, &parDeterminite.second)
    //     << ", the points is  (" << parDeterminite.second.x << " , " << parDeterminite.second.x << ") (" << parDeterminite.first.x
    //     << " , " << parDeterminite.first.x << ") \n";

    cout << "distance calculated in random: " << distanceBetweenTwoPoints(&parRandom.first, &parRandom.second) << ", the points is  ("
         << parRandom.second.x << " , " << parRandom.second.x << ") (" << parRandom.first.x << " , " << parRandom.first.x << ") \n";

    {
        lock_guard<mutex> lock(mtxResults);
        ofstream archivo(nameFileResult, std::ios::app);
        if (archivo.is_open())
        {
            archivo << n << "\t" << duracion2.count() << endl;
                    //<< "\t " << duracion1.count() << endl;
            archivo.close();
        }
        else
        {
            cout << "a ocurrido un error" << endl;
            return 1;
        }
    }

    thisPoints.clear(); // Eliminar todos los elementos del vector
    std::vector<Point>().swap(thisPoints);

    hilosCreados--;
    return 0;
}

int main()
{
    srand(time(NULL));

    int amountRep = 25;
    int hilosPermitidos = 1;

    const char *nameFileResult = "resultados.txt";

    ofstream archivo(nameFileResult, std::ios::out);
    if (archivo.is_open())
    {
        archivo << "tamaño del vector \t tiempo tardado random \t tiempo tardado Sweep" << endl;
        archivo.close();
    }
    else
    {
        cout << "a ocurrido un error" << endl;
        return 1;
    }

    for (int n = 50000000; n <= 50000000; n += 5000000)
    {
        vector<Point> points;
        for (int i = 0; i < n; ++i)
        {
            Point punto;
            punto.x = static_cast<float>(rand()) / RAND_MAX; // Valor aleatorio en [0, 1)
            punto.y = static_cast<float>(rand()) / RAND_MAX; // Valor aleatorio en [0, 1)
            points.push_back(punto);
        }
        vector<thread> hilos;
        for (int j = 0; j < amountRep; j++)
        {
            lock_guard<mutex> lock(mtk);
            if (hilosCreados < hilosPermitidos)
            {
                hilosCreados++; // Incrementa el contador de hilos
                thread hilo(threadComplete, ref(points), ref(n), nameFileResult);
                hilos.push_back(std::move(hilo));
            }
            else
            {
                this_thread::sleep_for(chrono::seconds(1)); // Espera si el límite se alcanzó
                j--;
            }
        }
        for (thread &hilo : hilos)
        {
            hilo.join();
        }
    }
}