#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

struct Rectangle {
    double x1, y1, x2, y2;

    Rectangle(double a = 0, double b = 0, double c = 0, double d = 0) : x1(a), y1(b), x2(c), y2(d) {}
};

struct Node {
    Rectangle mbr;
    vector<Node*> children; // Cambiamos a vector de nodos en lugar de rectángulos
    Node* parent;

    Node(Node* p = nullptr) : mbr(), parent(p) {}
};

const int MAX_CHILDREN = 4;

// Función para construir el R-tree
Node* buildRTree(const vector<Rectangle>& rectangles, int m) {
    if (rectangles.empty()) {
        return nullptr;
    }

    vector<Node*> nodes;

    for (const Rectangle& rect : rectangles) {
        Node* node = new Node();
        node->mbr = rect;
        nodes.push_back(node);
    }

    while (nodes.size() > 1) {
        vector<Node*> nextLevel;
        for (int i = 0; i < nodes.size(); i += m) {
            Node* node = new Node();
            node->children.assign(nodes.begin() + i, nodes.begin() + min(i + m, static_cast<int>(nodes.size())));
            for (Node* child : node->children) {
                if (child) {
                    child->parent = node;
                }
            }
            for (Node* child : node->children) {
                if (child) {
                    if (node->mbr.x1 == 0 && node->mbr.x2 == 0 && node->mbr.y1 == 0 && node->mbr.y2 == 0) {
                        node->mbr = child->mbr;
                    }
                    node->mbr.x1 = min(node->mbr.x1, child->mbr.x1);
                    node->mbr.y1 = min(node->mbr.y1, child->mbr.y1);
                    node->mbr.x2 = max(node->mbr.x2, child->mbr.x2);
                    node->mbr.y2 = max(node->mbr.y2, child->mbr.y2);
                }
            }
            nextLevel.push_back(node);
        }
        nodes = nextLevel;
    }

    return nodes[0];
}


// Función para eliminar nodos
void deleteNodes(Node* root) {
    if (!root) {
        return;
    }
    for (Node* child : root->children) {
        deleteNodes(child);
    }
    delete root;
}

// Función para guardar el R-tree en un archivo binario
void saveRTree(Node* root, const string& filename) {
    ofstream file(filename, ios::out | ios::binary);
    if (!file.is_open()) {
        cerr << "Error al abrir el archivo para escritura." << endl;
        return;
    }

    // Aquí deberías serializar la estructura del árbol y escribirla en el archivo binario

    file.close();
}

// Función para cargar el R-tree desde un archivo binario
Node* loadRTree(const string& filename) {
    ifstream file(filename, ios::in | ios::binary);
    if (!file.is_open()) {
        cerr << "Error al abrir el archivo para lectura." << endl;
        return nullptr;
    }

    Node* root = nullptr;

    // Aquí deberías deserializar la estructura del árbol desde el archivo binario

    file.close();

    return root;
}

int main() {
    vector<Rectangle> rectangles = {
        Rectangle(1, 2, 3, 4),
        Rectangle(2, 3, 4, 5),
        Rectangle(3, 4, 5, 6),
        Rectangle(4, 5, 6, 7)
    };

    int M = 2; // Número máximo de hijos por nodo

    Node* root = buildRTree(rectangles, M);
    cout << "a" << endl;
    saveRTree(root, "rtree.bin");
    cout << "b" << endl;
    Node* loadedRoot = loadRTree("rtree.bin");
    cout << "c" << endl;
    cout << "d" << endl;

    // Realiza operaciones con el R-tree

    deleteNodes(root); // Limpia la memoria al final
    deleteNodes(loadedRoot);

    return 0;
}