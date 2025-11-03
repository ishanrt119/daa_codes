#include <bits/stdc++.h>
using namespace std;
struct Item {
double weight, value;
bool divisible; // true for food/water, false for medicine kits
};

7

// Comparator based on value-to-weight ratio
bool cmp(const Item &a, const Item &b) {
double r1 = a.value / a.weight;
double r2 = b.value / b.weight;
return r1 > r2;
}
double fractionalKnapsack(double W, vector<Item> &items) {
sort(items.begin(), items.end(), cmp);
double totalValue = 0.0;
double currentWeight = 0.0;
cout << "Items selected for transport:\n";
for (auto &item : items) {
if (currentWeight + item.weight <= W) {
// Take the whole item
currentWeight += item.weight;
totalValue += item.value;
cout << " Took full item | Weight: " << item.weight
<< " | Value: " << item.value
<< " | Divisible: " << (item.divisible ? "Yes" : "No") << "\n";
} else {
double remain = W - currentWeight;
if (remain <= 0) break;
if (item.divisible) {
// Take fraction
double fraction = remain / item.weight;
totalValue += item.value * fraction;
cout << " Took fraction " << fraction * 100 << "% of item | Weight: " << remain
<< " | Value: " << item.value * fraction
<< " | Divisible: Yes\n";
currentWeight += remain;
break; // boat is full
} else {
// Cannot take indivisible item partially
continue;
}
}
}
cout << "Total weight on boat: " << currentWeight << "/" << W << " kg\n";

8

cout << "Maximum utility value: " << totalValue << "\n";
return totalValue;
}
int main() {
ios::sync_with_stdio(false);
cin.tie(nullptr);
double W = 50; // max weight capacity of boat
// Example items: {weight, value, divisible}
vector<Item> items = {
{10, 60, false}, // Medicine kit
{20, 100, true}, // Food
{30, 120, true}, // Water
{25, 180, false}, // Another medical kit
{15, 90, true} // More food
};
fractionalKnapsack(W, items);
return 0;
}
