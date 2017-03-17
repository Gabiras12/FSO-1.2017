#include <stdlib.h>
#include <stdio.h>
#include "operations.h"


int * alloc_vector(int n){
  int * vector = (int*) malloc(sizeof(int) * n);
  return vector;
}

void print_vector(int * vector, int n){
  for(int i=0 ;i < n; i++) {
    printf("%d ", vector[i]);
  }
  printf("\n");
}
