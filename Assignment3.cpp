#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <string>
using namespace std;
struct AidItem {
string itemName;

5

double mass;
double utility;
bool canSplit;
int priorityLevel;
AidItem(string n, double w, double u, bool d, int p)
: itemName(move(n)), mass(w), utility(u), canSplit(d), priorityLevel(p) {}
double utilityPerKg() const { return utility / mass; }
};
double loadPriorityItems(vector<AidItem> &inventory, double maxCapacity, double
&weightLoaded)
{ weightLoaded = 0.0;
double totalUtility = 0.0;
for (int pr = 1; pr <= 3; ++pr) {
vector<AidItem> currentGroup;
for (const auto &item : inventory)
if (item.priorityLevel == pr)
currentGroup.push_back(item);
if (currentGroup.empty()) continue;
sort(currentGroup.begin(), currentGroup.end(),
[](const AidItem &a, const AidItem &b) {
return a.utilityPerKg() > b.utilityPerKg();
});
cout << "\nProcessing priority " << pr << " items:\n";
for (const auto &item : currentGroup) {
if (!item.canSplit && maxCapacity > 0.0) {
if (item.mass <= maxCapacity) {
totalUtility += item.utility;
maxCapacity -= item.mass;
weightLoaded += item.mass;
cout << " Loaded (full): " << item.itemName << " (" << item.mass << " kg,
utility " << item.utility << ")\n";
} else {
cout << " Skipped (full, no space): " << item.itemName << "\n"; }
}
}
for (const auto &item : currentGroup) {
if (maxCapacity <= 0.0) break;
if (item.canSplit) {

6

double takeMass = min(item.mass, maxCapacity); double
utilTaken = item.utilityPerKg() * takeMass; totalUtility +=
utilTaken;
maxCapacity -= takeMass;
weightLoaded += takeMass;
cout << " Loaded (partial): " << item.itemName << " (" << takeMass << " kg, utility " <<
fixed << setprecision(2) << utilTaken << ")\n";
}
}
}
return totalUtility;
}
int main() {
int numItems;
cout << "Enter number of aid items: ";
if (!(cin >> numItems) || numItems <= 0) {
cerr << "Invalid number.\n";
return 1;
}
vector<AidItem> inventory;
for (int i = 0; i < numItems; ++i) {
string name;
double mass = 0.0, utility = 0.0;
int splitInt = 0, priority = 3;
cout << "\nItem #" << (i + 1) << ":\n";
cout << "Name: ";
getline(cin >> ws, name);
cout << "Mass (kg): "; cin >> mass;
cout << "Utility value: "; cin >> utility;
cout << "Divisible? (1 = yes, 0 = no): "; cin >> splitInt;
cout << "Priority (1 = High, 2 = Medium, 3 = Low): "; cin >> priority;
inventory.emplace_back(name, mass, utility, splitInt == 1, priority);
}
double maxLoad;
cout << "\nEnter maximum weight capacity of the boat (kg): ";
cin >> maxLoad;
double loadedWeight = 0.0;
double totalUtilityValue = loadPriorityItems(inventory, maxLoad, loadedWeight);
cout << "\n===== Final Load Report =====\n";

7
cout << "Total weight loaded: " << fixed << setprecision(2) << loadedWeight << " kg\n"; cout
<< "Total utility achieved: " << fixed << setprecision(2) << totalUtilityValue << " units\n";
return 0;
}
