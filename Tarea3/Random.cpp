#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <limits>
#include <stdlib.h>
#include <time.h>

// struct point
#include "Point.h"

using namespace std;

static bool debugMode = false;
/*
calculate the distance between two points
*/
float distanceBetweenTwoPoints(Point *a, Point *b)
{
    return sqrt(pow(a->x - b->x, 2) + (pow(a->y - b->y, 2)));
}

/*
will return the distance min betweem two point selected randomly, n times
(is possible the response not be correct)
*/
float distanceRandom(vector<Point> &points, int length, int n)
{
    int num1 = rand() % length;
    int num2 = rand() % length;
    while (num1 == num2)
    {
        num1 = rand() % length;
        num2 = rand() % length;
    }

    float distanceMin = distanceBetweenTwoPoints(&points[num1], &points[num2]);
    if (debugMode)
        cout << "initial distance min:  " << distanceMin << endl
             << "-------------------------------------------" << endl;

    for (int i = 1; i < n; i++)
    {
        num1 = rand() % length;
        num2 = rand() % length;
        while (num1 == num2)
        {
            num1 = rand() % length;
            num2 = rand() % length;
        }
        float thisDistance = distanceBetweenTwoPoints(&points[num1], &points[num2]);
        if (debugMode)
            cout << "distance calculated: " << thisDistance << endl;

        if (thisDistance < distanceMin)
        {
            distanceMin = thisDistance;
        }

        if (debugMode)
            cout << "temp distance min:  " << distanceMin << endl
                 << "-------------------------------------------" << endl;
    }
    return distanceMin;
}

// build the grids and save
bool buildGrids(vector<Point> *points, int &amountPoints, float &distance, vector<vector<vector<Point *>>> *grid)
{
    for (int i = 0; i < amountPoints; i++)
    {
        // este es el hash, es un hash realmente sencillo, pero coloca todo con coste O(0)
        // y los coloca todo de manera correcta
        Point *thisPoint = &(*points)[i]; // entrada del hash
        int ubqX = thisPoint->x / distance;
        int ubqY = thisPoint->y / distance;
        // este par de datos (la posicion) es nuestro hash

        (*grid)[ubqX][ubqY].push_back(thisPoint);
    }
    return true;
}

void showElementInGrid(vector<vector<vector<Point *>>> &grid, float d)
{
    cout << "-------------------------------------------" << endl
         << "show the element in each grid" << endl;
    for (int y = 0; y < grid.size(); ++y)
    {
        for (int x = 0; x < grid[y].size(); ++x)
        {
            const std::vector<Point *> *puntosEnCasilla = &(grid[x][y]);

            // Verificar si hay puntos en esta casilla
            if (puntosEnCasilla->empty())
            {
                continue; // No hay puntos en esta casilla, pasar a la siguiente
            }

            // Mostrar todos los puntos en la casilla
            std::cout << "Puntos en la casilla (" << x * d << ", " << y * d << "), hasta: (" << (x + 1) * d << ", " << (y + 1) * d << ")" << std::endl;
            for (int i = 0; i < (*puntosEnCasilla).size(); i++)
            {
                Point *punto = (*puntosEnCasilla)[i];
                std::cout << "(" << punto->x << ", " << punto->y << ")" << std::endl;
            }
            cout << "-------------------------------------------" << endl;
        }
    }
}

// Function that found the pair points comparing between points in the nere grids
bool searchClosestPairInGrid(vector<vector<vector<Point *>>> *grid, pair<Point, Point> *min)
{
    float distanceMin = 2; //

    pair<Point*,Point*> pairPoints;

    int heightWidthGrid = grid->size();
    for (int i = 0; i < heightWidthGrid; i += 2)
    {
        for (int j = 0; j < heightWidthGrid; j += 2)
        {
            vector<Point *> pointsInArea;

            //Put the points pointers in the vector (the 9 closest grid)
            for (int nx = i - 1; nx <= i + 1; nx++)
            {
                if (nx < 0 || nx >= heightWidthGrid)
                    continue;
                for (int ny = j - 1; ny <= j + 1; ny++)
                {
                    if (ny < 0 || ny >= heightWidthGrid)
                        continue;
                    for (Point *point : (*grid)[nx][ny])
                    {
                        pointsInArea.push_back(point);
                    }
                }
            }

            //look for the pair point that is closest to each other
            for(int base = 0; base < pointsInArea.size(); base ++){
                for (int comp = base + 1; comp < pointsInArea.size(); comp++){
                    float distance = distanceBetweenTwoPoints(pointsInArea[base],pointsInArea[comp]);
                    if (distance < distanceMin){
                        distanceMin = distance;
                        pairPoints = {pointsInArea[base],pointsInArea[comp]};
                    }
                }
            }
        }
    }

    *min = {*pairPoints.first,*pairPoints.second};

    return true;
}

/*
Function that found the pair of points that its distance is the min
*/
pair<Point, Point> closestPairRandom(vector<Point> points)
{
    srand(time(NULL));
    int lengthVPoints = points.size();

    if (debugMode)
        cout << "Searching the min value" << endl;
    ;
    float d = distanceRandom(points, lengthVPoints, lengthVPoints);
    if (debugMode)
        cout << "value min find: " << d << endl;

    int amountHeight = 0;
    float sizeNow = 0;
    if (debugMode)
        cout << "calculated size of each grid" << endl;
    while (sizeNow < 1)
    {
        sizeNow += d;
        amountHeight++;
    }

    if (debugMode)
        cout << "building the grid" << endl;
    vector<vector<vector<Point *>>> grid(amountHeight, vector<vector<Point *>>(amountHeight));

    buildGrids(&points, lengthVPoints, d, &grid);

    if (debugMode)
        showElementInGrid(grid, d);

    pair<Point, Point> thisMin;
    thisMin.first = {0, 0};
    searchClosestPairInGrid(&grid, &thisMin);

    return thisMin;
}