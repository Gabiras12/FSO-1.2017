#ifndef COMPUTE
#define COMPUTE

#include <math.h>
#include "types.h"

double edgeSize(point p1, point p2);
double crossProductLength(point a, point b, point c);
int quadrilateralIsConvex(quadrilateral q);
double quadrilateralArea(quadrilateral q);
double triangleArea(triangle t);

#endif
