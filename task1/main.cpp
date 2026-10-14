#include <iomanip>
#include <iostream>

using namespace std;
using Ll = long long;

struct MinMax {
  int min;
  int max;
};

double F(const int* a, int n) {
  Ll sum = 0;
  for (int i = 0; i < n; ++i) {
    sum += a[i];
  }
  return 1.0 * sum / n;
}

MinMax G(const int* a, int n) {
  MinMax ans = {a[0], a[0]};
  for (int i = 1; i < n; ++i) {
    if (a[i] < ans.min) {
      ans.min = a[i];
    }
    if (a[i] > ans.max) {
      ans.max = a[i];
    }
  }
  return ans;
}

int H(const int* a, int n) {
  int ans = 0;
  for (int i = 0; i < n; ++i) {
    if (a[i] < 0) {
      ++ans;
    }
  }
  return ans;
}

int main() {
  int n;
  cin >> n;
  int* a = new int[n];
  for (int i = 0; i < n; ++i) {
    cin >> a[i];
  }

  MinMax ans = G(a, n);
  cout << fixed << setprecision(1) << F(a, n) << '\n';
  cout << ans.min << ' ' << ans.max << '\n';
  cout << H(a, n) << '\n';
  delete[] a;
}
