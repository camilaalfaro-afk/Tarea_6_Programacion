#include <iostream>
#include <stack>
#include <queue>
#include <vector>
#include <algorithm>
#include <utility>

using namespace std;

// EJERCICIO 1: BST Iterativo y Análisis de Degradación
class IterativeBST {
private:
    struct Node {
        int key;
        Node* left; Node* right;
        explicit Node(int k) : key(k), left(nullptr), right(nullptr) {}
    };

    Node* root = nullptr;

public:
    ~IterativeBST() {
        if (!root) return;
        queue<Node*> q;
        q.push(root);
        while (!q.empty()) {
            Node* curr = q.front();
            q.pop();
            if (curr->left) q.push(curr->left);
            if (curr->right) q.push(curr->right);
            delete curr;
        }
    }

    //Inserción iterativa
    void insert(int key) {
        Node* newNode = new Node(key);
        if (!root) {
            root = newNode; return;
        }

        Node* curr = root;
        Node* parent = nullptr;

        while (curr != nullptr) {
            parent = curr;
            if (key < curr->key) {
                curr = curr->left;
            } else if (key > curr->key) {
                curr = curr->right;
            } else {
                delete newNode; return;
            }
        }

        if (key < parent->key) {
            parent->left = newNode;
        } else {
            parent->right = newNode;
        }
    }

    //Eliminación iterativa
    bool remove(int key) {
        Node* curr = root;
        Node* parent = nullptr;
        while (curr != nullptr && curr->key != key) {
            parent = curr;
            if (key < curr->key) curr = curr->left;
            else curr = curr->right;
        }
        if (!curr) return false;
        //Caso A: 0 o 1 hijo
        if (!curr->left || !curr->right) {
            Node* child = (curr->left) ? curr->left : curr->right;
            if (!parent) {
                root = child;
            } else if (parent->left == curr) {
                parent->left = child;
            } else {
                parent->right = child;
            }
            delete curr;
        }
        //Caso B: 2 hijos (reemplazar por sucesor in-order)
        else {
            Node* succParent = curr;
            Node* succ = curr->right;
            while (succ->left != nullptr) {
                succParent = succ;
                succ = succ->left;
            }
            curr->key = succ->key;
            if (succParent->left == succ) {
                succParent->left = succ->right;
            } else {
                succParent->right = succ->right;
            }
            delete succ;
        }
        return true;
    }
    //Recorrido In-Order usando exclusivamente stack
    void printInOrder() const {
        stack<Node*> st;
        Node* curr = root;
        cout << "In-Order: ";
        while (curr != nullptr || !st.empty()) {
            while (curr != nullptr) {
                st.push(curr);
                curr = curr->left;
            }
            curr = st.top();
            st.pop();
            cout << curr->key << " ";
            curr = curr->right;
        }
        cout << "\n";
    }
    //Cálculo iterativo de la altura (aristas desde raíz a hoja más lejana; -1 si vacío)
    int height() const {
        if (!root) return -1;
        queue<Node*> q;
        q.push(root);
        int h = -1;
        while (!q.empty()) {
            int levelSize = static_cast<int>(q.size());
            h++;
            for (int i = 0; i < levelSize; ++i) {
                Node* curr = q.front();
                q.pop();
                if (curr->left) q.push(curr->left);
                if (curr->right) q.push(curr->right);
            }
        }
        return h;
    }
};