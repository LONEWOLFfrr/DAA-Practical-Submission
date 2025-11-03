#include <bits/stdc++.h>
using namespace std;
// Represents an edge between nodes
struct Edge {
int destination;
double base_cost; // Original cost (e.g., fuel/distance)
double current_cost; // Real-time modified cost
};
int main() {
ios::sync_with_stdio(false);
cin.tie(nullptr);
// --- Input ---
int num_stages;
cout << "Enter number of stages: " << flush;
cin >> num_stages;
vector<int> nodes_per_stage(num_stages);
long long total_nodes = 0;
cout << "Enter number of nodes in each stage (" << num_stages << " values): " << flush;
for (int i = 0; i < num_stages; ++i) {
cin >> nodes_per_stage[i];
total_nodes += nodes_per_stage[i];
}
vector<int> stage_start_index(num_stages);
int node_index = 0;
for (int i = 0; i < num_stages; ++i) {

stage_start_index[i] = node_index;
node_index += nodes_per_stage[i];
}
int num_edges;
cout << "Enter number of edges: " << flush;
cin >> num_edges;
vector<vector<Edge>> adj_list(total_nodes); // adjacency list
vector<vector<int>> reverse_adj(total_nodes); // reverse edges for incremental update
cout << "Enter each edge as: u v cost\n" << flush;
for (int i = 0; i < num_edges; ++i) {
int u, v;
double cost;
cin >> u >> v >> cost;
if (u < 0 || u >= total_nodes || v < 0 || v >= total_nodes) {
cerr << "Invalid edge node id\n";
return 0;
}
Edge e{v, cost, cost};
adj_list[u].push_back(e);
reverse_adj[v].push_back(u);
}
// --- DP Initialization ---
const double INF = 1e30;
vector<double> min_cost(total_nodes, INF); // DP table
vector<int> next_hop(total_nodes, -1); // path reconstruction
// Set cost = 0 for sink nodes in the last stage
int last_stage = num_stages - 1;
for (int k = 0; k < nodes_per_stage[last_stage]; ++k) {
int node = stage_start_index[last_stage] + k;
min_cost[node] = 0.0;
next_hop[node] = -1;
}
// --- Forward DP from sink to source ---
for (int stage = num_stages - 2; stage >= 0; --stage) {
for (int k = 0; k < nodes_per_stage[stage]; ++k) {
int u = stage_start_index[stage] + k;
double best = INF;
int best_node = -1;

for (auto &e : adj_list[u]) {
int v = e.destination;
double cost = e.current_cost;
if (min_cost[v] + cost < best) {
best = min_cost[v] + cost;
best_node = v;
}
}
min_cost[u] = best;
next_hop[u] = best_node;
}
}
// --- Output costs from stage-0 nodes ---
cout << fixed << setprecision(6);
cout << "\nBest costs from Stage-0 nodes:\n";
for (int k = 0; k < nodes_per_stage[0]; ++k) {
int u = stage_start_index[0] + k;
if (min_cost[u] >= INF / 2) cout << "Node " << u << ": unreachable\n";
else cout << "Node " << u << ": cost = " << min_cost[u] << "\n";
}
// --- Retrieve path from a source node ---
cout << "\nEnter a source node id (in stage 0) to print path, or -1 to skip: " << flush;
int source;
cin >> source;
if (source >= 0 && source < total_nodes) {
if (min_cost[source] >= INF / 2) {
cout << "No route from " << source << "\n";
} else {
cout << "Path from " << source << " : ";
int current = source;
double total_cost = 0.0;
while (current != -1) {
cout << current;
int next = next_hop[current];
if (next != -1) {
double edge_cost = 0;
bool found = false;
for (auto &e : adj_list[current]) {
if (e.destination == next) {
edge_cost = e.current_cost;
found = true;
break;

}
}
if (!found) edge_cost = 0;
total_cost += edge_cost;
cout << " -> ";
} else break;
current = next;
}
cout << "\nTotal route cost (sum edges): " << total_cost << "\n";
}
}
// --- Real-time cost updates ---
cout << "\nEnter number of live updates to edge costs (0 to finish): " << flush;
int updates;
cin >> updates;
while (updates-- > 0) {
int u, v;
double multiplier;
cout << "Enter edge update (u v multiplier): " << flush;
cin >> u >> v >> multiplier;
// Apply multiplier to edge cost from u to v
for (auto &e : adj_list[u]) {
if (e.destination == v) {
e.current_cost = e.base_cost * multiplier;
}
}
// Incremental DP Update
auto recompute = [&](int node) -> double {
double best = INF;
int best_node = -1;
for (auto &e : adj_list[node]) {
if (min_cost[e.destination] >= INF / 2) continue;
double candidate = e.current_cost + min_cost[e.destination];
if (candidate < best) {
best = candidate;
best_node = e.destination;
}
}
next_hop[node] = best_node;
return best;

};
queue<int> q;
double new_cost = recompute(u);
if (abs(new_cost - min_cost[u]) > 1e-9) {
min_cost[u] = new_cost;
q.push(u);
}
while (!q.empty()) {
int node = q.front(); q.pop();
for (int pred : reverse_adj[node]) {
double updated_cost = recompute(pred);
if (abs(updated_cost - min_cost[pred]) > 1e-9) {
min_cost[pred] = updated_cost;
q.push(pred);
}
}
}
}
// --- Output updated costs ---
cout << "\nAfter updates, best costs from Stage-0 nodes:\n";
for (int k = 0; k < nodes_per_stage[0]; ++k) {
int u = stage_start_index[0] + k;
if (min_cost[u] >= INF / 2) cout << "Node " << u << ": unreachable\n";
else cout << "Node " << u << ": cost = " << min_cost[u] << "\n";
}
// --- Retrieve updated path from source ---
cout << "\nEnter a source node id (in stage 0) to print path, or -1 to skip: " << flush;
cin >> source;
if (source >= 0 && source < total_nodes) {
if (min_cost[source] >= INF / 2) {
cout << "No route from " << source << "\n";
} else {
cout << "Path from " << source << " : ";
int current = source;
double total_cost = 0.0;
while (current != -1) {
cout << current;
int next = next_hop[current];
if (next != -1) {
double edge_cost = 0;

bool found = false;
for (auto &e : adj_list[current]) {
if (e.destination == next) {
edge_cost = e.current_cost;
found = true;
break;
}
}
if (!found) edge_cost = 0;
total_cost += edge_cost;
cout << " -> ";
} else break;
current = next;
}
cout << "\nTotal route cost (sum edges): " << total_cost << "\n";
}
}
return 0;
}
