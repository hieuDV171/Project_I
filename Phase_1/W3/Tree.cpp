#include <iostream>
using namespace std;

struct Node {
    int id;
    Node *leftMostChild;
    Node *rightSibling;
};
Node *root;

Node* makeNode(int v) {
    Node *p = new Node;
    p->id=v;
    p->leftMostChild = nullptr;
    p->rightSibling = nullptr;
    return p;
}

Node* search(int v, Node* r) {
    if (r == nullptr)
        return nullptr;
    if (r->id == v)
        return r;
    Node *p = r->leftMostChild;
    if (p == nullptr)
        return nullptr;
    while (p != nullptr) {
        Node* rs = search(v, p);
        if (rs != nullptr)
            return rs;
        p = p->rightSibling;
    }
    return NULL;
}

void Insert(int u, int v) {
    Node *p = search(v, root);
    Node *q = search(u, root);
    if (p == nullptr) {
        return;
    }
    if (q != nullptr)
        return;
    Node *h = makeNode(u);
    if (p->leftMostChild == nullptr) {
        p->leftMostChild = h;
    } else {
        Node *t = p->leftMostChild;
        while (t->rightSibling != nullptr) {
            t = t->rightSibling;
        }
        t->rightSibling = h;
    }
}

void visit(Node* r) {
    cout << r->id << " ";
}

void InOrder(Node* r) {
    if (r == nullptr)
        return;
    Node *p = r->leftMostChild;
    if (p != nullptr) {
        InOrder(p);
        p = p->rightSibling;
    }
    visit(r);
    while(p != nullptr) {
        InOrder(p);
        p = p->rightSibling;
    }
}

void PreOrder(Node* r) {
    if (r == nullptr)
        return;
    visit(r);
    Node *p = r->leftMostChild;
    while (p != nullptr) {
        PreOrder(p);
        p = p->rightSibling;
    }
}

void PostOrder(Node* r) {
    if (r == nullptr)
        return;
    Node *p = r->leftMostChild;
    while (p != nullptr) {
        PostOrder(p);
        p = p->rightSibling;
    }
    visit(r);
}

int main() {
    string s;
    int A[100], B[100], C[100];
    int i = 0, j = 0, k = 0;
    while (s.compare("*") != 0) {
        cin >> s;
        if (s.compare("MakeRoot") == 0) {
            int v;
            cin >> v;
            root = makeNode(v);
        } else if (s.compare("Insert") == 0) {
            int u, v;
            cin >> u >> v;
            Insert(u, v);
        } else if (s.compare("InOrder") == 0) {
            InOrder(root);
            cout << endl;
        } else if (s.compare("PreOrder") == 0) {
            PreOrder(root);
            cout << endl;
        } else if (s.compare("PostOrder") == 0) {
            PostOrder(root);
            cout << endl;
        }
    }
    return 0;
}