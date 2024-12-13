#include <cstdlib>
#include <iostream>

using namespace std;

template <class T> class Memory {
public:
  static T **allocArray(int m, int n) {
    int i;
    T **p = (T **)malloc(m * sizeof(T *));
    p[0] = (T *)malloc(m * n * sizeof(T));

    for (i = 1; i < m; i++)
      p[i] = p[0] + i * n;
    return p;
  }
};

int main() {
  int **array;
  array = Memory<int>::allocArray(5, 10);
  int j, k;
  for (j = 0; j < 5; j++)
    for (k = 0; k < 10; k++)
      array[j][k] = j * 10 + k;
  for (j = 0; j < 5; j++)
    for (k = 0; k < 10; k++)
      cout << array[j][k] << " ";
}
