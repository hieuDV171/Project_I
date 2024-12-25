#include <iostream>
#include <string.h>
#include <queue>
#define INF 1000000000
#define MAX_N 10000
using namespace std;

int flow[MAX_N][MAX_N];
int capacity[MAX_N][MAX_N];
bool visited[MAX_N];

void initialGraph(int n, int m) {
    for (int u = 1; u <= n; u++) {
        for (int v = 1; v <= n; v++) {
            flow[u][v] = 0;
            capacity[u][v] = 0;
        }
    }
        for (int i = 0; i < m; i++) {
            int u, v, cap;
            cin >> u >> v >> cap;
            capacity[u][v] += cap;
        }
}

bool bfs(int parent[], int n, int source, int target) {
    queue<int> q;
    for (int i = 1; i <= n; i++) {
        visited[i] = false;
    }
    q.push(source);
    visited[source] = true;
    parent[source] = -1;
    while(!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v = 1; v <= n; v++) {
            if (!visited[v] && capacity[u][v] - flow[u][v] > 0) {
                q.push(v);
                parent[v] = u;
                visited[v] = true;
                if (v == target) {
                    return true;
                }
            }
        }
    }
    return false;
}

int edmonds_Karp(int n, int m, int source, int target) {
    int maxFlow = 0;
    int parent[MAX_N];
    while (bfs(parent, n, source, target)) {
        int pathFlow = INF;
        for (int v = target; v != source; v = parent[v]) {
            int u = parent[v];
            pathFlow = min(pathFlow, capacity[u][v] - flow[u][v]);
        }

        for (int v = target; v != source; v = parent[v]) {
            int u = parent[v];
            flow[u][v] += pathFlow;
            flow[v][u] -= pathFlow;
        }
        maxFlow += pathFlow;
    }
    return maxFlow;
}

int main() {
    int n, m, source, target;
    cin >> n >> m;
    cin >> source >> target;
    initialGraph(n, m);
    int maxFlow = edmonds_Karp(n, m, source, target);
    cout << maxFlow << endl;
    return 0;
}