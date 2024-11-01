#include <iostream>
#define N 100001
using namespace std;
struct Node {
    int id;
    Node *next;

    Node(int v) : id(v), next(nullptr) {}
};
Node *head;
Node *tail;
Node *A[N];
bool visited[N];
int n, m;

int empty() {
    return (head == nullptr && tail == nullptr);
}

bool had(int v) {
    if (head == nullptr)
        return false;
    Node *p = head;
    while (p != nullptr) {
        if (p->id == v)
            return true;
        p = p->next;
    }
    return false;
}

void initQueue() {
    head = nullptr;
    tail = nullptr;
}

void push(int v) {
    Node *q = new Node(v);
    if (head == nullptr) {
        head = q;
        tail = q;
    } else {
        tail->next = q;
        tail = q;
    }
}

Node *add(int v, Node *r) {
    Node *q = new Node(v);
    if (r == nullptr) {
        r = q;
    } else {
        if (r->id > v) {
            q->next = r;
            r = q;
        } else {
            Node *p = r;
            while (p->next != nullptr && p->next->id < v) {
                p = p->next;
            }
            q->next = p->next;
            p->next = q;
        }
    }
    return r;
}

Node *pop() {
    if (head == nullptr)
        return nullptr;
    Node *tmp = head;
    head = head->next;
    if (head == nullptr)
        tail = nullptr;
    return tmp;
}

void BFS(int u) {
    initQueue();
    push(u);
    while (!empty()) {
        int v = pop()->id;
        visited[v] = true;
        cout << v << " ";
        for (Node *i = A[v]; i != nullptr; i = i->next) {
            if (!visited[i->id] && !had(i->id)) {
                push(i->id);
            }
        }
    }
}

void solve() {
    for (int i = 1; i <= n; i++) {
        visited[i] = false;
    }
    for (int i = 1; i <= n; i++) {
        if (!visited[i]) {
            BFS(i);
        }
    }
}

// void PrintAi(int i) {
//     Node *p = A[i];
//     while (p != nullptr) {
//         cout << p->id << " ";
//         p = p->next;
//     }
//     cout << endl;
// }

int main() {
    cin >> n >> m;
    for (int i = 0; i < m; i++) {
        int x, y;
        cin >> x >> y;
        A[x] = add(y, A[x]);
        A[y] = add(x, A[y]);
    }
    solve();
    // cout << endl;
    // for (int i = 1; i <= n; i++) {
    //     cout << "A[" << i << "]: ";
    //     PrintAi(i);
    // }
    return 0;
}