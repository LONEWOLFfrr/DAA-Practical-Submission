#include <bits/stdc++.h>
using namespace std;
// Dijkstra's algorithm to compute shortest paths from a source
void computeShortestPaths(int startNode, const vector<vector<pair<int,int>>> &adjList,
vector<int> &distance, vector<int> &predecessor) { int numVertices = adjList.size();
distance.assign(numVertices, INT_MAX);
predecessor.assign(numVertices, -1);
distance[startNode] = 0;
priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;
pq.push({0, startNode});
while (!pq.empty()) {
int current = pq.top().second;
int currDist = pq.top().first;
pq.pop();
if (currDist > distance[current]) continue;
for (auto &edge : adjList[current]) {
int neighbor = edge.first;
int weight = edge.second;
if (distance[neighbor] > distance[current] + weight) {
distance[neighbor] = distance[current] + weight;
predecessor[neighbor] = current;
pq.push({distance[neighbor], neighbor}); }
}
}
}
int main() {
int numIntersections, numRoads;
cout << "Enter number of intersections: ";
cin >> numIntersections;
cout << "Enter number of roads: ";
cin >> numRoads;

vector<vector<pair<int,int>>> cityGraph(numIntersections);
cout << "Enter roads (u v travelTime):\n";
for (int i = 0; i < numRoads; i++) {
int u, v, t;
cin >> u >> v >> t;
cityGraph[u].push_back({v, t}); // directed
// cityGraph[v].push_back({u, t}); // uncomment for undirected }
int ambulanceStart;
cout << "Enter ambulance starting intersection: ";
cin >> ambulanceStart;
int numHospitals;
cout << "Enter number of hospitals: ";
cin >> numHospitals;
vector<int> hospitalNodes(numHospitals);
cout << "Enter hospital locations: ";
for (int i = 0; i < numHospitals; i++) cin >> hospitalNodes[i];
vector<int> shortestDist, prevNode;
auto startTime = chrono::high_resolution_clock::now();
computeShortestPaths(ambulanceStart, cityGraph, shortestDist, prevNode);
auto endTime = chrono::high_resolution_clock::now();
chrono::duration<double> elapsed = endTime - startTime;
cout << "Dijkstra executed in " << fixed << setprecision(6)
<< elapsed.count() << " seconds.\n";
int minTime = INT_MAX, closestHospital = -1;
for (int h : hospitalNodes) {
if (shortestDist[h] < minTime) {
minTime = shortestDist[h];
closestHospital = h;
}
}
if (closestHospital == -1) {
cout << "No reachable hospital.\n";
} else {
cout << "Closest hospital is at node " << closestHospital <<
" with travel time " << minTime << " minutes.\n";
cout << "Path: ";
stack<int> route;
int curr = closestHospital;
while (curr != -1) {
route.push(curr);
curr = prevNode[curr];
}
while (!route.empty()) {
cout << route.top() << " ";
route.pop();
}
cout << "\n";

}
return 0;
}
