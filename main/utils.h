#ifndef TEXHELPER_UTILS_H
#define TEXHELPER_UTILS_H

namespace Vahe {
int abs(int a) {
  return a > 0 ? a : -a;
}

int gfc(int a, int b) {
  if (!b)
    return abs(a);
  return abs(gfc(b, a % b));
}

int euclid(int a, int b, int &x, int &y) {
  if (a == 0) {
    x = 0;
    y = 1;
    return b;
  }
  int x1, y1;
  int d = euclid(b % a, a, x1, y1);
  x = y1 - (b / a) * x1;
  y = x1;
  return d;
}
}

#endif // TEXHELPER_UTILS_H
