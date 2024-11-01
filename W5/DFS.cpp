#include <iostream>
#define N 100001
using namespace std;
struct Node {
    int val;
    Node *next;
};
Node *A[N];
bool visited[N];

Node *makeNode(int u) {
    Node *p = new Node;
    p->val = u;
    p->next = nullptr;
    return p;
}

void push(int u, Node *&r) {
    Node *q = makeNode(u);
    if (r == nullptr) {
        r = q;
    } else {
        Node *p = r;
        while (p->next != nullptr) {
            p = p->next;
        }
        p->next = q;
    }
}

void DFS(int u) {
    visited[u] = true;
    cout << u << " ";
    Node *p = A[u];
    while (p != nullptr) {
        if (!visited[p->val]) {
            DFS(p->val);
        }
        p = p->next;
    }
}

int solve(int n) {
    int rs = 0;
    for (int v = 1; v <= n; v++) {
        if (!(visited[v])) {
            rs++;
            DFS(v);
        }
    }
    return rs;
}

int main() {
    int n, M;
    cin >> n >> M;
    for (int i = 0; i < M; i++) {
        int x, y;
        cin >> x >> y;
        push(y, A[x]);
        push(x, A[y]);
    }
    for (int v = 1; v <= n; v++) {
        visited[v] = false;
    }
    int rs = solve(n);
    return 0;
}