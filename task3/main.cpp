#include <cstring>
#include <iomanip>
#include <iostream>

using namespace std;

constexpr int c_n = 5;
constexpr int c_m = 10;

bool Inside(int x, int y) { return x >= 0 && x < c_m && y >= 0 && y < c_n; }

void Clear(char a[][c_m]) {
  for (int i = 0; i < c_n; ++i) {
    for (int j = 0; j < c_m; ++j) {
      a[i][j] = ' ';
    }
  }
}

bool Put(char a[][c_m], int x, int y, char c) {
  if (!Inside(x, y)) {
    return false;
  }
  a[y][x] = c;
  return true;
}

bool Text(char a[][c_m], int x, int y, const char* s) {
  int n = strlen(s);
  if (!Inside(x, y) || n > c_m - x) {
    return false;
  }
  for (int i = 0; i < n; ++i) {
    Put(a, x + i, y, s[i]);
  }
  return true;
}

void Print(const char a[][c_m]) {
  for (int i = 0; i < c_n; ++i) {
    for (int j = 0; j < c_m; ++j) {
      cout << a[i][j];
    }
    cout << '\n';
  }
}

int main() {
  char a[c_n][c_m];
  Clear(a);
  char cmd[8];

  while (cin >> setw(sizeof(cmd)) >> cmd) {
    if (strcmp(cmd, "quit") == 0) {
      break;
    }
    if (strcmp(cmd, "clear") == 0) {
      Clear(a);
    } else if (strcmp(cmd, "print") == 0) {
      Print(a);
    } else if (strcmp(cmd, "put") == 0) {
      int x;
      int y;
      char c;
      cin >> x >> y >> c;
      if (!Put(a, x, y, c)) {
        cout << "error\n";
      }
    } else if (strcmp(cmd, "text") == 0) {
      int x;
      int y;
      char s[c_m + 1];
      cin >> x >> y >> setw(sizeof(s)) >> s;
      if (!Text(a, x, y, s)) {
        cout << "error\n";
      }
    }
  }
}
