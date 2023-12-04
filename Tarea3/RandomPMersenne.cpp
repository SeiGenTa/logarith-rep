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
double distanceRandomPmersenne(vector<Point> &points, int length, int n, pair<Point, Point> &opti)
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
bool hashPMersenne(vector<Point> &points, int &amountPoints, int &gridSize, vector<vector<vector<Point *>>> &grid)
{
    // AQUI SE DEBE PROGRAMAR EL HASH DE PRIMOS DE MERSENNE
    //for (int i = 0; i < amountPoints; i++)
    //{
    //    Point *point = &points[i];
    //    int valX = point->x * gridSize - 1;
    //    int valY = point->y * gridSize - 1;
    //    grid[valX][valY].push_back(point);
    //}
    return true;
};

// Function to get points in the specified area
vector<Point *> &getPointsInAreaPMersenne(vector<vector<vector<Point *>>> *grid, int i, int j, int heightWidthGrid)
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
bool searchClosestPairInGridPMersenne(vector<vector<vector<Point *>>> *grid, pair<Point, Point> *min)
{
    double distanceMin = 1; //

    pair<Point *, Point *> pairPoints = {nullptr, nullptr};

    int heightWidthGrid = grid->size();
    for (int i = 0; i < heightWidthGrid; i += 2)
    {
        for (int j = 0; j < heightWidthGrid; j += 2)
        {
            vector<Point *> &pointsInArea = getPointsInAreaPMersenne(grid, i, j, heightWidthGrid);

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
                        *min = {*(pairPoints.first), *(pairPoints.second)};
                        return true;
                    }
                }
            }
        }
    }

    *min = {*(pairPoints.first), *(pairPoints.second)};
    return true;
}

/*
Function that found the pair of points that its distance is the min
*/
pair<Point, Point> closestPairRandomPMersenne(vector<Point> points)
{
    srand(time(NULL));
    int lengthVPoints = points.size();

    pair<Point, Point> thisMin;
    thisMin.first = {0, 0};
    thisMin.second = {0, 0};

    if (showState || debugMode)
        cout << "Searching the min value" << endl;
    ;

    double d = distanceRandomPmersenne(points, lengthVPoints, lengthVPoints, thisMin);
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
    cout << "valor de tamaño actual: " << amountHeight << endl;

    vector<vector<vector<Point *>>> matriz(amountHeight, vector<vector<Point *>>(amountHeight));

    if (debugMode || showState)
        cout << "building the grid" << endl;
    hashPMersenne(points, lengthVPoints, amountHeight, matriz);

    if (debugMode || showState)
        cout << "searching the min point" << endl;
    searchClosestPairInGridPMersenne(&matriz, &thisMin);

    return thisMin;
}