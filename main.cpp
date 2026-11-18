#include <iostream>
#include <stack>
#include <queue>
#include <vector>
#include <algorithm>
#include <utility>

using namespace std;

//EJERCICIO 1: BST Iterativo y Análisis de Degradación
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

//PROBLEMA 2: Implementación de Red-Black Tree
enum Color { RED, BLACK };
class RedBlackTree {
private:
    struct Node {
        int key;
        Color color;
        Node* left; Node* right;
        Node* parent;
        Node(int k, Color c, Node* nil)
            : key(k), color(c), left(nil), right(nil), parent(nil) {}
    };
    Node* NIL; Node* root;
    void leftRotate(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        if (y->left != NIL) {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == NIL) {
            root = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }
    void rightRotate(Node* y) {
        Node* x = y->left;
        y->left = x->right;
        if (x->right != NIL) {
            x->right->parent = y;
        }
        x->parent = y->parent;
        if (y->parent == NIL) {
            root = x;
        } else if (y == y->parent->right) {
            y->parent->right = x;
        } else {
            y->parent->left = x;
        }
        x->right = y;
        y->parent = x;
    }
    void insertFixup(Node* z) {
        while (z->parent->color == RED) {
            if (z->parent == z->parent->parent->left) {
                Node* uncle = z->parent->parent->right;
                // Caso 1: Tío ROJO -> Recoloración
                if (uncle->color == RED) {
                    z->parent->color = BLACK;
                    uncle->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    //Caso 2: Tío NEGRO y z es hijo derecho -> Rotación izquierda
                    if (z == z->parent->right) {
                        z = z->parent;
                        leftRotate(z);
                    }
                    //Caso 3: Tío NEGRO y z es hijo izquierdo -> Recoloración + Rotación derecha
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rightRotate(z->parent->parent);
                }
            } else {
                Node* uncle = z->parent->parent->left;
                //Caso 1 simétrico
                if (uncle->color == RED) {
                    z->parent->color = BLACK;
                    uncle->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    //Caso 2 simétrico
                    if (z == z->parent->left) {
                        z = z->parent;
                        rightRotate(z);
                    }
                    //Caso 3 simétrico
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    leftRotate(z->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }
public:
    RedBlackTree() {
        NIL = new Node(0, BLACK, nullptr);
        NIL->left = NIL->right = NIL->parent = NIL;
        root = NIL;
    }
    ~RedBlackTree() {
        if (root != NIL) {
            queue<Node*> q;
            q.push(root);
            while (!q.empty()) {
                Node* curr = q.front();
                q.pop();
                if (curr->left != NIL) q.push(curr->left);
                if (curr->right != NIL) q.push(curr->right);
                delete curr;
            }
        }
        delete NIL;
    }
    void insert(int key) {
        Node* z = new Node(key, RED, NIL);
        Node* y = NIL;
        Node* x = root;
        while (x != NIL) {
            y = x;
            if (z->key < x->key) x = x->left;
            else if (z->key > x->key) x = x->right;
            else {
                delete z;
                return;
            }
        }
        z->parent = y;
        if (y == NIL) {
            root = z;
        } else if (z->key < y->key) {
            y->left = z;
        } else {
            y->right = z;
        }
        insertFixup(z);
    }
    bool verifyProperties() const {
        if (NIL->color != BLACK) return false;
        if (root == NIL) return true;
        if (root->color != BLACK) return false;
        struct State {
            Node* node;
            int blackCount;
        };
        stack<State> st;
        st.push({root, 0});
        int expectedBlackHeight = -1;
        while (!st.empty()) {
            State curr = st.top();
            st.pop();
            Node* u = curr.node;
            if (u == NIL) {
                int totalBlack = curr.blackCount + 1;
                if (expectedBlackHeight == -1) {
                    expectedBlackHeight = totalBlack;
                } else if (totalBlack != expectedBlackHeight) {
                    return false;
                }
                continue;
            }
            if (u->color != RED && u->color != BLACK) return false;
            if (u->color == RED) {
                if (u->left->color == RED || u->right->color == RED) {
                    return false;
                }
            }
            int nextBlack = curr.blackCount + (u->color == BLACK ? 1 : 0);
            st.push({u->right, nextBlack});
            st.push({u->left, nextBlack});
        }
        return true;
    }
    int height() const {
        if (root == NIL) return -1;
        queue<Node*> q;
        q.push(root);
        int h = -1;
        while (!q.empty()) {
            int sz = static_cast<int>(q.size());
            h++;
            for (int i = 0; i < sz; ++i) {
                Node* curr = q.front();
                q.pop();
                if (curr->left != NIL) q.push(curr->left);
                if (curr->right != NIL) q.push(curr->right);
            }
        }
        return h;
    }
    void printPreOrder() const {
        cout << "Pre-Order RBT: ";
        if (root == NIL) {
            cout << "(vacío)\n";
            return;
        }
        stack<Node*> st;
        st.push(root);
        while (!st.empty()) {
            Node* curr = st.top();
            st.pop();
            cout << curr->key << "(" << (curr->color == RED ? "R" : "B") << ") ";
            if (curr->right != NIL) st.push(curr->right);
            if (curr->left != NIL) st.push(curr->left);
        }
        cout << "\n";
    }
};