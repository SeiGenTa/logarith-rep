#include <bits/stdc++.h>

// struct point
#include "Point.h"

using namespace std;

static bool debugMode = false;
static bool showState = true;

struct MyKey
{
    int first;
    int second;

    bool operator==(const MyKey &other) const
    {
        return std::tie(first, second) == std::tie(other.first, other.second);
    }
};

namespace std
{
    template <>
    struct hash<MyKey>
    {
        std::size_t operator()(const MyKey &k) const
        {
            return hash<int>()(k.first) ^ hash<int>()(k.second);
        }
    };
}

typedef unordered_map<pair<int, int>, vector<Point *>> HashGrid;
/*
calculate the distance between two points
*/
float distanceBetweenTwoPoints(const Point *a, const Point *b)
{
    return sqrt(pow(a->x - b->x, 2) + (pow(a->y - b->y, 2)));
}

/*
will return the distance min betweem two point selected randomly, n times
(is possible the response not be correct)
*/
float distanceRandom(vector<Point> &points, int length, int n, pair<Point, Point> &opti)
{
    int num1 = rand() % length;
    int num2 = rand() % length;
    Point *pont1;
    Point *pont2;
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
            pont1 = &points[num1];
            pont2 = &points[num2];
        }

        if (debugMode)
            cout << "temp distance min:  " << distanceMin << endl
                 << "-------------------------------------------" << endl;
    }
    opti = {*pont1,*pont2};
    return distanceMin;
};

// build the grids and save
bool buildGrids(vector<Point> *points, int &amountPoints, int &gridSize, unordered_map<MyKey, vector<Point *>> *grid)
{

    for (int i = 0; i < amountPoints; i++)
    {
        // este es el hash, es un hash realmente sencillo, pero coloca todo con coste O(0)
        // y los coloca todo de manera correcta
        Point *thisPoint = &(*points)[i]; // entrada del hash
        int hashedX = static_cast<int>(std::round(thisPoint->x * gridSize));
        int hashedY = static_cast<int>(std::round(thisPoint->y * gridSize));
        // este par de datos (la posicion) es nuestro hash
        auto key = make_pair(hashedX, hashedY);

        (*grid)[{hashedX, hashedY}].push_back(new Point{*thisPoint});
    }
    return true;
};

// Function that found the pair points comparing between points in the nere grids
bool searchClosestPairInGrid(vector<vector<vector<Point *>>> *grid, pair<Point, Point> *min)
{
    float distanceMin = 2; //

    pair<Point *, Point *> pairPoints;

    int heightWidthGrid = grid->size();
    for (int i = 0; i < heightWidthGrid; i += 2)
    {
        for (int j = 0; j < heightWidthGrid; j += 2)
        {
            vector<Point *> pointsInArea;

            // Put the points pointers in the vector (the 9 closest grid)
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

            // look for the pair point that is closest to each other
            for (int base = 0; base < pointsInArea.size(); base++)
            {
                for (int comp = base + 1; comp < pointsInArea.size(); comp++)
                {
                    float distance = distanceBetweenTwoPoints(pointsInArea[base], pointsInArea[comp]);
                    if (distance < distanceMin)
                    {
                        distanceMin = distance;
                        pairPoints = {pointsInArea[base], pointsInArea[comp]};
                    }
                }
            }
        }
    }

    *min = {*pairPoints.first, *pairPoints.second};

    return true;
}

/*
Function that found the pair of points that its distance is the min
*/
pair<Point, Point> closestPairRandom(vector<Point> points)
{
    srand(time(NULL));
    int lengthVPoints = points.size();

    pair<Point, Point> thisMin;
    thisMin.first = {0, 0};
    thisMin.second = {0, 0};

    if (showState || debugMode)
        cout << "Searching the min value" << endl;
    ;

    float d = distanceRandom(points, lengthVPoints, lengthVPoints, thisMin);
    if (debugMode)
        cout << "value min find: " << d << endl;

    if (d == 0)
        return thisMin;

    if (d < 0.0003)
        d = 0.0003; // Por seguridad de mi memoria

    int amountHeight = 0;
    float sizeNow = 0;
    if (debugMode || showState)
        cout << "calculated size of each grid" << endl;
    while (sizeNow < 1)
    {
        sizeNow += d;
        amountHeight++;
    };

    unordered_map<MyKey, vector<Point *>> gridHash;

    if (debugMode || showState)
        cout << "building the grid" << endl;
    try
    {
        buildGrids(&points, lengthVPoints, amountHeight, &gridHash);
    }
    catch (std::exception &e)
    {
        cout << e.what() << endl;
        cout << "error en: " << d << endl;
        exit(1);
    }

    // searchClosestPairInGrid(&grid, &thisMin);

    return thisMin;
}