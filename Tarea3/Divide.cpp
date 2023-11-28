#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <limits>

using namespace std;

struct Point {
    float x, y;
};

bool compareX(const Point& a, const Point& b) {
    return a.x < b.x;
}

bool compareY(const Point& a, const Point& b) {
    return a.y < b.y;
}

float euclideanDistance(const Point& p1, const Point& p2) {
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

pair<Point, Point> closestPairUtil(const vector<Point>& points, int start, int end) {
    if (end - start <= 3) {
        // Si hay pocos puntos, resolver de manera directa
        float minDistance = numeric_limits<float>::infinity();
        pair<Point, Point> closestPair;

        for (int i = start; i < end; ++i) {
            for (int j = i + 1; j < end; ++j) {
                float distance = euclideanDistance(points[i], points[j]);
                if (distance < minDistance) {
                    minDistance = distance;
                    closestPair = make_pair(points[i], points[j]);
                }
            }
        }

        return closestPair;
    }

    int mid = (start + end) / 2;

    // Solucionar el problema de forma recursiva para ambas partes
    pair<Point, Point> leftMin = closestPairUtil(points, start, mid);
    pair<Point, Point> rightMin = closestPairUtil(points, mid, end);

    // Calcular el mínimo entre ambas soluciones: LRmin
    float minLR = min(euclideanDistance(leftMin.first, leftMin.second), euclideanDistance(rightMin.first, rightMin.second));

    // Combinación de puntos
    vector<Point> strip;
    for (int i = start; i < end; ++i) {
        if (abs(points[i].x - points[mid].x) < minLR) {
            strip.push_back(points[i]);
        }
    }

    sort(strip.begin(), strip.end(), compareY);

    // Encontrar la distancia mínima dCmin entre todos los pares de puntos formados por un punto de cada parte
    pair<Point, Point> minStrip = make_pair(strip[0], strip[1]); // Inicializar con los primeros dos puntos
    for (int i = 0; i < strip.size(); ++i) {
        for (int j = i + 1; j < strip.size() && (strip[j].y - strip[i].y) < minLR; ++j) {
            float distance = euclideanDistance(strip[i], strip[j]);
            if (distance < euclideanDistance(minStrip.first, minStrip.second)) {
                minStrip = make_pair(strip[i], strip[j]);
            }
        }
    }

    // Retornar finalmente el par más cercano
    return (euclideanDistance(leftMin.first, leftMin.second) < euclideanDistance(minStrip.first, minStrip.second)) ? leftMin : minStrip;
}

pair<Point, Point> closestPair(const vector<Point>& points) {
    vector<Point> sortedPoints = points;
    sort(sortedPoints.begin(), sortedPoints.end(), compareX);

    return closestPairUtil(sortedPoints, 0, sortedPoints.size());
}

int main() {
    vector<Point> points = {{0.1, 0.2}, {0.3, 0.4}, {0.6, 0.7}, {0.8, 0.9}};
    pair<Point, Point> closestPoints = closestPair(points);

    cout << "Par de puntos más cercano: (" << closestPoints.first.x << ", " << closestPoints.first.y << ") y ("
         << closestPoints.second.x << ", " << closestPoints.second.y << ")" << endl;

    return 0;
}