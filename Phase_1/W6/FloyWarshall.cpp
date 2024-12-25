#include <iostream>
#include <iomanip>
#define INF 1000000000
using namespace std;
int d[10000][10000];

void initialWeight(int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                d[i][j] = 0;
            } else {
                d[i][j] = INF;
            }
        }
    }
}

void addEdge(int u, int v, int w) {
    d[u][v] = w;
}

void printWeight(int n) {
    for (int u = 0; u < n; u++) {
        for (int v = 0; v < n; v++) {
            if (d[u][v] == INF) {
                cout << "-1 ";
            } else {
                cout << setw(2) << d[u][v] << " ";
            }
        }
        cout << endl;
    }
}

void floyWarshall(int n) {
    for (int k = 0; k < n; k++) {
        for (int u = 0; u < n; u++) {
            for (int v = 0; v < n; v++) {
                if (d[u][v] > d[u][k] + d[k][v]) {
                    d[u][v] = d[u][k] + d[k][v];
                }
            }
        }
    }
}

int main() {
    int n, m;
    cin >> n >> m;

    initialWeight(n);

    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        addEdge(u - 1, v - 1, w);
    }

    floyWarshall(n);
    printWeight(n);
    return 0;
}