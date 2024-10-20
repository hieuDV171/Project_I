#include <iostream>
using namespace std;
struct Node {
    int val;
    Node *next;
};

Node *head = nullptr;
Node *tail = nullptr;

Node* makeNode(int v) {
    Node *p = new Node;
    p->val = v;
    p->next = nullptr;
    return p;
}

void push(int v) {
    Node *p = makeNode(v);
    if (head == nullptr) {
        head = p;
        tail = p;
    } else {
        tail->next = p;
        tail = p;
    }
}

Node* pop() {
    if (tail == nullptr)
        return nullptr;
    if (head == tail) {
        Node* tmp = head;
        head = nullptr;
        tail = nullptr;
        return tmp;
    } else {
        Node *tmp = head;
        head = head->next;
        return tmp;
    }
}

int main() {
    string s;
    while(s.compare("#") != 0) {
        cin >> s;
        if (s.compare("PUSH") == 0) {
            int v;
            cin >> v;
            push(v);
        } else if (s.compare("POP") == 0) {
            Node *p = pop();
            if (p == nullptr) {
                cout << "NULL" << endl;
            } else {
                cout << p->val << endl;
            }
        }
    }
    return 0;
}