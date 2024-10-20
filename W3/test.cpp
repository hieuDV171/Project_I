#include <iostream>
#include <vector>
#define M 100
using namespace std;

struct Node {
    string name;
    Node *leftMostChild;
    Node *rightSibling;
    Node(string v) : name(v), leftMostChild(nullptr), rightSibling(nullptr) {}
};

Node *root = nullptr;
vector<Node *> A[M]; // Sử dụng vector để lưu các node tại một vị trí băm

Node *makeNode(string v) {
    return new Node(v);
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
    A[i].push_back(r); // Thêm node vào danh sách ở chỉ số băm
}

Node *get(string s) {
    int i = Hash(s);
    for (Node *node : A[i]) { // Duyệt qua danh sách để tìm node đúng
        if (node->name == s) {
            return node;
        }
    }
    return nullptr;
}

int have(string s) {
    return get(s) != nullptr;
}

Node *search(string s, Node *r) {
    if (r == nullptr) {
        return nullptr;
    }
    if (r->name == s) {
        return r;
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

    if (!child) {
        child = makeNode(c);
        put(child);
    }

    if (!par) {
        par = makeNode(p);
        put(par);
    }

    if (root == nullptr) {
        root = par;
    }

    if (par->leftMostChild == nullptr) {
        par->leftMostChild = child;
    } else {
        Node *sibling = par->leftMostChild;
        while (sibling->rightSibling != nullptr) {
            sibling = sibling->rightSibling;
        }
        sibling->rightSibling = child;
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
    if (!p)
        return 0;
    return PreOrder(p) - 1;
}

int height(Node *r) {
    if (r == nullptr)
        return 0;
    int maxH = 0;
    Node *p = r->leftMostChild;
    while (p != nullptr) {
        maxH = max(maxH, height(p));
        p = p->rightSibling;
    }
    return maxH + 1;
}

int generation(string s) {
    Node *p = search(s, root);
    if (!p)
        return 0;
    return height(p) - 1;
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

    while (true) {
        cin >> s;
        if (s.compare("***") == 0) {
            break;
        } else if (s == "descendants") {
            string u;
            cin >> u;
            cout << descendants(u) << endl;
        } else if (s == "generation") {
            string u;
            cin >> u;
            cout << generation(u) << endl;
        }
    }

    return 0;
}
