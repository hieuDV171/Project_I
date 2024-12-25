#include <iostream>
using namespace std;
struct Node {
    char value;
    Node *prev;
    Node *next;
};
Node *top = nullptr;
Node *first = nullptr;

Node* makeNode(char v) {
    Node *q = new Node;
    q->value = v;
    q->prev = nullptr;
    q->next = nullptr;
    return q;
}

void push(char v) {
    Node *p = makeNode(v);
    if (top == nullptr) {
        top = p;
        first = p;
    } else {
        top->next = p;
        p->prev = top;
        top = p;
    }
}

char pop() {
    if (top == nullptr)
        return '\n';
    if (top == first) {
        Node *tmp = top;
        char c = tmp->value;
        top = nullptr;
        first = nullptr;
        delete tmp;
        return c;
    } else {
        Node *tmp = top;
        char c = tmp->value;
        top = top->prev;
        top->next = nullptr;
        delete tmp;
        return c;
    }
}

int check(char a, char b) {
    if (a == '(' && b == ')')
        return 1;
    else if (a == '[' && b == ']')
        return 1;
    else if (a == '{' && b == '}')
        return 1;
    return 0;
}

int Parenthesis(string s) {
    int len = s.size();
    for (int i = 0; i < len; i++) {
        if (s[i] == '(' || s[i] == '[' || s[i] == '{') {
            push(s[i]);
        } else if (s[i] == ')' || s[i] == ']' || s[i] == '}') {
            char c = pop();
            if (c == '\n' ||!check(c, s[i])) {
                return 0;
            }
        }
    }
    if (top != nullptr)
        return 0;
    else
        return 1;
}

int main() {
    string s;
    getline(cin, s);
    int rs = Parenthesis(s);
    cout << rs << endl;
    return 0;
}