#include <bits/stdc++.h>
using namespace std;
using Cost = double;
const Cost INF = 1e18;
struct Edge {
int from;
int to;
Cost base_cost;
Cost current_cost;
Edge(int f, int t, Cost b): from(f), to(t), base_cost(b), current_cost(b) {}
};
struct Node {
int id;
int stage;
Node(int _id=0, int _stage=0): id(_id), stage(_stage) {}
};
class MultiStageGraph {
public:
int num_nodes;
int num_stages;
vector<Node> nodes;
vector<vector<int>> nodes_in_stage;
vector<vector<Edge>> adj;
vector<Edge*> all_edges;

double w_traffic = 1.0, w_weather = 1.0, w_fuel = 1.0;
double traffic_level = 0.0, weather_level = 0.0, fuel_level = 0.0;
MultiStageGraph(int n_nodes, int n_stages)
: num_nodes(n_nodes), num_stages(n_stages),
nodes(n_nodes), nodes_in_stage(n_stages), adj(n_nodes) {}
void setNode(int id, int stage) {
nodes[id] = Node(id, stage);
nodes_in_stage[stage].push_back(id);
}
void addEdge(int from, int to, Cost base_cost) {
adj[from].emplace_back(from, to, base_cost);
all_edges.push_back(&adj[from].back());
}
void setRealTimeFactors(double _traffic_level, double _weather_level, double _fuel_level,
double _w_traffic=1.0, double _w_weather=1.0, double _w_fuel=1.0) {

traffic_level = _traffic_level;
weather_level = _weather_level;
fuel_level = _fuel_level;
w_traffic = _w_traffic; w_weather = _w_weather; w_fuel = _w_fuel;
recomputeEdgeCosts();
}
void recomputeEdgeCosts() {
for (Edge* e : all_edges) {
double composite = 1.0 + w_traffic * traffic_level + w_weather * weather_level + w_fuel *

fuel_level;
if (composite < 0.0) composite = 0.0;
e->current_cost = e->base_cost * composite;
}
}
struct DPResult {
vector<Cost> dist;
vector<int> parent;
};
DPResult runMultistageDP(const vector<int>& start_nodes) {
vector<Cost> dist(num_nodes, INF);
vector<int> parent(num_nodes, -1);
for (int s : start_nodes) {

dist[s] = 0.0;
parent[s] = -1;
}
for (int st = 0; st < num_stages; ++st) {
for (int u : nodes_in_stage[st]) {
if (dist[u] == INF) continue;
for (const Edge &e : adj[u]) {
int v = e.to;
if (nodes[v].stage <= nodes[u].stage) continue;
Cost nd = dist[u] + e.current_cost;
if (nd < dist[v]) {
dist[v] = nd;
parent[v] = u;
}
}
}
}
return {dist, parent};
}
vector<int> reconstructPath(int target, const vector<int>& parent) {
vector<int> path;
if (target < 0 || target >= num_nodes) return path;
if (parent[target] == -1 && nodes[target].stage != 0) {
if (nodes[target].stage==0) path.push_back(target);
return path;
}
int cur = target;
while (cur != -1) {
path.push_back(cur);
cur = parent[cur];
}
reverse(path.begin(), path.end());
return path;
}
unordered_map<int, pair<Cost, vector<int>>> batchQuery(const vector<int>& start_nodes, const
vector<int>& targets) {
DPResult res = runMultistageDP(start_nodes);
unordered_map<int, pair<Cost, vector<int>>> out;
for (int t : targets) {
Cost c = res.dist[t];
vector<int> path;
if (c < INF) path = reconstructPath(t, res.parent);

out[t] = {c, path};
}
return out;
}
};
int main() {
ios::sync_with_stdio(false);
cin.tie(nullptr);
int N_stages = 3;
int n_nodes = 6;
MultiStageGraph G(n_nodes, N_stages);
G.setNode(0, 0);
G.setNode(1, 0);
G.setNode(2, 1);
G.setNode(3, 1);
G.setNode(4, 2);
G.setNode(5, 2);
G.addEdge(0, 2, 10.0);
G.addEdge(0, 3, 12.0);
G.addEdge(1, 2, 9.0);
G.addEdge(1, 3, 11.0);
G.addEdge(2, 4, 15.0);
G.addEdge(2, 5, 20.0);
G.addEdge(3, 4, 14.0);
G.addEdge(3, 5, 16.0);
G.setRealTimeFactors(0.2, 0.1, 0.05, 1.0, 0.5, 0.5);
vector<int> starts = {0,1};
vector<int> targets = {4,5};
auto answers = G.batchQuery(starts, targets);
for (int t : targets) {
auto &p = answers[t];
Cost best = p.first;
cout << "Target node " << t << ": ";
if (best >= INF/2) cout << "UNREACHABLE\n";
else {
cout << "cost = " << best << ", path = ";
for (int id : p.second) {
cout << id;

if (id != p.second.back()) cout << " -> ";
}
cout << "\n";
}
}
for (Edge* e : G.all_edges) {
if (e->from == 2 && e->to == 4) e->base_cost = 1e6;
}
G.recomputeEdgeCosts();
cout << "\nAfter closure of edge 2->4:\n";
answers = G.batchQuery(starts, targets);
for (int t : targets) {
auto &p = answers[t];
Cost best = p.first;
cout << "Target node " << t << ": ";
if (best >= INF/2) cout << "UNREACHABLE\n";
else {
cout << "cost = " << best << ", path = ";
for (int id : p.second) {
cout << id;
if (id != p.second.back()) cout << " -> ";
}
cout << "\n";
}
}
return 0;
}
