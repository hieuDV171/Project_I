#include <iostream>
#include <vector>
#define M 100
using namespace std;
struct Node {
    string name;
    Node *leftMostChild;
    Node *rightSibling;
};
Node *root = nullptr;
vector<Node*> A[M];

Node *makeNode(string v) {
    Node *p = new Node;
    p->name = v;
    p->leftMostChild = nullptr;
    p->rightSibling = nullptr;
    return p;
}

int Hash(string s) {
    int l = s.length();
    int p = 1, S = 0;
    for (int i = 0; i < l; i++) {
        S = (S + (s[l - 1 - i] * p) % M) % M;
        p = (p * 256) % M;
    }
    return S;
}

void put(Node *r) {
    int i = Hash(r->name);
    A[i].push_back(r);
}

Node* get(string s) {
    int i = Hash(s);
    for (Node *node : A[i]) {
        if (node->name == s) {
            return node;
        }
    }
    return nullptr;
}

Node *search(string s, Node *r) {
    if (r == nullptr) {
        return nullptr;
    }
    if (r->name.compare(s) == 0) {
        return r;
    }
    if (r->leftMostChild == nullptr) {
        return nullptr;
    }
    Node *p = r->leftMostChild;
    while (p != nullptr) {
        Node *rs = search(s, p);
        if (rs != nullptr)
            return rs;
        p = p->rightSibling;
    }
    return nullptr;
}

void Insert(string c, string p) {
    Node *child = get(c);
    Node *par = get(p);
    if (child == nullptr) {
        child = makeNode(c);
        put(child);
    }
    if (par == nullptr) {
        par = makeNode(p);
        put(par);
    }
    if (root == nullptr) {
        root = par;
    }
    if (par->leftMostChild == nullptr) {
        par->leftMostChild = child;
    } else {
        Node *p = par->leftMostChild;
        while (p->rightSibling != nullptr) {
            p = p->rightSibling;
        }
        p->rightSibling = child;
    }
}

int PreOrder(Node *r) {
    if (r == nullptr)
        return 0;
    int cnt = 1;
    Node *p = r->leftMostChild;
    while (p != nullptr) {
        cnt += PreOrder(p);
        p = p->rightSibling;
    }
    return cnt;
}

int descendants(string s) {
    Node *p = search(s, root);
    int rs = PreOrder(p);
    return rs - 1;
}

int height(Node *r) {
    if (r == nullptr)
        return 0;
    if (r->leftMostChild == nullptr) {
        return 1;
    }
    int maxH = 0;
    Node *p = r->leftMostChild;
    while (p != nullptr) {
        int h = height(p);
        maxH = maxH > h + 1 ? maxH : h + 1;
        p = p->rightSibling;
    }
    return maxH;
}

int generation(string s) {
    Node *p = search(s, root);
    int rs = height(p);
    return rs - 1;
}

int main() {
    string s;
    while (1) {
        string u, v;
        cin >> u;
        if (u.compare("***") == 0)
            break;
        cin >> v;
        Insert(u, v);
    }
    while (s.compare("***") != 0) {
        cin >> s;
        if (s.compare("descendants") == 0) {
            string u;
            cin >> u;
            int rs = descendants(u);
            cout << rs << endl;
        } else if (s.compare("generation") == 0) {
            string u;
            cin >> u;
            int rs = generation(u);
            cout << rs << endl;
        }
    }
    return 0;
}