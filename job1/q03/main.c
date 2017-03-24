#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sort.h"
#include "operations.h"
#define MAX 20

int main (int argc, char *argv[]){

  int * vector = alloc_vector(MAX);
  int len = 0;

  while(1)
  {
    scanf("%d", &vector[len]);
    if(vector[len] < 0 || len == MAX){
      break;
    }
    len ++;
  }

  int * vector_sorted;

  if (argv[1] == NULL){
    vector_sorted = sort_ascending(vector, len);
    printf("Saida crescente:\n");
  }
  else if(strcmp(argv[1],"-d") == 0){
    vector_sorted = sort_ascending(vector, len);
    printf("Saida crescente:\n");
  }
  else if (strcmp(argv[1],"-r") == 0) {
    vector_sorted = sort_descending(vector, len);
    printf("Saida decrescente:\n");
  }

  print_vector(vector_sorted, len);

  free(vector);

  return 0;
}
