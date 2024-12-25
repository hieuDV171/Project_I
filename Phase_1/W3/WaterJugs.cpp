#include <iostream>
#define N 1000
using namespace std;
struct Node {
    int x, y;
    Node *next;
    Node *parent;
};
Node *head = nullptr;
Node *tail = nullptr;
Node *start = nullptr;
Node *target = nullptr;
int a, b, c;
int A[N][N];
int found = 0;

Node* makeNode(int u, int v, Node* par) {
    Node *p = new Node;
    p->x = u;
    p->y = v;
    p->next = nullptr;
    p->parent = par;
    return p;
}

void push(Node* r) {
    if (head == nullptr) {
        head = r;
        tail = r;
    } else {
        tail->next = r;
        tail = r;
    }
}

Node* pop() {
    if (head == nullptr)
        return nullptr;
    if (head == tail) {
        Node *tmp = head;
        head = nullptr;
        tail = nullptr;
        return tmp;
    } else {
        Node *tmp = head;
        head = head->next;
        return tmp;
    }
}

int checkFinal(int x, int y) {
    return (x == c || y == c || x + y == c);
}

int empty() {
    return head == nullptr;
}

void genStatus(int x, int y, Node* par) {
    A[x][y] = 1;
    Node *p = makeNode(x, y, par);
    push(p);
    if (checkFinal(x, y)) {
        found = 1;
        target = p;
    }
}

int solution(int a, int b, int c) {
    start = makeNode(0, 0, nullptr);
    push(start);
    A[0][0] = 1;
    while (!found && !empty()) {
        Node *p = pop();
        int x = p->x;
        int y = p->y;
        if (A[a][y] == 0) {
            genStatus(a, y, p);
        }
        if (A[x][b] == 0) {
            genStatus(x, b, p);
        }
        if (A[0][y] == 0) {
            genStatus(0, y, p);
        }
        if (A[x][0] == 0) {
            genStatus(x, 0, p);
        }
        if (x + y >= b) {
            if (A[x + y - b][b] == 0) {
                genStatus(x + y - b, b, p);
            }
        } else {
            if (A[0][x + y] == 0) {
                genStatus(0, x + y, p);
            }
        }
        if (x + y >= a) {
            if (A[a][x + y - a] == 0) {
                genStatus(a, x + y - a, p);
            }
        } else {
            if (A[x + y][0] == 0) {
                genStatus(x + y, 0, p);
            }
        }
    }
    int cnt = 0;
    if (found) {
        Node *t = target;
        while (t->parent != nullptr) {
            cnt++;
            t = t->parent;
        }
    } else {
        cnt = -1;
    }
    return cnt;
}

int main() {
    cin >> a >> b >> c;
    int rs = solution(a, b, c);
    cout << rs << endl;
    return 0;
}