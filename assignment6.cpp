#include <bits/stdc++.h>
using namespace std;
pair<int, vector<int>> knapsack(vector<int>& weights, vector<int>& values, int W, vector<int>&
priorities) {
int n = weights.size();
vector<vector<int>> dp(n + 1, vector<int>(W + 1, 0));
// DP computation with visualization
for (int i = 1; i <= n; i++) {
for (int w = 1; w <= W; w++) {
if (weights[i - 1] <= w) {
int effective_value = values[i - 1];
if (!priorities.empty())
effective_value += priorities[i - 1] * 1000;
dp[i][w] = max(dp[i - 1][w], dp[i - 1][w - weights[i - 1]] + effective_value);
} else {
dp[i][w] = dp[i - 1][w];
}
}
// ---- VISUALIZATION: print DP table after processing item i ----
cout << "\nDP Table after item " << i
<< " (weight=" << weights[i-1]
<< ", value=" << values[i-1]
<< ", perishable=" << priorities[i-1] << "):\n";
for (int r = 0; r <= i; r++) {
for (int c = 0; c <= W; c++) {
cout << setw(5) << dp[r][c] << " ";
}
cout << "\n";
}
cout << "--------------------------------------------------\n";
}
// Reconstruct chosen items
vector<int> chosen;
int w = W;
for (int i = n; i > 0; i--) {
if (dp[i][w] != dp[i - 1][w]) {
chosen.push_back(i - 1);
w -= weights[i - 1];
}

6

}
reverse(chosen.begin(), chosen.end());
return {dp[n][W], chosen};
}
int main() {
vector<int> weights = {10, 20, 30, 40, 15};
vector<int> values = {60, 100, 120, 150, 80};
vector<int> priorities = {1, 0, 0, 0, 1}; // 1 = perishable
int W = 50;
auto result = knapsack(weights, values, W, priorities);
cout << "\nMax Utility Achieved: " << result.first << "\n";
cout << "Items Selected:\n";
for (int idx : result.second) {
cout << " Item " << idx + 1
<< ": Weight=" << weights[idx]
<< ", Value=" << values[idx]
<< ", Perishable=" << (priorities[idx] ? "Yes" : "No") << "\n";
}
return 0;
}
