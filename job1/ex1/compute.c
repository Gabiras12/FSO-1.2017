#include "compute.h"

double edgeSize(point p1, point p2) {
    double distance;
    distance = sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y-p2.y) *(p1.y-p2.y));
    return distance;
}

int quadrilateralIsConvex(quadrilateral q) {
    int got_negative = 0;
    int got_positive = 0;
    point quadrilateralToArray[4];
    quadrilateralToArray[0] = q.a;
    quadrilateralToArray[1] = q.b;
    quadrilateralToArray[2] = q.c;
    quadrilateralToArray[3] = q.d;
    int num_points = 4;
    int A, B, C;

    for (A = 0; A < num_points; A++) {
        B = (A + 1) % num_points;
        C = (B + 1) % num_points;

        double cross_product = crossProductLength(quadrilateralToArray[A],
          quadrilateralToArray[B],
          quadrilateralToArray[C]);

        if (cross_product < 0) {
            got_negative = 1;
        }
        else if (cross_product > 0) {
            got_positive = 1;
        }

        if (got_negative && got_positive) {
          return 0;
        }
    }

    // If we got this far, the polygon is convex.
    return 1;
}

// Return the cross product AB x BC.
// The cross product is a vector perpendicular to AB
// and BC having length |AB| * |BC| * Sin(theta) and
// with direction given by the right-hand rule.
// For two vectors in the X-Y plane, the result is a
// vector with X and Y components 0 so the Z component
// gives the vector's length and direction.
double crossProductLength(point a, point b, point c) {
  // Get the vectors' coordinates.
  double BAx = a.x - b.x;
  double BAy = a.y - b.y;
  double BCx = c.x - b.x;
  double BCy = c.y - b.y;

  // Calculate the Z coordinate of the cross product.
  return (BAx * BCy - BAy * BCx);
}

double quadrilateralArea(quadrilateral q) {
  if (quadrilateralIsConvex(q)) {
    triangle a;
    triangle b;

    a.a = q.a;
    a.b = q.b;
    a.c = q.c;

    b.a = q.c;
    b.b = q.d;
    b.c = q.a;

    return triangleArea(a) + triangleArea(b);
  }
}

double triangleArea(triangle t) {
  double a = edgeSize(t.a, t.b);
  double b = edgeSize(t.b, t.c);
  double c = edgeSize(t.c, t.a);
  double semi_perimetro = (a + b + c)/2;

	return sqrt(semi_perimetro*(semi_perimetro - a) * (semi_perimetro - b) *
		(semi_perimetro - c));
}
