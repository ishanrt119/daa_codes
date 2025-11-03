#include <bits/stdc++.h>
using namespace std;

struct Order {
long long timestamp;
string customerName;
string orderDetails;
};

// Merge function

void merge(vector<Order>& arr, int left, int mid, int right) {
int n1 = mid - left + 1;
int n2 = right - mid;

vector<Order> L(n1), R(n2);

for (int i = 0; i < n1; i++)
L[i] = arr[left + i];
for (int j = 0; j < n2; j++)
R[j] = arr[mid + 1 + j];

int i = 0, j = 0, k = left;

while (i < n1 && j < n2) {
if (L[i].timestamp <= R[j].timestamp)
arr[k++] = L[i++];
else
arr[k++] = R[j++];
}

while (i < n1) arr[k++] = L[i++];
while (j < n2) arr[k++] = R[j++];
}

// Recursive Merge Sort
void mergeSort(vector<Order>& arr, int left, int right) {
if (left < right) {
int mid = left + (right - left) / 2;
mergeSort(arr, left, mid);
mergeSort(arr, mid + 1, right);
merge(arr, left, mid, right);
}
}

int main() {
ios::sync_with_stdio(false);
cin.tie(nullptr);

const int N = 1'000'000; // 1 million records
vector<Order> orders(N);

// Random data generator
mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
uniform_int_distribution<long long> tsDist(1600000000LL, 1700000000LL); // random epoch
timestamps
uniform_int_distribution<int> nameDist(1, 1000000);

for (int i = 0; i < N; i++) {
orders[i].timestamp = tsDist(rng);
orders[i].customerName = "Customer" + to_string(nameDist(rng));
orders[i].orderDetails = "Order" + to_string(i);
}

cout << "Sorting " << N << " records...\n";

auto start = chrono::high_resolution_clock::now();
mergeSort(orders, 0, N - 1);
auto end = chrono::high_resolution_clock::now();

chrono::duration<double> elapsed = end - start;
cout << "Sorting completed in " << elapsed.count() << " seconds\n";

// Print first 10 sorted records to verify
cout << "\nFirst 10 sorted records:\n";
for (int i = 0; i < 10; i++) {
cout << orders[i].timestamp << " "
<< orders[i].customerName << " "
<< orders[i].orderDetails << "\n";
}

return 0;
}
