#include <iostream>
using namespace std;
struct Node {
    int val;
    Node* left;
    Node *right;
};
Node *root;

Node* makeNode(int v) {
    Node *p = new Node;
    p->val = v;
    p->left = nullptr;
    p->right = nullptr;
    return p;
}

Node* insert(int k, Node* r) {
    if (r == nullptr) {
        r = makeNode(k);
    } else {
        if (r->val > k) {
            r->left = insert(k, r->left);
        } else if (r->val < k) {
            r->right = insert(k, r->right);
        }
    }
    return r;
}

void PreOrder(Node* r) {
    if (r == nullptr) {
        return;
    }
    cout << r->val << " ";
    PreOrder(r->left);
    PreOrder(r->right);
}

int main() {
    string s;
    while (s.compare("#") != 0) {
        cin >> s;
        if (s.compare("insert") == 0) {
            int k;
            cin >> k;
            root = insert(k, root);
        }
    }
    PreOrder(root);
    cout << endl;
    return 0;
}