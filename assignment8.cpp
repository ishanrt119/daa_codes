#include <bits/stdc++.h>
using namespace std;

static const double INF = 1e18;

struct Node {
vector<vector<double>> red; // reduced cost matrix
double bound = 0.0; // lower bound after reduction
double acc_cost = 0.0; // real cost so far
double acc_time = 0.0; // real time so far
int level = 0; // number of edges fixed so far
int city = 0; // current city (last in path)
vector<int> path; // partial tour, starts at START
vector<char> used; // visited flags
};

struct Input {
int N;
vector<vector<double>> cost;
vector<vector<double>> timeM; // may equal cost if no time matrix
double TMAX;
int START;
};

// Reduce matrix: subtract row mins, then col mins, return added sum.
// Skips INF entries.
static double reduce_matrix(vector<vector<double>>& A) {
int n = (int)A.size();
double add = 0.0;

// Row reduction
for (int i = 0; i < n; ++i) {
double mn = INF;
for (int j = 0; j < n; ++j) mn = min(mn, A[i][j]);
if (mn > 0 && mn < INF/2) {
for (int j = 0; j < n; ++j) if (A[i][j] < INF/2) A[i][j] -= mn;
add += mn;
}
}
// Column reduction
for (int j = 0; j < n; ++j) {
double mn = INF;
for (int i = 0; i < n; ++i) mn = min(mn, A[i][j]);
if (mn > 0 && mn < INF/2) {
for (int i = 0; i < n; ++i) if (A[i][j] < INF/2) A[i][j] -= mn;
add += mn;
}
}
return add;
}

// Build child by fixing edge u->v. Also set constraints so we don't return to START too early.
static Node make_child(const Node& parent,
int u, int v,
const vector<vector<double>>& baseCost)
{

int n = (int)parent.red.size();
Node ch;
ch.red = parent.red; // copy
ch.bound = parent.bound;
ch.acc_cost = parent.acc_cost + baseCost[u][v];
ch.level = parent.level + 1;
ch.path = parent.path;
ch.path.push_back(v);
ch.used = parent.used;
ch.used[v] = 1;
ch.city = v;
ch.acc_time = parent.acc_time; // will be set by caller (needs time matrix)

// When we take u->v, block row u and column v
for (int j = 0; j < n; ++j) ch.red[u][j] = INF;
for (int i = 0; i < n; ++i) ch.red[i][v] = INF;
// To prevent immediate return to START before last step, forbid v->START unless final later
ch.red[v][parent.path.front()] = INF;

// Also forbid going to already used cities (except the final return)
for (int k = 0; k < n; ++k) if (ch.used[k]) ch.red[v][k] = INF;

// Reduce the new matrix to get an updated bound
ch.bound += reduce_matrix(ch.red);
return ch;
}

static Input read_input() {
ios::sync_with_stdio(false);
cin.tie(nullptr);

Input in;
int N; cin >> N;
in.N = N;
in.cost.assign(N, vector<double>(N));
for (int i = 0; i < N; ++i) {
for (int j = 0; j < N; ++j) {
double x; cin >> x;
if (i == j) in.cost[i][j] = INF;
else in.cost[i][j] = (x < 0 ? INF : x);
}
}
int hasTime; cin >> hasTime;
if (hasTime == 1) {
in.timeM.assign(N, vector<double>(N));
for (int i = 0; i < N; ++i) {
for (int j = 0; j < N; ++j) {
double x; cin >> x;
if (i == j) in.timeM[i][j] = INF;
else in.timeM[i][j] = (x < 0 ? INF : x);
}
}

} else {
in.timeM = in.cost; // assume time == cost
}
cin >> in.TMAX;
cin >> in.START;
return in;
}

int main() {
auto in = read_input();
int N = in.N;
int START = in.START;

// Initial node
Node root;
root.red = in.cost; // start from cost matrix
root.bound = reduce_matrix(root.red);
root.acc_cost = 0.0;
root.acc_time = 0.0;
root.level = 0;
root.city = START;
root.path = { START };
root.used.assign(N, 0);
root.used[START] = 1;

// Best solution found so far

double best_cost = INF, best_time = INF;
vector<int> best_tour;

// Min-heap by bound
auto cmp = [](const Node& a, const Node& b){ return a.bound > b.bound; };
priority_queue<Node, vector<Node>, decltype(cmp)> pq(cmp);
pq.push(root);

long long nodes_generated = 1, nodes_expanded = 0;

while (!pq.empty()) {
Node cur = pq.top(); pq.pop();
if (cur.bound >= best_cost) continue; // prune by cost bound
if (in.TMAX >= 0 && cur.acc_time > in.TMAX) continue; // prune by time limit

// If we fixed N-1 edges, only one city remains; close the tour.
if (cur.level == N - 1) {
// Find the last unvisited city
int last = -1;
for (int k = 0; k < N; ++k) if (!cur.used[k]) { last = k; break; }
if (last == -1) {
// already have N cities in path; just return to start
int u = cur.city;
if (in.cost[u][START] >= INF/2) continue; // no return edge
double final_cost = cur.acc_cost + in.cost[u][START];
double final_time = cur.acc_time + in.timeM[u][START];

if (in.TMAX >= 0 && final_time > in.TMAX) {
// violates time
} else if (final_cost < best_cost) {
best_cost = final_cost;
best_time = final_time;
best_tour = cur.path;
best_tour.push_back(START);
}
} else {
// take city -> last -> start
int u = cur.city, v = last;
if (in.cost[u][v] >= INF/2 || in.cost[v][START] >= INF/2) {
// infeasible close
} else {
double final_cost = cur.acc_cost + in.cost[u][v] + in.cost[v][START];
double final_time = cur.acc_time + in.timeM[u][v] + in.timeM[v][START];
if ((in.TMAX < 0 || final_time <= in.TMAX) && final_cost < best_cost) {
best_cost = final_cost;
best_time = final_time;
best_tour = cur.path;
best_tour.push_back(v);
best_tour.push_back(START);
}
}
}
continue;

}

// Expand from current city to any unvisited city v
int u = cur.city;
++nodes_expanded;

for (int v = 0; v < N; ++v) {
if (cur.used[v]) continue;
if (in.cost[u][v] >= INF/2) continue;

Node child = make_child(cur, u, v, in.cost);
child.acc_time = cur.acc_time + in.timeM[u][v];

// Quick admissible pruning:
// 1) If time already exceeds limit
if (in.TMAX >= 0 && child.acc_time > in.TMAX) continue;
// 2) If child's bound ≥ current best cost
if (child.bound >= best_cost) continue;

pq.push(child);
++nodes_generated;
}
}

// Report
if (best_cost >= INF/2) {

cout << "No feasible tour found";
if (in.TMAX >= 0) cout << " (time limit might be too tight)";
cout << ".\n";
cout << "Nodes generated: " << nodes_generated
<< ", expanded: " << nodes_expanded << "\n";
return 0;
}

cout.setf(std::ios::fixed); cout << setprecision(2);
cout << "Optimal tour cost: " << best_cost << "\n";
cout << "Total time: " << best_time << "\n";
cout << "Tour: ";
for (size_t i = 0; i < best_tour.size(); ++i) {
if (i) cout << " -> ";
cout << best_tour[i];
}
cout << "\n";
cout.unsetf(std::ios::fixed);
cout << "Nodes generated: " << nodes_generated
<< ", expanded: " << nodes_expanded << "\n";
return 0;
}
