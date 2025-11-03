#include <bits/stdc++.h>
using namespace std;

struct Edge {
int to;
double weight; // travel time
};

using Graph = vector<vector<Edge>>;

vector<double> dijkstra(const Graph &graph, int source) {
int n = graph.size();
vector<double> dist(n, 1e18); // large number = infinity
dist[source] = 0.0;

typedef pair<double, int> P; // {distance, node}
priority_queue<P, vector<P>, greater<P>> pq;
pq.push({0.0, source});

while (!pq.empty()) {
P top = pq.top();
pq.pop();

double d = top.first;

9

int
u
= top.second;

if (d
> dist[u]) continue; // stale entry

for (auto &edge : graph[u])
{

int
v
= edge.to;
double
w
= edge.weight;

if (dist[u]
+
w
< dist[v])
{

dist[v]
= dist[u]
+ w;
pq.push({dist[v], v});
}
}
}

return dist; }

int main()
{

ios::sync_with_stdio(false);
cin.tie(nullptr);

int
n
= 6; // intersections
Graph graph(n);

// Roads (u ->

v, time in minutes)
graph[0].push_back({1, 4});
graph[0].push_back({2, 2});
graph[1].push_back({2, 5});
graph[1].push_back({3, 10});
graph[2].push_back({3, 3});
graph[3].push_back({4, 4});
graph[4].push_back({5,
11});

1
0

int source = 0; // ambulance starting point
vector<int> hospitals = {3, 5}; // possible destinations

auto dist = dijkstra(graph, source);

cout << "Shortest travel times from Source " << source << ":\n";
for (int h : hospitals) {
cout << "To Hospital at " << h << " = " << dist[h] << " minutes\n";
}

return 0;
}
