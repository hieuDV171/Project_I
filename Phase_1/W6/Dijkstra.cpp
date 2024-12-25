#include <iostream>
using namespace std;

const int INF = 1000000000;
const int MAX_NODE = 100000;

struct Edge {
    int to;
    int weight;
    Edge *next;
};

Edge *Graph[MAX_NODE];
bool visited[MAX_NODE];
int Distance[MAX_NODE];
int parent[MAX_NODE];

struct PriorityQueue {
    struct Element {
        int node;
        int distance;
    };
    Element elements[MAX_NODE];
    int size;

    PriorityQueue() : size(0) {}

    void push(int node, int distance) {
        int i = size++;
        elements[i].node = node;
        elements[i].distance = distance;
        while (i > 0) {
            if (elements[i].distance < elements[(i - 1) / 2].distance) {
                Element tmp = elements[i];
                elements[i] = elements[(i - 1) / 2];
                elements[(i - 1) / 2] = tmp;
            }
            i = (i - 1) / 2;
        }
    }

    Element pop() {
        Element minElement = elements[0];
        elements[0] = elements[--size];
        int i = 0;
        while (2 * i + 1 < size) {
            int j = 2 * i + 1;
            if (j + 1 < size && elements[j + 1].distance < elements[j].distance)
                j++;
            if (elements[i].distance <= elements[j].distance) {
                break;
            }
            Element tmp = elements[i];
            elements[i] = elements[j];
            elements[j] = tmp;
            i = j;
        }
        return minElement;
    }

    bool empty() {
        return size == 0;
    }
};

void addEdge(int u, int v, int weight) {
    Edge *edge = new Edge();
    edge->to = v;
    edge->weight = weight;
    edge->next = Graph[u];
    Graph[u] = edge;
}

int dijkstra(int start, int target, int n) {
    for (int i = 0; i < n; i++) {
        Distance[i] = INF;
        visited[i] = false;
        parent[i] = -1;
    }
    Distance[start] = 0;
    PriorityQueue pq;
    pq.push(start, 0);

    while (!pq.empty()) {
        PriorityQueue::Element element = pq.pop();
        int u = element.node;

        if (visited[u])
            continue;
        visited[u] = true;

        if (u == target)
            return Distance[u];

        for (Edge *i = Graph[u]; i != nullptr; i = i->next) {
            int v = i->to;
            int weight = i->weight;
            if (!visited[v] && Distance[u] + weight < Distance[v]) {
                Distance[v] = Distance[u] + weight;
                parent[v] = u;
                pq.push(v, Distance[v]);
            }
        }
    }
    return -1;
}

void printPath(int start, int target) {
    if (Distance[target] == INF) {
        cout << "No path found!" << endl;
        return;
    }

    int current = target;
    cout << "Path: ";
    while (current != -1) {
        cout << current + 1 << " ";
        current = parent[current];
    }
    cout << endl;
}

int main() {
    int n, m;
    cin >> n >> m;
    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        addEdge(u - 1, v - 1, w);
    }
    int s, t;
    cin >> s >> t;

    int shortest_path = dijkstra(s - 1, t - 1, n);
    cout << "Shortest Path Distance: " << shortest_path << endl;

    printPath(s - 1, t - 1);

    return 0;
}