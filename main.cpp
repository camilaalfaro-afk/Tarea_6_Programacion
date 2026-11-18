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

// PROBLEMAS 3 y 4: Árbol Binario General para DFS y BFS Avanzado
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    explicit TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};
//PROBLEMA 3: DFS Iterativo Avanzado (Exclusivamente stack, sin recursión)
vector<int> findRootToLeafPathWithSumK(TreeNode* root, int K) {
    if (!root) return {};
    struct Frame {
        TreeNode* node;
        int currentSum;
        vector<int> path;
    };
    stack<Frame> st;
    st.push({root, root->val, {root->val}});
    while (!st.empty()) {
        Frame curr = move(st.top());
        st.pop();
        TreeNode* u = curr.node;
        if (!u->left && !u->right) {
            if (curr.currentSum == K) {
                return curr.path;
            }
        }
        if(u->right) {
            vector<int> rightPath = curr.path;
            rightPath.push_back(u->right->val);
            st.push({u->right, curr.currentSum + u->right->val, move(rightPath)});
        }
        if (u->left) {
            vector<int> leftPath = curr.path;
            leftPath.push_back(u->left->val);
            st.push({u->left, curr.currentSum + u->left->val, move(leftPath)});
        }
    }
    return {};
}
//PROBLEMA 4: BFS Iterativo Avanzado (Exclusivamente queue, sin recursión)
struct BFSMetrics {
    bool isComplete;
    int maxWidth;
    int minDepth;
};

BFSMetrics analyzeTreeBFS(TreeNode* root) {
    if (!root) {
        return {true, 0, 0};
    }
    queue<TreeNode*> q;
    q.push(root);
    bool isComplete = true;
    bool seenNullChild = false;
    int maxWidth = 0;
    int minDepth = -1;
    int currentDepth = 0;

    while (!q.empty()) {
        int levelWidth = static_cast<int>(q.size());
        currentDepth++;
        maxWidth = max(maxWidth, levelWidth);
        for (int i = 0; i < levelWidth; ++i) {
            TreeNode* curr = q.front();
            q.pop();
            if (!curr->left && !curr->right && minDepth == -1) {
                minDepth = currentDepth;
            }
            if (curr->left) {
                if (seenNullChild) isComplete = false;
                q.push(curr->left);
            } else {
                seenNullChild = true;
            }
            if (curr->right) {
                if (seenNullChild) isComplete = false;
                q.push(curr->right);
            } else {
                seenNullChild = true;
            }
        }
    }
    return {isComplete, maxWidth, minDepth};
}

int main() {
    cout << "EJERCICIO 1: BST Iterativo (20 inserciones, 2 eliminaciones)\n";
    IterativeBST bstRandom;
    vector<int> values20 = {
        50, 30, 70, 20, 40, 60, 80, 15, 25, 35,
        45, 55, 65, 75, 85, 10, 22, 37, 62, 90
    };
    for (int v : values20) bstRandom.insert(v);
    cout << "Altura inicial (20 elementos): " << bstRandom.height() << "\n";
    bstRandom.remove(10);
    bstRandom.remove(30);
    cout << "Tras eliminar 10 y 30:\n";
    bstRandom.printInOrder();
    cout << "Altura final: " << bstRandom.height() << "\n\n";

    cout << "PROBLEMA 2: Red-Black Tree vs BST (20 datos ascendentes)\n";
    IterativeBST degradedBST;
    RedBlackTree rbt;
    for (int i = 1; i <= 20; ++i) {
        degradedBST.insert(i);
        rbt.insert(i);
    }
    cout << "Insercion ascendente 1..20:\n";
    cout << " -> Altura BST tradicional: " << degradedBST.height() << "\n";
    cout << " -> Altura Red-Black Tree:  " << rbt.height() << "\n";
    cout << " -> Propiedades RBT validas: "
         << (rbt.verifyProperties() ? "SI (5/5 cumplidas)" : "NO") << "\n";
    rbt.printPreOrder();
    cout << "\n";

    cout << "PROBLEMA 3: DFS Iterativo (Camino raiz-hoja con suma K)\n";
    TreeNode* sampleTree = new TreeNode(5);
    sampleTree->left = new TreeNode(4);
    sampleTree->right = new TreeNode(8);
    sampleTree->left->left = new TreeNode(11);
    sampleTree->left->left->left = new TreeNode(7);
    sampleTree->left->left->right = new TreeNode(2);
    sampleTree->right->left = new TreeNode(13);
    sampleTree->right->right = new TreeNode(4);
    sampleTree->right->right->right = new TreeNode(1);
    int K = 22;
    vector<int> path = findRootToLeafPathWithSumK(sampleTree, K);
    cout << "Buscando camino raiz-hoja con suma K = " << K << ": ";
    if (!path.empty()) {
        for (size_t i = 0; i < path.size(); ++i) {
            cout << path[i] << (i + 1 < path.size() ? " -> " : "\n");
        }
    } else {
        cout << "No encontrado\n";
    }
    cout << "\n";

    cout << "PROBLEMA 4: BFS Iterativo Avanzado\n";
    BFSMetrics m1 = analyzeTreeBFS(sampleTree);
    cout << "Arbol de muestra (Problema 3):\n";
    cout << " -> Es completo:        " << (m1.isComplete ? "SI" : "NO") << "\n";
    cout << " -> Ancho maximo:       " << m1.maxWidth << "\n";
    cout << " -> Profundidad minima: " << m1.minDepth << "\n";
    TreeNode* completeTree = new TreeNode(1);
    completeTree->left = new TreeNode(2);
    completeTree->right = new TreeNode(3);
    completeTree->left->left = new TreeNode(4);
    completeTree->left->right = new TreeNode(5);
    completeTree->right->left = new TreeNode(6);
    BFSMetrics m2 = analyzeTreeBFS(completeTree);
    cout << "Arbol completo de 6 nodos:\n";
    cout << " -> Es completo:        " << (m2.isComplete ? "SI" : "NO") << "\n";
    cout << " -> Ancho maximo:       " << m2.maxWidth << "\n";
    cout << " -> Profundidad minima: " << m2.minDepth << "\n";

    delete sampleTree->left->left->left;
    delete sampleTree->left->left->right;
    delete sampleTree->left->left;
    delete sampleTree->left;
    delete sampleTree->right->left;
    delete sampleTree->right->right->right;
    delete sampleTree->right->right;
    delete sampleTree->right;
    delete sampleTree;

    delete completeTree->left->left;
    delete completeTree->left->right;
    delete completeTree->left;
    delete completeTree->right->left;
    delete completeTree->right;
    delete completeTree;

    return 0;
}