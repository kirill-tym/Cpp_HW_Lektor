#include <cstring>
#include <iomanip>
#include <iostream>

using namespace std;
using Ll = long long;

int main() {
  // программа хранит запасы на складе зона -> стеллаж -> ячейка
  // int*** нужен для трёхуровневого массива: число стеллажей
  // в каждой зоне и число ячеек на каждом стеллаже может различаться
  // void* хранит данные записей журнала. int[4] для команды set
  // и char[] для заметки note. Массив type задаёт тип каждой записи
  // после чтения журнала выполняем записи и выводим запасы по зонам
  int n;
  cin >> n;
  int*** a = new int**[n];
  int* m = new int[n];
  int** k = new int*[n];

  for (int i = 0; i < n; ++i) {
    cin >> m[i];
    a[i] = new int*[m[i]];
    k[i] = new int[m[i]];
    for (int j = 0; j < m[i]; ++j) {
      cin >> k[i][j];
      a[i][j] = new int[k[i][j]]();
    }
  }

  int q;
  cin >> q;
  void** data = new void*[q];
  int* type = new int[q];

  for (int i = 0; i < q; ++i) {
    char cmd[5];
    cin >> setw(sizeof(cmd)) >> cmd;
    if (strcmp(cmd, "set") == 0) {
      int* p = new int[4];
      cin >> p[0] >> p[1] >> p[2] >> p[3];
      data[i] = p;
      type[i] = 0;
    } else {
      char* s = new char[101];
      cin >> setw(101) >> s;
      data[i] = s;
      type[i] = 1;
    }
  }

  for (int i = 0; i < q; ++i) {
    if (type[i] == 0) {
      int* p = static_cast<int*>(data[i]);
      a[p[0]][p[1]][p[2]] = p[3];
      delete[] p;
    } else {
      char* s = static_cast<char*>(data[i]);
      cout << "note: " << s << '\n';
      delete[] s;
    }
  }
  delete[] data;
  delete[] type;

  for (int i = 0; i < n; ++i) {
    Ll sum = 0;
    for (int j = 0; j < m[i]; ++j) {
      for (int t = 0; t < k[i][j]; ++t) {
        sum += a[i][j][t];
      }
      delete[] a[i][j];
    }
    cout << "zone " << i << ": " << sum << '\n';
    delete[] a[i];
    delete[] k[i];
  }
  delete[] a;
  delete[] m;
  delete[] k;
}
