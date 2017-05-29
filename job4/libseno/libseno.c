#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "libseno.h"

#define MAX_TERMS 10
#define PI 3.1415926535897932384626433832795

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
  double arc_cos = 0;

  arc_cos =  (-0.69813170079773212 * seno * seno - 0.87266462599716477) * seno + 1.5707963267948966;
  arc_seno = PI/2 - arc_cos;

  return arc_seno;
}
