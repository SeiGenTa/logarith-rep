#include <bits/stdc++.h>

// struct point
#include "Point.h"

using namespace std;

static bool debugMode = false;
static bool showState = true;

/*
will return the distance min betweem two point selected randomly, n times
(is possible the response not be correct)
*/
double distanceRandomFMR(vector<Point> &points, int length, int n, pair<Point, Point> &opti)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, length - 1);

    int num1 = dist(gen);
    int num2 = dist(gen);

    while (num1 == num2)
    {
        num2 = dist(gen);
    }

    Point *pont1 = &points[num1];
    Point *pont2 = &points[num2];

    double distanceMin = distanceBetweenTwoPoints(&points[num1], &points[num2]);
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
        double thisDistance = distanceBetweenTwoPoints(&points[num1], &points[num2]);
        if (debugMode)
            cout << "distance calculated: " << thisDistance << endl;

        if (thisDistance < distanceMin)
        {
            distanceMin = thisDistance;
            pont1 = &points[num1];
            pont2 = &points[num2];
        }

        if (debugMode)
            cout << "temp distance min:  " << distanceMin << endl
                 << "-------------------------------------------" << endl;
    }
    opti = {*pont1, *pont2};
    return distanceMin;
};

// build the grids and save
bool hashFMR(vector<Point> &points, int &amountPoints, int &l, int &hashSize, vector<vector<vector<Point *>>> &grid)
{

    for (int i = 0; i < amountPoints; i++)
    {
        Point *point = &points[i];
        int a = static_cast<int>(point->x * hashSize);
        int b = static_cast<int>(point->y * hashSize);

        long long resultadoX = static_cast<long long>(a) % hashSize;
        long long resultadoY = static_cast<long long>(b) % hashSize;

        //    int valX = point->x * gridSize - 1;
        //    int valY = point->y * gridSize - 1;
        grid[static_cast<int>(resultadoX)][static_cast<int>(resultadoY)].push_back(point);
    }

    return true;
};

// Function to get points in the specified area
vector<Point *> &getPointsInAreaFMR(vector<vector<vector<Point *>>> *grid, int i, int j, int heightWidthGrid)
{
    static vector<Point *> pointsInArea; // Static for optimization

    pointsInArea.clear();

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

    return pointsInArea;
}

// Function that found the pair points comparing between points in the nere grids
bool searchClosestPairInGridFMR(vector<vector<vector<Point *>>> *grid, pair<Point, Point> *min, int heightWidthGrid)
{
    double distanceMin = 1; //

    pair<Point *, Point *> pairPoints = {nullptr, nullptr};

    for (int i = 0; i < heightWidthGrid; i += 2)
    {
        for (int j = 0; j < heightWidthGrid; j += 2)
        {
            vector<Point *> &pointsInArea = getPointsInAreaFMR(grid, i, j, heightWidthGrid);
            // look for the pair point that is closest to each other
            for (int base = 0; base < pointsInArea.size(); base++)
            {
                for (int comp = base + 1; comp < pointsInArea.size(); comp++)
                {
                    double distance = distanceBetweenTwoPoints(pointsInArea[base], pointsInArea[comp]);

                    if (distance < distanceMin)
                    {
                        distanceMin = distance;
                        pairPoints = {pointsInArea[base], pointsInArea[comp]};
                    }
                    if (distance == 0)
                    {
                        if (pairPoints.first && pairPoints.second) // Verificar si los punteros son diferentes de nullptr
                        {
                            *min = {*(pairPoints.first), *(pairPoints.second)};
                            return true;
                        }
                        else
                        {
                            // Manejar el caso en el que al menos uno de los punteros es nullptr
                            return false;
                        }
                    }
                }
            }
        }
    }

    *min = {*(pairPoints.first), *(pairPoints.second)};
    return true;
}

// Function that searches for the value >= than value in the vector
// Returns the result and modifies the index through a reference
int searchBinary(const std::vector<int> &vector, int value, int &index)
{
    int left = 0;
    int right = vector.size() - 1;
    int result = -1;
    index = -1;

    while (left <= right)
    {
        int medio = left + (right - left) / 2;

        if (vector[medio] >= value)
        {
            result = vector[medio];
            index = medio;
            right = medio - 1;
        }
        else
        {
            left = medio + 1;
        }
    }

    return result;
}

/*
Function that found the pair of points that its distance is the min
*/
pair<Point, Point> closestPairRandomFMR(vector<Point> points, vector<vector<vector<Point *>>> &matriz)
{
    srand(time(NULL));
    int lengthVPoints = points.size();

    pair<Point, Point> thisMin;
    thisMin.first = {0, 0};
    thisMin.second = {0, 0};

    if (showState || debugMode)
        cout << "Searching the min value" << endl;
    ;

    double d = distanceRandomFMR(points, lengthVPoints, lengthVPoints, thisMin);
    if (debugMode)
        cout << "value min find: " << d << endl;

    cout << "size of d: " << d << endl;

    if (d == 0)
        return thisMin;

    if (d < 0.0001)
        d = 0.0001;

    int amountHeight = 0;
    double sizeNow = 0;
    if (debugMode || showState)
        cout << "calculated size of each grid" << endl;
    while (sizeNow < 1)
    {
        sizeNow += d;
        amountHeight++;
    };

    int l;

    vector<int> powOf2 = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384};
    int sizeGrill = searchBinary(powOf2, amountHeight, l);

    cout << "valor de tamaño actual: " << sizeGrill << endl;

    if (debugMode || showState)
        cout << "building the grid" << endl;
    hashFMR(points, lengthVPoints, l, sizeGrill, matriz);

    if (debugMode || showState)
        cout << "searching the min point" << endl;
    searchClosestPairInGridFMR(&matriz, &thisMin,sizeGrill);

    return thisMin;
}