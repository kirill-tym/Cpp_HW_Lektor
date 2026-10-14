#include <cstring>
#include <iomanip>
#include <iostream>
#include <limits>

using namespace std;
using Ll = long long;

void Deposit(Ll& sum) {
  Ll k;
  cin >> k;
  if (k <= 0) {
    cout << "error\n";
    return;
  }
  sum += k;
}

void Withdraw(Ll& sum) {
  Ll k;
  cin >> k;
  if (k <= 0 || k > sum) {
    cout << "error\n";
    return;
  }
  sum -= k;
}

void Balance(Ll& sum) { cout << sum << '\n'; }

void Reset(Ll& sum) { sum = 0; }

void Inc(Ll& sum) { ++sum; }

struct Command {
  const char* name;
  void (*f)(Ll&);
};

int main() {
  Command a[] = {{"deposit", Deposit},
                 {"withdraw", Withdraw},
                 {"balance", Balance},
                 {"reset", Reset},
                 {"inc", Inc}};
  Ll sum = 0;
  char cmd[32];

  while (cin >> setw(sizeof(cmd)) >> cmd) {
    if (strcmp(cmd, "stop") == 0) {
      break;
    }
    bool flag = false;
    for (Command c : a) {
      if (strcmp(cmd, c.name) == 0) {
        c.f(sum);
        flag = true;
        break;
      }
    }
    if (!flag) {
      cout << "error\n";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
  }
}
