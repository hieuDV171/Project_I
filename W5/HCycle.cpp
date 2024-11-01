#include <cstring>
#include <iostream>
#include <vector>

using namespace std;

const int MAX_N = 20;
vector<int> adj[MAX_N];
bool visited[MAX_N];
int n, m;

bool isHamiltonian(int pos, int start, int count) {
    if (count == n && pos == start) {
        return true;
    }

    for (int next : adj[pos]) {
        if (!visited[next]) {
            visited[next] = true;
            if (isHamiltonian(next, start, count + 1))
                return true;
            visited[next] = false;
        } else if (next == start && count == n) {
            return true;
        }
    }

    return false;
}

int checkHamiltonian() {
    for (int i = 0; i < n; i++) {
        memset(visited, false, sizeof(visited));
        visited[i] = true;
        if (isHamiltonian(i, i, 1)) {
            return 1;
        }
    }
    return 0;
}

int main() {
    int T;
    cin >> T;
    while (T--) {
        cin >> n >> m;

        for (int i = 0; i < n; i++)
            adj[i].clear();

        for (int i = 0; i < m; i++) {
            int u, v;
            cin >> u >> v;
            u--;
            v--;
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
        cout << checkHamiltonian() << endl;
    }

    return 0;
}
