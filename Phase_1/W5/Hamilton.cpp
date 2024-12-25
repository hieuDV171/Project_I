#include <iostream>
#include <string.h>
#define N 21
using namespace std;
struct Node {
    int val;
    Node *next;
};
Node *A[N];
bool visited[N];
int x[N];
int n, m; // number of nodes and edges

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

void solution() {
    for (int i = 0; i < n; i++) {
        cout << x[i] << " ";
    }
    cout << endl;
}

int check(int val) {
    if (!visited[val])
        return 1;
    return 0;
}

int Try(int i, int start, int k) {
    for (Node *adj = A[i]; adj != nullptr; adj = adj->next) {
        if (check(adj->val)) {
            x[k] = adj->val;
            visited[adj->val] = true;
            if (Try(adj->val, start, k + 1)) {
                return true;
            }
            visited[adj->val] = false;
            x[k] = 0;
        } else if (adj->val == start && k == n) {
            solution();
            return true;
        }
    }
    return false;
}

int checkHamiltonian() {
    for (int i = 1; i <= n; i++) {
        memset(visited, false, sizeof(visited));
        memset(x, 0, sizeof(x));
        visited[i] = true;
        x[0] = i;
        if (Try(i, i, 1)) {
            return 1;
        }
    }
    return 0;
}

int main() {
    int T; // number of graphs;
    cin >> T;
    while (T > 0) {
        cin >> n >> m;
        for (int i = 1; i <= n; i++) {
            A[i] = nullptr;
        }
        for (int i = 0; i < m; i++) {
            int x, y;
            cin >> x >> y;
            push(y, A[x]);
            push(x, A[y]);
        }
        cout << checkHamiltonian() << endl;
        T--;
    }

    return 0;
}