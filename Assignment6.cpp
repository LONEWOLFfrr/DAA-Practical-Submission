#include <bits/stdc++.h>
using namespace std;

// Structure to represent an item
struct Item {
int itemID;
int weight;
int utility;
bool isPerishable;
};

// ================== Brute Force Approach ==================
int knapsackBruteForce(const vector<Item>& items, int capacity, int index = 0) {
if (index == (int)items.size() || capacity <= 0) return 0;

// Option 1: skip current item
int skipItem = knapsackBruteForce(items, capacity, index + 1);

6

// Option 2: take current item if it fits
int takeItem = 0;
if (items[index].weight <= capacity)
takeItem = items[index].utility + knapsackBruteForce(items, capacity - items[index].weight,
index + 1);

return max(takeItem, skipItem);
}

// ================== Dynamic Programming Approach ==================

pair<int, vector<int>> knapsackDP(const vector<Item>& items, int capacity) {
int itemCount = items.size();
vector<vector<int>> dpTable(itemCount + 1, vector<int>(capacity + 1, 0));

for (int i = 1; i <= itemCount; ++i) {
for (int w = 0; w <= capacity; ++w) {
dpTable[i][w] = dpTable[i-1][w]; // not taking item i
if (items[i-1].weight <= w)
dpTable[i][w] = max(dpTable[i][w], dpTable[i-1][w - items[i-1].weight] + items[i-1].utility);
}
}

// Traceback to find selected items
int remainingUtility = dpTable[itemCount][capacity];
vector<int> selectedItems;
int w = capacity;
for (int i = itemCount; i > 0 && remainingUtility > 0; --i) {
if (remainingUtility == dpTable[i-1][w]) continue;
else {
selectedItems.push_back(items[i-1].itemID);
remainingUtility -= items[i-1].utility;

7

w -= items[i-1].weight;
}
}
reverse(selectedItems.begin(), selectedItems.end());
return {dpTable[itemCount][capacity], selectedItems};
}

// ================== Greedy Heuristic Approach ==================
pair<int, vector<int>> greedyKnapsack(vector<Item> items, int capacity) {
sort(items.begin(), items.end(), [](const Item& a, const Item& b) {
double ratioA = (double)a.utility / a.weight;
double ratioB = (double)b.utility / b.weight;
return ratioA > ratioB;
});

int totalUtility = 0, totalWeight = 0;
vector<int> chosenItems;

for (auto &item : items) {
if (totalWeight + item.weight <= capacity) {
totalWeight += item.weight;
totalUtility += item.utility;
chosenItems.push_back(item.itemID);
}
}
return {totalUtility, chosenItems};
}

// ================== Main Function ==================
int main() {
ios::sync_with_stdio(false);
cin.tie(nullptr);

8

int numItems, truckCapacity;
cout << "Enter number of items and truck capacity: ";
cin >> numItems >> truckCapacity;

vector<Item> items(numItems);
cout << "Enter weight, utility, perishability(1=perishable,0=non):\n";
for (int i = 0; i < numItems; ++i) {
cin >> items[i].weight >> items[i].utility >> items[i].isPerishable;
items[i].itemID = i + 1;

// Increase priority for perishable items by boosting their utility
if (items[i].isPerishable)
items[i].utility = (int)(items[i].utility * 1.5);
}

cout << "\n========= Dynamic Programming Solution =========\n";
auto [maxUtilityDP, selectedDP] = knapsackDP(items, truckCapacity);
cout << "Max Utility (DP): " << maxUtilityDP << "\nItems chosen: ";
for (int id : selectedDP) cout << id << " ";
cout << "\n";

if (numItems <= 20) {
cout << "\n========= Brute Force (Verification) =========\n";
cout << "Max Utility (Brute Force): " << knapsackBruteForce(items, truckCapacity) << "\n";
}

cout << "\n========= Greedy Heuristic (Fast Approximation) =========\n";
auto [approxUtilityGreedy, selectedGreedy] = greedyKnapsack(items, truckCapacity);
cout << "Approx Utility (Greedy): " << approxUtilityGreedy << "\nItems chosen: ";
for (int id : selectedGreedy) cout << id << " ";
cout << "\n";

9

cout << "\n========= Performance Comparison =========\n";
cout << "DP: Optimal, O(N*W)\n";
cout << "Greedy: Fast, may miss optimum\n";
cout << "Brute Force: Exact but exponential\n";
cout << "---------------------------------------------\n";
cout << "Model easily extendable to multi-truck scenario.\n";

return 0;
}
