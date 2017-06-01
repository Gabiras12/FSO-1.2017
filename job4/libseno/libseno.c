#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "libseno.h"

#define MAX_TERMS 10
#define PI 3.1415926535897932384626433832795
#define A0 1.5707288
#define A1 -0.2121144
#define A2 0.0742610
#define A3 -0.0187293



unsigned long factorial(unsigned long f)
{
  if ( f == 0 )
  return 1;
  return(f * factorial(f - 1));
}

double seno(double angulo){
  double seno = 0;

  for(int i=0; i<MAX_TERMS; i++) { // That's Taylor series!!
    seno += pow(-1, i) * pow(angulo, 2 * i + 1) / factorial(2 * i + 1);
  }
  return seno;
}

double arc_seno(double seno){
  double arc_seno = 0;

  arc_seno = PI/2 - sqrt(1-seno) * (A0 + A1 * seno + A2 * pow(seno,2) + A3 * pow(seno, 3));

  return arc_seno;
}
