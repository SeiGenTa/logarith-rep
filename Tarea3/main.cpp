#include <bits/stdc++.h>
#include <thread>
#include <mutex>
#include "Point.h"
#include "Sweep.h"
#include "RandomUniversal.h"
#include "RandomFMR.h"
#include "RandomPMersenne.h"
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

    double distanciaMinima = std::numeric_limits<double>::max();
    std::pair<Point, Point> parMasCercano = {{0.0, 0.0}, {0.0, 0.0}};

    // Iterar sobre todos los pares de puntos y encontrar la distancia mínima
    for (size_t i = 0; i < puntos.size(); ++i)
    {
        for (size_t j = i + 1; j < puntos.size(); ++j)
        {
            double distancia = distanceBetweenTwoPoints(&puntos[i], &puntos[j]);
            if (distancia < distanciaMinima)
            {
                distanciaMinima = distancia;
                parMasCercano = {puntos[i], puntos[j]};
            }
        }
    }

    return parMasCercano;
}

// Función para resetear la matriz
void resetearMatriz(std::vector<std::vector<std::vector<Point *>>> &matriz)
{
    for (auto &nivel1 : matriz)
    {
        for (auto &nivel2 : nivel1)
        {
            nivel2.clear();
        }
    }
}

// Función que realiza la ejecución en paralelo
int threadComplete(vector<Point> &points, int n, const std::string &nameFileResult)
{
    cout << "making the matrix for randoms" << endl;
    vector<vector<vector<Point *>>> matriz(16384, vector<vector<Point *>>(16384));

    vector<Point>
        thisPoints(points);
    cout << "iniciando sweep" << endl;
    auto inicio = std::chrono::high_resolution_clock::now();
    pair<Point, Point> parDeterminite = encontrarParMasCercanoSweep(thisPoints);
    auto fin = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracion1 = fin - inicio;

    resetearMatriz(matriz);

    cout << "iniciando random Universal" << endl;
    inicio = std::chrono::high_resolution_clock::now();
    pair<Point, Point> parRandom = closestPairRandomUniversal(thisPoints, matriz);
    fin = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracion2 = fin - inicio;

    cout << "finished universal" << endl;
    resetearMatriz(matriz);

    cout << "iniciando random FMR" << endl;
    inicio = std::chrono::high_resolution_clock::now();
    pair<Point, Point> parRandomFMR = closestPairRandomFMR(thisPoints, matriz);
    fin = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracion3 = fin - inicio;

    cout << "finished FMR" << endl;
    resetearMatriz(matriz);

    //cout << "iniciando random P Mersene" << endl;
    //inicio = std::chrono::high_resolution_clock::now();
    //pair<Point, Point> parRandomPMersenne = closestPairRandomPMersenne(thisPoints, matriz);
    //fin = std::chrono::high_resolution_clock::now();
    //std::chrono::duration<double> duracion4 = fin - inicio;
    //cout << "finished P Mersene" << endl;
//
    //resetearMatriz(matriz);

    cout << "distance calculated in determinite: " << distanceBetweenTwoPoints(&parDeterminite.first, &parDeterminite.second)
         << ", the points is  (" << parDeterminite.second.x << " , " << parDeterminite.second.x << ") (" << parDeterminite.first.x
         << " , " << parDeterminite.first.x << ") \n";

    cout << "distance calculated in random universal: " << distanceBetweenTwoPoints(&parRandom.first, &parRandom.second) << ", the points is  ("
         << parRandom.second.x << " , " << parRandom.second.x << ") (" << parRandom.first.x << " , " << parRandom.first.x << ") \n";

    cout << "distance calculated in random FMR: " << distanceBetweenTwoPoints(&parRandomFMR.first, &parRandomFMR.second) << ", the points is  ("
         << parRandomFMR.second.x << " , " << parRandomFMR.second.x << ") (" << parRandomFMR.first.x << " , " << parRandomFMR.first.x << ") \n";

    //cout << "distance calculated in random P Mersene : " << distanceBetweenTwoPoints(&parRandomPMersenne.first, &parRandomPMersenne.second) << ", the points is  ("
    //     << parRandomPMersenne.second.x << " , " << parRandomPMersenne.second.x << ") (" << parRandomPMersenne.first.x << " , " << parRandomPMersenne.first.x << ") \n";

    {
        lock_guard<mutex> lock(mtxResults);
        ofstream archivo(nameFileResult, std::ios::app);
        if (archivo.is_open())
        {
            archivo << n
                    << "\t" << duracion1.count()
                    << "\t" << duracion2.count()
                    << "\t" << duracion3.count()
                    //<< "\t" << duracion4.count()
                    << endl;
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
        archivo << "tamaño del vector \t tiempo tardado Sweep \t tiempo tardado random Universal \t tiempo tardado random FMR \t tiempo tardado random P Mersenne " << endl;
        archivo.close();
    }
    else
    {
        cout << "a ocurrido un error" << endl;
        return 1;
    }

    for (int n = 5000000; n <= 50000000; n += 5000000)
    {
        vector<Point> points;
        for (int i = 0; i < n; ++i)
        {
            Point punto;
            punto.x = static_cast<double>(rand()) / RAND_MAX; // Valor aleatorio en [0, 1)
            punto.y = static_cast<double>(rand()) / RAND_MAX; // Valor aleatorio en [0, 1)
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