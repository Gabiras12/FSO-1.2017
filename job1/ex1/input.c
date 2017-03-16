#include "input.h"

point getPoint() {
  point a;
  double x, y;
  scanf("%lf %lf", &x, &y);
  a.x = x;
  a.y = y;
  return a;
}

quadrilateral getPoints() {
  quadrilateral q;

  q.a = getPoint();
  q.b = getPoint();
  q.c = getPoint();
  q.d = getPoint();

  return q;
}
