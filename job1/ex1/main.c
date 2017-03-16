#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "compute.h"
#include "input.h"

int main() {
  quadrilateral q;

  q = getPoints();
  int isConvex = quadrilateralIsConvex(q);
  
  if (isConvex) {
    printf("Quadrilatero convexo.\n");
    printf("Área: %lf\n", quadrilateralArea(q));
  } else {
    printf("Quadrilatero não convexo.\n");
  }

}
