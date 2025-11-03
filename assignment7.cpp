#include <bits/stdc++.h>
using namespace std;

// Super simple: build conflict graph, run Welsh–Powell (degree-desc order) + greedy
coloring.

int main() {
ios::sync_with_stdio(false);
cin.tie(nullptr);

int C, M;
if (!(cin >> C >> M)) return 0;

vector<vector<int>> adj(C);

auto add_edge = [&](int u, int v){
if (u==v) return;
adj[u].push_back(v);
adj[v].push_back(u);
};

for (int i = 0; i < M; ++i) {
int u, v; cin >> u >> v;
add_edge(u, v);
}

// Order vertices by descending degree (Welsh–Powell)
vector<int> order(C);
iota(order.begin(), order.end(), 0);
sort(order.begin(), order.end(), [&](int a, int b){
if (adj[a].size() != adj[b].size()) return adj[a].size() > adj[b].size();
return a < b;
});

// Greedy coloring in that order
vector<int> color(C, -1);
vector<int> used(C+1, 0);
int stamp = 1;

for (int u : order) {
// mark neighbor colors
for (int v : adj[u]) if (color[v] != -1) used[color[v]] = stamp;
int c = 0; while (used[c] == stamp) ++c;
color[u] = c;
++stamp;
}

int slots = *max_element(color.begin(), color.end()) + 1;
cout << "Slots used: " << slots << "\n";

// Print courses per slot
vector<vector<int>> per_slot(slots);
for (int u = 0; u < C; ++u) per_slot[color[u]].push_back(u);

for (int s = 0; s < slots; ++s) {
cout << "Slot " << s << ": ";
for (int i = 0; i < (int)per_slot[s].size(); ++i) {
if (i) cout << ' ';
cout << per_slot[s][i];
}
cout << "\n";
}

return 0;
}
