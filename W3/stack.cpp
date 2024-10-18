#include <iostream>
using namespace std;

struct Node {
    int val;
    Node *prev;
    Node *next;
};
Node *first = nullptr;
Node *last = nullptr;

Node* makeNode(int v) {
    Node *p = new Node;
    p->val = v;
    p->prev = nullptr;
    p->next = nullptr;
    return p;
}

void push(int v) {
    Node *p = makeNode(v);
    if (last == nullptr) {
        first = p;
        last = p;
    } else {
        last->next = p;
        p->prev = last;
        last = p;
    }
}

Node* pop() {
    if (first == nullptr)
        return nullptr;
    if (first == last) {
        Node *tmp = first;
        first = nullptr;
        last = nullptr;
        return tmp;
    } else {
        Node *tmp = last;
        last = last->prev;
        last->next = nullptr;
        return tmp;
    }
}

int main() {
    string s;
    int A[1000];
    int i = 0;
    while (s.compare("#") != 0) {
        cin >> s;
        if (s.compare("PUSH") == 0) {
            int v;
            cin >> v;
            push(v);
        } else if (s.compare("POP") == 0) {
            Node *p = pop();
            if (p == nullptr) {
                A[i] = 0;
                i++;
            } else {
                A[i] = p->val;
                i++;
            }
        }
    }
    for (int j = 0; j < i; j++) {
        if (A[j] == 0) {
            cout << "NULL" << endl;
        } else
            cout << A[j] << endl;
    }
    return 0;
}