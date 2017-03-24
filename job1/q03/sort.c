#include <stdlib.h>
#include <stdio.h>
#include "sort.h"

int * sort_ascending(int * vector, int len){
  int swap = 0;

  for (int c = 0 ; c < ( len - 1 ); c++)
  {
    for (int d = 0 ; d < len - c - 1; d++)
    {
      if (vector[d] > vector[d+1])
      {
        swap = vector[d];
        vector[d] = vector[d+1];
        vector[d+1] = swap;
      }
    }
  }

  return vector;
}

int * sort_descending(int * vector, int len){
  int swap = 0;

  for (int c = 0 ; c < ( len - 1 ); c++)
  {
    for (int d = 0 ; d < len - c - 1; d++)
    {
      if (vector[d] < vector[d+1])
      {
        swap = vector[d];
        vector[d] = vector[d+1];
        vector[d+1] = swap;
      }
    }
  }

  return vector;
}
