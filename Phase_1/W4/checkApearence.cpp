#include <iostream>
using namespace std;
struct Node {
    int val;
    int height;
    Node *left;
    Node *right;

    Node(int v) : val(v),
                  height(1),
                  left(nullptr),
                  right(nullptr) {}
};
Node *root;

int max(int a, int b) {
    return a > b ? a : b;
}

int height(Node* r) {
    if (r == nullptr)
        return 0;
    return r->height;
}

int isBalanced(Node* r) {
    if (r == nullptr)
        return 0;
    return height(r->left) - height(r->right);
}

Node* roteRight(Node* r) {
    Node *p = r->left;
    Node *q = p->right;

    r->left = q;
    p->right = r;

    r->height = 1 + max(height(r->left), height(r->right));
    p->height = 1 + max(height(p->left), height(p->right));

    return p;
}

Node* roteLeft(Node* r) {
    Node *p = r->right;
    Node *q = p->left;

    r->right = q;
    p->left = r;

    r->height = 1 + max(height(r->left), height(r->right));
    p->height = 1 + max(height(p->left), height(p->right));

    return p;
}

Node* search(int v, Node* r) {
    if (r == nullptr) {
        return nullptr;
    }
    Node *p = r;
    while (p != nullptr) {
        if (p->val == v)
            return p;
        if (p->val < v)
            p = p->right;
        else if (p->val > v)
            p = p->left;
    }
    return p;
}

Node* Insert(int v, Node* r) {
    if (r == nullptr) {
        r = new Node(v);
    }
    if (r->val > v) {
        r->left = Insert(v, r->left);
    } else if (r->val < v) {
        r->right = Insert(v, r->right);
    }
    return r;

    r->height = 1 + max(height(r->left), height(r->right));

    int balance = isBalanced(r);
    if (balance > 1 && v < r->left->val) {
        return roteRight(r);
    } else if (balance > 1 && v > r->left->val) {
        r->left = roteLeft(r->left);
        return roteRight(r);
    } else if (balance < -1 && r->right->val < v) {
        return roteLeft(r);
    } else if (balance < -1 && v < r->right->val) {
        r->right = roteRight(r->right);
        return roteLeft(r);
    }

    return r;
}

int main() {
    int n;
    cin >> n;
    for (int i = 0; i < n; i++) {
        int v;
        cin >> v;
        Node *p = search(v, root);
        if (p == nullptr) {
            cout << 0 << endl;
            root = Insert(v, root);
        } else {
            cout << 1 << endl;
        }
    }
    return 0;
}
